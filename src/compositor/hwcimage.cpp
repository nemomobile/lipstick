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

#include <QQuickWindow>
#include <QSGSimpleTextureNode>

#include <EGL/egl.h>
#include <EGL/eglext.h>

static void *hwcimage_eglbuffer_to_handle(EGLClientBuffer buffer);

HwcImage::HwcImage()
    : m_status(Null)
    , m_darkness(0)
{
    setFlag(ItemHasContents, true);
}

HwcImage::~HwcImage()
{

}

void HwcImage::setSource(const QUrl &source)
{
	if (source == m_source)
		return;
	m_source = source;
    emit sourceChanged();
    polish();
}

void HwcImage::setDarkness(qreal darkness)
{
    qreal d = qBound<qreal>(0, darkness, 1);
    if (m_darkness == d)
        return;
    m_darkness = d;
    emit darknessChanged();
    polish();
}


void HwcImage::updatePolish()
{
    if (m_source.isEmpty())
        return;

    m_status = Loading;
    emit statusChanged();

    QString file = m_source.toLocalFile();
    m_image = QImage(file).convertToFormat(QImage::Format_RGBX8888);
    setWidth(m_image.width());
    setHeight(m_image.height());
    if (m_image.size().isValid()) {
        m_status = Ready;
        QPainter p(&m_image);
        p.fillRect(m_image.rect(), QColor::fromRgbF(0, 0, 0, m_darkness));
    } else {
        m_status = Error;
    }
    emit statusChanged();
    update();
}



class HwcImageTexture : public QSGTexture
{
public:
    HwcImageTexture(EGLClientBuffer buffer, EGLImageKHR image, const QSize &size);
    ~HwcImageTexture();

    int textureId() const { return m_id; }
    QSize textureSize() const { return m_size; }
    bool hasAlphaChannel() const { return false; }
    bool hasMipmaps() const { return false; }
    void bind();

    static QSGTexture *create(const QImage &image);

    void *handle() const { return hwcimage_eglbuffer_to_handle(m_buffer); }

private:
    GLuint m_id;
    EGLClientBuffer m_buffer;
    EGLImageKHR m_image;
    QSize m_size;
    void *m_handle;
    bool m_bound;
};


class HwcImageNode : public QSGSimpleTextureNode
{
public:
    HwcImageNode() { qsgnode_set_description(this, "hwc-image-node"); }
    ~HwcImageNode() { delete texture(); }
    void *handle() const {
        HwcImageTexture *t = static_cast<HwcImageTexture *>(texture());
        qDebug() << "asking for handle from" << this << t << (t ? t->handle() : 0);
        return t ? t->handle() : 0;
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
        if (tn->texture())
            delete tn->texture();
        QSGTexture *t = HwcImageTexture::create(m_image);
        if (t)
            tn->setTexture(t);
        else
            tn->setTexture(window()->createTextureFromImage(m_image));
        m_image = QImage();
    }
    tn->setRect(0, 0, width(), height());
    return tn;
}

QSGNode *HwcImage::updatePaintNode(QSGNode *old, UpdatePaintNodeData *)
{
    // Check for availablility of EGL_HYBRIS_native_buffer support, which we
    // need to do hwc layering of background images...
    static int hybrisBuffers = -1;
    if (hybrisBuffers < 0) {
        if (strstr(eglQueryString(eglGetCurrentDisplay(), EGL_EXTENSIONS), "EGL_HYBRIS_native_buffer") == 0)
            hybrisBuffers = 0;
        else
            hybrisBuffers = 1;
    }

    if (!HwcRenderStage::isHwcEnabled())
        return updateActualPaintNode(old);

    if (old) {
        HwcNode *hwcNode = static_cast<HwcNode *>(old);
        HwcImageNode *contentNode = updateActualPaintNode(hwcNode->firstChild());
        if (contentNode == 0) {
            delete hwcNode;
            return 0;
        } else if (contentNode != hwcNode->firstChild()) {
            // No need to remove the old node as the updatePaintNode call will
            // already have deleted the old content node and it would thus have
            // already been taken out.
            hwcNode->appendChildNode(contentNode);
        }
        hwcNode->update(contentNode->handle());
        return hwcNode;
    }

    HwcImageNode *contentNode = updateActualPaintNode(0);
    if (contentNode) {
        HwcNode *hwcNode = new HwcNode();
        hwcNode->appendChildNode(contentNode);
        hwcNode->update(contentNode->handle());
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

#define EGL_NATIVE_BUFFER_HYBRIS             0x3140

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

QSGTexture *HwcImageTexture::create(const QImage &image)
{
    hwcimage_initialize();

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

    return new HwcImageTexture(buffer, eglImage, QSize(width, height));

}


HwcImageTexture::HwcImageTexture(EGLClientBuffer buffer, EGLImageKHR image, const QSize &size)
    : m_buffer(buffer)
    , m_image(image)
    , m_size(size)
    , m_bound(false)
{
    glGenTextures(1, &m_id);
}

HwcImageTexture::~HwcImageTexture()
{
    glDeleteTextures(1, &m_id);
    eglDestroyImageKHR(eglGetDisplay(EGL_DEFAULT_DISPLAY), m_image);
    eglHybrisReleaseNativeBuffer(m_buffer);
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

