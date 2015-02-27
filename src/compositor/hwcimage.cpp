/***************************************************************************
**
** Copyright (C) 2015 Jolla Ltd.
** Contact: Gunnar Sletta <gunnar.sletta@jollamobile.com>
**
** This file is part of lipstick.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include "hwcimage.h"
#include "hwcrenderstage.h"

#include <QRunnable>
#include <QThreadPool>

#include <QQuickWindow>
#include <QSGSimpleTextureNode>

#include <EGL/egl.h>
#include <EGL/eglext.h>

static void *hwcimage_eglbuffer_to_handle(EGLClientBuffer buffer);
static bool hwcimage_is_enabled();

#define HWCIMAGE_LOAD_EVENT ((QEvent::Type) (QEvent::User + 1))

class HwcImageLoadRequest : public QRunnable, public QEvent
{
public:
    HwcImageLoadRequest()
        : QEvent(HWCIMAGE_LOAD_EVENT)
    {
        setAutoDelete(false);
    }

    ~HwcImageLoadRequest() {
        qCDebug(LIPSTICK_LOG_HWC, "HwcImageLoadRequest completed and destroyed...");
    }

    void execute() {
        image = QImage(file).convertToFormat(QImage::Format_RGBX8888);

        if (rotation != 0) {
            QTransform xform;
            xform.rotate(rotation);
            image = image.transformed(xform, Qt::FastTransformation);
        }

        if (textureSize.width() > 0 && textureSize.height() > 0)
            image = image.scaled(textureSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        if (image.size().isValid()) {

            QPainter p(&image);

            // Apply overlay
            if (overlay.isValid())
                p.fillRect(image.rect(), overlay);

            // Apply glass..
            if (effect.contains(QStringLiteral("glass"))) {
                QImage glass("//usr/share/themes/jolla-ambient/meegotouch/icons/graphic-shader-texture.png");
                glass = glass.scaled(glass.width(), glass.height(),
                                     Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                p.save();
                p.setOpacity(0.1);
                p.setCompositionMode(QPainter::CompositionMode_Plus);
                p.fillRect(image.rect(), glass);
                p.restore();
            }
        }
    }

    void run() {
        execute();
        mutex.lock();
        if (hwcImage && hwcImage->m_pendingRequest == this) {
            hwcImage->m_pendingRequest = 0;
            QCoreApplication::postEvent(hwcImage, this);
        } else {
            setAutoDelete(true);
        }
        mutex.unlock();
    }

    QImage image;
    QString file;
    QString effect;
    QColor overlay;
    QSize textureSize;
    qreal pixelRatio;
    qreal rotation;

    HwcImage *hwcImage;

    static QMutex mutex;
};

QMutex HwcImageLoadRequest::mutex;

HwcImage::HwcImage()
    : m_pendingRequest(0)
    , m_rotationHandler(0)
    , m_status(Null)
    , m_textureRotation(0)
    , m_asynchronous(true)
{
    setFlag(ItemHasContents, true);
}

HwcImage::~HwcImage()
{
    HwcImageLoadRequest::mutex.lock();
    if (m_pendingRequest)
        m_pendingRequest->hwcImage = 0;
    HwcImageLoadRequest::mutex.unlock();
}


/* We want to track the item that does rotation changes explicitly. This isn't
   strictly needed, but it is easy to do and means we get away with tracking
   one single object as opposed to listening for changes in an entire parent
   hierarchy which would also be a lot slower.
  */
void HwcImage::setRotationHandler(QQuickItem *item)
{
    if (!hwcimage_is_enabled()) {
        qCDebug(LIPSTICK_LOG_HWC, "HwcImage ignoring rotation handler as HWC is disabled");
        return;
    }

    if (m_rotationHandler == item)
        return;
    if (m_rotationHandler)
        disconnect(m_rotationHandler, &QQuickItem::rotationChanged, this, &HwcImage::handlerRotationChanged);
    m_rotationHandler = item;
    if (m_rotationHandler)
        connect(m_rotationHandler, &QQuickItem::rotationChanged, this, &HwcImage::handlerRotationChanged);
    emit rotationHandlerChanged();
    polish();

    qCDebug(LIPSTICK_LOG_HWC) << "HwcImage" << this << "tracking rotation handler" << item;
}

void HwcImage::setAsynchronous(bool is)
{
    if (m_asynchronous == is)
        return;
    m_asynchronous = is;
    emit asynchronousChanged();
    polish();
}

void HwcImage::setSource(const QUrl &source)
{
    if (source == m_source)
        return;
    m_source = source;
    emit sourceChanged();
    m_status = Loading;
    emit statusChanged();
    polish();
}

void HwcImage::setOverlayColor(const QColor &color)
{
    if (m_overlayColor == color)
        return;
    m_overlayColor = color;
    emit overlayColorChanged();
    polish();
}

void HwcImage::setTextureSize(const QSize &size)
{
    if (m_textureSize == size)
        return;
    m_textureSize = size;
    emit textureSizeChanged();
    polish();
}

void HwcImage::setEffect(const QString &effect)
{
    if (m_effect == effect)
        return;
    m_effect = effect;
    emit effectChanged();
    polish();
}

void HwcImage::setPixelRatio(qreal ratio)
{
    if (m_pixelRatio == ratio)
        return;
    m_pixelRatio = ratio;
    emit pixelRatioChanged();
    polish();
}

qreal hwcimage_get_rotation(QQuickItem *item)
{
    qreal rotation = fmod(item->rotation(), 360.0);
    return rotation < 0 ? 360.0 + rotation : rotation;
}

void HwcImage::handlerRotationChanged()
{
    qreal rotation = hwcimage_get_rotation(m_rotationHandler);
    bool is90 = qFuzzyCompare(0.0, fmod(rotation, 90));
    qCDebug(LIPSTICK_LOG_HWC, " - rotation changed: %6.3f, 90 degree=%s", rotation, is90 ? "yes" : "no");
    if (is90 && m_textureRotation != rotation)
        polish();
}

void HwcImage::updatePolish()
{
    if (m_source.isEmpty()) {
        // Trigger deletion of the sg node and texture in case the source has been
        // removed.
        update();
        return;
    }
    m_status = Loading;
    emit statusChanged();

    m_image = QImage();
    HwcImageLoadRequest *req = new HwcImageLoadRequest();
    req->hwcImage = this;
    req->file = m_source.toLocalFile();
    req->textureSize = m_textureSize;
    req->effect = m_effect;
    req->overlay = m_overlayColor;
    req->pixelRatio = m_pixelRatio;
    req->rotation = m_rotationHandler ? hwcimage_get_rotation(m_rotationHandler) : 0;

    qCDebug(LIPSTICK_LOG_HWC,
            "Scheduling HwcImage request, source=%s, (%d x %d), eff=%s, olay=%s, rot=%f, pr=%f, %s",
            qPrintable(m_source.toString()),
            m_textureSize.width(), m_textureSize.height(),
            qPrintable(m_effect),
            qPrintable(m_overlayColor.name(QColor::HexArgb)),
            req->rotation,
            m_pixelRatio,
            (m_asynchronous ? "async" : "sync")
            );

    if (m_asynchronous) {
        HwcImageLoadRequest::mutex.lock();
        m_pendingRequest = req;
        QThreadPool::globalInstance()->start(m_pendingRequest);
        HwcImageLoadRequest::mutex.unlock();
    } else {
        req->execute();
        apply(req);
        delete req;
    }
}

void HwcImage::apply(HwcImageLoadRequest *req)
{
    m_image = req->image;
    QSize s = m_image.size();
    setSize(s);
    m_status = s.isValid() ? Ready : Error;
    emit statusChanged();
    m_textureRotation = req->rotation;
    update();
}

bool HwcImage::event(QEvent *e)
{
    if (e->type() == HWCIMAGE_LOAD_EVENT) {
        HwcImageLoadRequest *req = static_cast<HwcImageLoadRequest *>(e);

        bool accept = m_source.toLocalFile() == req->file
                      && m_effect == req->effect
                      && m_textureSize == req->textureSize
                      && m_pixelRatio == req->pixelRatio
                      && m_overlayColor == req->overlay;
        qCDebug(LIPSTICK_LOG_HWC,
                "HwcImage request completed: %s, source=%s, (%d x %d), eff=%s, olay=%s, rot=%f, pr=%f",
                (accept ? "accepted" : "rejected"),
                qPrintable(req->file),
                req->textureSize.width(), req->textureSize.height(),
                qPrintable(req->effect),
                qPrintable(req->overlay.name(QColor::HexArgb)),
                req->rotation,
                req->pixelRatio);
        if (accept)
            apply(req);

        return true;
    }
    return QQuickItem::event(e);
}

class HwcImageTexture : public QSGTexture
{
    Q_OBJECT

public:
    HwcImageTexture(EGLClientBuffer buffer, EGLImageKHR image, const QSize &size, HwcRenderStage *hwc);
    ~HwcImageTexture();
    void release();

    int textureId() const { return m_id; }
    QSize textureSize() const { return m_size; }
    bool hasAlphaChannel() const { return false; }
    bool hasMipmaps() const { return false; }
    void bind();

    static QSGTexture *create(const QImage &image, QQuickWindow *window);

    void *handle() const { return hwcimage_eglbuffer_to_handle(m_buffer); }

private:
    GLuint m_id;
    HwcRenderStage *m_hwc;
    EGLClientBuffer m_buffer;
    EGLImageKHR m_image;
    QSize m_size;
    void *m_handle;
    bool m_bound;
};

class HwcImageNode : public QSGSimpleTextureNode
{
public:
    HwcImageNode() {
        qCDebug(LIPSTICK_LOG_HWC) << "HwcImageNode is created...";
        qsgnode_set_description(this, QStringLiteral("hwc-image-node"));
    }
    ~HwcImageNode() {
        qCDebug(LIPSTICK_LOG_HWC) << "HwcImageNode is gone...";
        releaseTexture();
    }
    void *handle() const {
        HwcImageTexture *t = static_cast<HwcImageTexture *>(texture());
        return t ? t->handle() : 0;
    }
    void releaseTexture() {
        if (HwcImageTexture *t = qobject_cast<HwcImageTexture *>(texture()))
            t->release();
        else
            delete texture();
    }
};

HwcImageNode *HwcImage::updateActualPaintNode(QSGNode *old)
{
    if (m_image.isNull() || width() <= 0 || height() <= 0) {
        delete old;
        return 0;
    }

    HwcImageNode *tn = static_cast<HwcImageNode *>(old);
    if (!tn)
        tn = new HwcImageNode();
    if (!tn->texture() || !m_image.isNull()) {
        tn->releaseTexture();
        QSGTexture *t = HwcImageTexture::create(m_image, window());
        if (t)
            tn->setTexture(t);
        else
            tn->setTexture(window()->createTextureFromImage(m_image));
        m_image = QImage();
    }
    tn->setRect(0, 0, width(), height());
    return tn;
}

QMatrix4x4 HwcImage::reverseTransform() const
{
    if (!m_rotationHandler)
        return QMatrix4x4();
    // We assume a center-oriented rotation with no monkey busines between us
    // and the rotationHandler and the rotationHandler must be a parent of
    // ourselves. (It wouldn't rotate otherwise)
    float w2 = width() / 2;
    float h2 = height() / 2;
    QMatrix4x4 m;
    m.translate(w2, h2);
    m.rotate(-m_textureRotation, 0, 0, 1);
    m.translate(-w2, -h2);
    return m;
}

QSGNode *HwcImage::updatePaintNode(QSGNode *old, UpdatePaintNodeData *)
{
    if (!hwcimage_is_enabled()) {
        qCDebug(LIPSTICK_LOG_HWC) << "HwcImage" << this << "updating paint node without HWC support";
        return updateActualPaintNode(old);
    }

    /*
        When we're using hwcomposer, we replace the image with a slightly more
        complex subtree. There is an HwcNode which is where we put the buffer
        handle which the HwcRenderStage will pick up. Then we have a
        QSGTransformNode which we use to apply reverse transforms. For different
        orientations we create a rotated texture and invert the transform
        in the scene graph.

        HwcNode             <- The hook which makes us get picked up by the HwcRenderStage
           |
        QSGTransformNode    <- The reverse transform used to get correct orientation
           |
        HwcImageNode        <- The texture node which gets rendered as a fallback.
     */
    if (old) {
        qCDebug(LIPSTICK_LOG_HWC) << "HwcImage" << this << "updating paint existing node";
        HwcNode *hwcNode = static_cast<HwcNode *>(old);
        HwcImageNode *contentNode = updateActualPaintNode(hwcNode->firstChild()->firstChild());
        if (contentNode == 0) {
            delete hwcNode;
            return 0;
        } else if (contentNode != hwcNode->firstChild()->firstChild()) {
            // No need to remove the old node as the updatePaintNode call will
            // already have deleted the old content node and it would thus have
            // already been taken out.
            hwcNode->firstChild()->appendChildNode(contentNode);
        }
        static_cast<QSGTransformNode *>(hwcNode->firstChild())->setMatrix(reverseTransform());
        hwcNode->update(contentNode, contentNode->handle());
        return hwcNode;
    }

    HwcImageNode *contentNode = updateActualPaintNode(0);
    if (contentNode) {
        qCDebug(LIPSTICK_LOG_HWC) << "HwcImage" << this << "creating new node";
        HwcNode *hwcNode = new HwcNode();
        QSGTransformNode *xnode = new QSGTransformNode();
        xnode->setMatrix(reverseTransform());
        qsgnode_set_description(xnode, QStringLiteral("hwc-reverse-xform"));
        xnode->appendChildNode(contentNode);
        hwcNode->appendChildNode(xnode);
        hwcNode->update(contentNode, contentNode->handle());
        return hwcNode;
    }

    return 0;
}


// from hybris_nativebuffer.h in libhybris
#define HYBRIS_USAGE_SW_WRITE_RARELY    0x00000020
#define HYBRIS_USAGE_HW_TEXTURE         0x00000100
#define HYBRIS_USAGE_HW_COMPOSER        0x00000800
#define HYBRIS_PIXEL_FORMAT_RGBA_8888   1
#define HYBRIS_PIXEL_FORMAT_RGB_888     3
#define HYBRIS_PIXEL_FORMAT_BGRA_8888   5

#define EGL_NATIVE_BUFFER_HYBRIS        0x3140

extern "C" {
    typedef EGLBoolean (EGLAPIENTRYP _eglHybrisCreateNativeBuffer)(EGLint width, EGLint height, EGLint usage, EGLint format, EGLint *stride, EGLClientBuffer *buffer);
    typedef EGLBoolean (EGLAPIENTRYP _eglHybrisLockNativeBuffer)(EGLClientBuffer buffer, EGLint usage, EGLint l, EGLint t, EGLint w, EGLint h, void **vaddr);
    typedef EGLBoolean (EGLAPIENTRYP _eglHybrisUnlockNativeBuffer)(EGLClientBuffer buffer);
    typedef EGLBoolean (EGLAPIENTRYP _eglHybrisReleaseNativeBuffer)(EGLClientBuffer buffer);

    typedef void (EGLAPIENTRYP _glEGLImageTargetTexture2DOES)(GLenum target, EGLImageKHR image);
    typedef EGLImageKHR (EGLAPIENTRYP _eglCreateImageKHR)(EGLDisplay dpy, EGLContext ctx, EGLenum target, EGLClientBuffer buffer, const EGLint *attribs);
    typedef EGLBoolean (EGLAPIENTRYP _eglDestroyImageKHR)(EGLDisplay dpy, EGLImageKHR image);
}

_glEGLImageTargetTexture2DOES glEGLImageTargetTexture2DOES = 0;
_eglCreateImageKHR eglCreateImageKHR = 0;
_eglDestroyImageKHR eglDestroyImageKHR = 0;

_eglHybrisCreateNativeBuffer eglHybrisCreateNativeBuffer = 0;
_eglHybrisLockNativeBuffer eglHybrisLockNativeBuffer = 0;
_eglHybrisUnlockNativeBuffer eglHybrisUnlockNativeBuffer = 0;
_eglHybrisReleaseNativeBuffer eglHybrisReleaseNativeBuffer = 0;

static void hwcimage_initialize()
{
    static bool initialized = false;
    if (initialized)
        return;
    initialized = true;


    glEGLImageTargetTexture2DOES = (_glEGLImageTargetTexture2DOES) eglGetProcAddress("glEGLImageTargetTexture2DOES");
    eglCreateImageKHR = (_eglCreateImageKHR) eglGetProcAddress("eglCreateImageKHR");
    eglDestroyImageKHR = (_eglDestroyImageKHR) eglGetProcAddress("eglDestroyImageKHR");
    eglHybrisCreateNativeBuffer = (_eglHybrisCreateNativeBuffer) eglGetProcAddress("eglHybrisCreateNativeBuffer");
    eglHybrisLockNativeBuffer = (_eglHybrisLockNativeBuffer) eglGetProcAddress("eglHybrisLockNativeBuffer");
    eglHybrisUnlockNativeBuffer = (_eglHybrisUnlockNativeBuffer) eglGetProcAddress("eglHybrisUnlockNativeBuffer");
    eglHybrisReleaseNativeBuffer = (_eglHybrisReleaseNativeBuffer) eglGetProcAddress("eglHybrisReleaseNativeBuffer");
}

static bool hwcimage_is_enabled()
{
    // Check for availablility of EGL_HYBRIS_native_buffer support, which we
    // need to do hwc layering of background images...
    static int hybrisBuffers = -1;
    if (hybrisBuffers < 0) {
        if (strstr(eglQueryString(eglGetDisplay(EGL_DEFAULT_DISPLAY), EGL_EXTENSIONS), "EGL_HYBRIS_native_buffer") == 0)
            hybrisBuffers = 0;
        else
            hybrisBuffers = 1;
    }
    return hybrisBuffers == 1 && HwcRenderStage::isHwcEnabled();
}

QSGTexture *HwcImageTexture::create(const QImage &image, QQuickWindow *window)
{
    hwcimage_initialize();

    if (!hwcimage_is_enabled())
        return 0;

    EGLClientBuffer buffer = 0;
    int width = image.width();
    int height = image.height();
    int usage = HYBRIS_USAGE_SW_WRITE_RARELY | HYBRIS_USAGE_HW_TEXTURE | HYBRIS_USAGE_HW_COMPOSER;
    int stride = 0;
    int format = HYBRIS_PIXEL_FORMAT_RGBA_8888;
    char *data;

    eglHybrisCreateNativeBuffer(width, height, usage, format, &stride, &buffer);
    Q_ASSERT(buffer);
    eglHybrisLockNativeBuffer(buffer, HYBRIS_USAGE_SW_WRITE_RARELY, 0, 0, width, height, (void **) &data);
    Q_ASSERT(data);

    const int dbpl = stride * 4;
    const int bpl = qMin(dbpl, image.bytesPerLine());
    for (int y=0; y<height; ++y)
        memcpy(data + y * dbpl, image.constScanLine(y), bpl);

    eglHybrisUnlockNativeBuffer(buffer);

    EGLImageKHR eglImage = eglCreateImageKHR(eglGetDisplay(EGL_DEFAULT_DISPLAY),
                                             EGL_NO_CONTEXT,
                                             EGL_NATIVE_BUFFER_HYBRIS,
                                             buffer,
                                             0);
    Q_ASSERT(eglImage);
    Q_ASSERT(QQuickWindowPrivate::get(window));
    Q_ASSERT(QQuickWindowPrivate::get(window)->customRenderStage);

    return new HwcImageTexture(buffer,
                               eglImage,
                               QSize(width, height),
                               static_cast<HwcRenderStage *>(QQuickWindowPrivate::get(window)->customRenderStage));
}


HwcImageTexture::HwcImageTexture(EGLClientBuffer buffer, EGLImageKHR image, const QSize &size, HwcRenderStage *hwc)
    : m_hwc(hwc)
    , m_buffer(buffer)
    , m_image(image)
    , m_size(size)
    , m_bound(false)
{
    Q_ASSERT(m_hwc);
    glGenTextures(1, &m_id);
    qCDebug(LIPSTICK_LOG_HWC,
            "HwcImageTexture(%p) created, size=(%d x %d), texId=%d, buffer=%p, image=%p",
            this, m_size.width(), m_size.height(), m_id, m_buffer, m_image);
}

HwcImageTexture::~HwcImageTexture()
{
    Q_ASSERT(m_id == 0); // Should have been deleted via release().

    eglDestroyImageKHR(eglGetDisplay(EGL_DEFAULT_DISPLAY), m_image);
    eglHybrisReleaseNativeBuffer(m_buffer);
    qCDebug(LIPSTICK_LOG_HWC,
            "HwcImageTexture(%p) destroyed, size=(%d x %d), texId=%d, buffer=%p, image=%p",
            this, m_size.width(), m_size.height(), m_id, m_buffer, m_image);
}

void HwcImageTexture::bind()
{
    updateBindOptions();
    glBindTexture(GL_TEXTURE_2D, m_id);
    if (!m_bound) {
        m_bound = true;
        glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, m_image);
    }
}

void HwcImageTexture::release()
{
    qCDebug(LIPSTICK_LOG_HWC,
            "HwcImageTexture(%p) released, size=(%d x %d), texId=%d, buffer=%p, image=%p",
            this, m_size.width(), m_size.height(), m_id, m_buffer, m_image);

    // We're on the render thread, so delete the texture right away
    Q_ASSERT(m_id);
    glDeleteTextures(1, &m_id);
    m_id = 0;

    // Then tell the hwc render stage to clean it up when possible.
    // Chances are the cleanup will happen on the GUI thread, but this
    // is safe as the EGL resource can be deleted from any thread..
    m_hwc->deleteOnBufferRelease(handle(), this);
}


// Memory layout of ANativeWindowBuffer from Android's /system/window.h which
// is the structure that libhybris uses internally to represent the actual
// EGLClientBuffer. We need this to extract the gralloc handle.
struct android_native_base_t {
    int magic;
    int version;
    void *reserved[4];
    void (*incRef)(struct android_native_base_t* base);
    void (*decRef)(struct android_native_base_t* base);
};
struct ANativeWindowBuffer {
    android_native_base_t common;
    int width;
    int height;
    int stride;
    int format;
    int usage;
    void *reserved[2];
    void *handle;
    void *reserved_proc[8];
};

void *hwcimage_eglbuffer_to_handle(EGLClientBuffer buffer)
{
    return ((ANativeWindowBuffer *) buffer)->handle;
}

#include "hwcimage.moc"

