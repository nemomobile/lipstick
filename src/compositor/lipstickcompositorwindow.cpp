/***************************************************************************
**
** Copyright (C) 2013 Jolla Ltd.
** Contact: Aaron Kennedy <aaron.kennedy@jollamobile.com>
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

#include <QCoreApplication>
#include <QWaylandCompositor>
#include <QWaylandInputDevice>
#include <QTimer>
#include <sys/types.h>
#include <signal.h>
#include "lipstickcompositor.h"
#include "lipstickcompositorwindow.h"


#include "hwcrenderstage.h"
#include <EGL/egl.h>
#include <private/qwlsurface_p.h>
#include <private/qquickwindow_p.h>

LipstickCompositorWindow::LipstickCompositorWindow(int windowId, const QString &category,
                                                   QWaylandQuickSurface *surface, QQuickItem *parent)
: QWaylandSurfaceItem(surface, parent), m_windowId(windowId), m_category(category), m_ref(0),
  m_delayRemove(false), m_windowClosed(false), m_removePosted(false), m_mouseRegionValid(false),
  m_interceptingTouch(false), m_mapped(false)
{
    setFlags(QQuickItem::ItemIsFocusScope | flags());
    refreshMouseRegion();

    // Handle ungrab situations
    connect(this, SIGNAL(visibleChanged()), SLOT(handleTouchCancel()));
    connect(this, SIGNAL(enabledChanged()), SLOT(handleTouchCancel()));
    connect(this, SIGNAL(touchEventsEnabledChanged()), SLOT(handleTouchCancel()));
    connect(this, &QWaylandSurfaceItem::surfaceDestroyed, this, &QObject::deleteLater);

    connectSurfaceSignals();
}

LipstickCompositorWindow::~LipstickCompositorWindow()
{
    // We don't want tryRemove() posting an event anymore, we're dying anyway
    m_removePosted = true;
    LipstickCompositor::instance()->windowDestroyed(this);
}

QVariant LipstickCompositorWindow::userData() const
{
    return m_data;
}

void LipstickCompositorWindow::setUserData(QVariant data)
{
    if (m_data == data)
        return;

    m_data = data;
    emit userDataChanged();
}

int LipstickCompositorWindow::windowId() const
{
    return m_windowId;
}

qint64 LipstickCompositorWindow::processId() const
{
    if (surface())
        return surface()->processId();
    else return 0;
}

bool LipstickCompositorWindow::delayRemove() const
{
    return m_delayRemove;
}

void LipstickCompositorWindow::setDelayRemove(bool delay)
{
    if (delay == m_delayRemove)
        return;

    m_delayRemove = delay;
    emit delayRemoveChanged();

    tryRemove();
}

QString LipstickCompositorWindow::category() const
{
    return m_category;
}

QString LipstickCompositorWindow::title() const
{
    if (surface())
        return surface()->title();
    return QString();
}

void LipstickCompositorWindow::imageAddref()
{
    ++m_ref;
}

void LipstickCompositorWindow::imageRelease()
{
    Q_ASSERT(m_ref);
    --m_ref;
    tryRemove();
}

bool LipstickCompositorWindow::canRemove() const
{
    return m_windowClosed && !m_delayRemove && m_ref == 0;
}

void LipstickCompositorWindow::tryRemove()
{
    if (canRemove() && !m_removePosted) {
        m_removePosted = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::User));
    }
}

QRect LipstickCompositorWindow::mouseRegionBounds() const
{
    if (m_mouseRegionValid)
        return m_mouseRegion.boundingRect();
    else
        return QRect(0, 0, width(), height());
}

void LipstickCompositorWindow::refreshMouseRegion()
{
    QWaylandSurface *s = surface();
    if (s) {
        QVariantMap properties = s->windowProperties();
        if (properties.contains(QLatin1String("MOUSE_REGION"))) {
            m_mouseRegion = s->windowProperties().value("MOUSE_REGION").value<QRegion>();
            m_mouseRegionValid = true;
            if (LipstickCompositor::instance()->debug())
                qDebug() << "Window" << windowId() << "mouse region set:" << m_mouseRegion;
        } else {
            m_mouseRegionValid = false;
            if (LipstickCompositor::instance()->debug())
                qDebug() << "Window" << windowId() << "mouse region cleared";
        }

        emit mouseRegionBoundsChanged();
    }
}

void LipstickCompositorWindow::refreshGrabbedKeys()
{
    QWaylandSurface *s = surface();
    if (s) {
        const QStringList grabbedKeys = s->windowProperties().value(
                    QLatin1String("GRABBED_KEYS")).value<QStringList>();

        if (m_grabbedKeys.isEmpty() && !grabbedKeys.isEmpty()) {
            qApp->installEventFilter(this);
        } else if (!m_grabbedKeys.isEmpty() && grabbedKeys.isEmpty()) {
            qApp->removeEventFilter(this);
        }

        m_grabbedKeys.clear();
        foreach (const QString &key, grabbedKeys)
            m_grabbedKeys.append(key.toInt());

        if (LipstickCompositor::instance()->debug())
            qDebug() << "Window" << windowId() << "grabbed keys changed:" << grabbedKeys;
    }
}

bool LipstickCompositorWindow::eventFilter(QObject *obj, QEvent *event)
{
#if QT_VERSION >= 0x050202
    if (obj == window() && m_interceptingTouch) {
        switch (event->type()) {
        case QEvent::TouchUpdate: {
            QTouchEvent *te = static_cast<QTouchEvent *>(event);
            // If we get press/release, don't intercept the event, but send it through QQuickWindow.
            // These are sent through to QQuickWindow so that the integrity of the touch
            // handling is maintained.
            if (te->touchPointStates() & (Qt::TouchPointPressed | Qt::TouchPointReleased))
                return false;
            handleTouchEvent(static_cast<QTouchEvent *>(event));
            return true;
        }
        case QEvent::TouchEnd: // Intentional fall through...
        case QEvent::TouchCancel:
            obj->removeEventFilter(this);
            m_interceptingTouch = false;
        default:
            break;
        }
        return false;
    }
#else
    Q_UNUSED(obj);
#endif
    if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        QWaylandSurface *m_surface = surface();
        if (m_surface && m_grabbedKeys.contains(ke->key())) {
            QWaylandInputDevice *inputDevice = m_surface->compositor()->defaultInputDevice();
            inputDevice->sendFullKeyEvent(m_surface, ke);

            return true;
        }
    }
    return false;
}

bool LipstickCompositorWindow::isInProcess() const
{
    return false;
}

void LipstickCompositorWindow::itemChange(ItemChange change, const ItemChangeData &data)
{
    if (change == ItemSceneChange) {
        handleTouchCancel();
    }
    QWaylandSurfaceItem::itemChange(change, data);
}

bool LipstickCompositorWindow::event(QEvent *e)
{
    bool rv = QWaylandSurfaceItem::event(e);
    if (e->type() == QEvent::User) {
        m_removePosted = false;
        if (canRemove()) delete this;
    }
    return rv;
}

void LipstickCompositorWindow::mousePressEvent(QMouseEvent *event)
{
    QWaylandSurface *m_surface = surface();
    if (m_surface && (!m_mouseRegionValid || m_mouseRegion.contains(event->pos()))) {
        QWaylandInputDevice *inputDevice = m_surface->compositor()->defaultInputDevice();
        if (inputDevice->mouseFocus() != this)
            inputDevice->setMouseFocus(this, event->pos(), event->globalPos());
        inputDevice->sendMousePressEvent(event->button(), event->pos(), event->globalPos());
    } else {
        event->ignore();
    }
}

void LipstickCompositorWindow::mouseMoveEvent(QMouseEvent *event)
{
    QWaylandSurface *m_surface = surface();
    if (m_surface){
        QWaylandInputDevice *inputDevice = m_surface->compositor()->defaultInputDevice();
        inputDevice->sendMouseMoveEvent(this, event->pos(), event->globalPos());
    } else {
        event->ignore();
    }
}

void LipstickCompositorWindow::mouseReleaseEvent(QMouseEvent *event)
{
    QWaylandSurface *m_surface = surface();
    if (m_surface){
        QWaylandInputDevice *inputDevice = m_surface->compositor()->defaultInputDevice();
        inputDevice->sendMouseReleaseEvent(event->button(), event->pos(), event->globalPos());
    } else {
        event->ignore();
    }
}

void LipstickCompositorWindow::wheelEvent(QWheelEvent *event)
{
    QWaylandSurface *m_surface = surface();
    if (m_surface) {
        QWaylandInputDevice *inputDevice = m_surface->compositor()->defaultInputDevice();
        inputDevice->sendMouseWheelEvent(event->orientation(), event->delta());
    } else {
        event->ignore();
    }
}

void LipstickCompositorWindow::touchEvent(QTouchEvent *event)
{
    if (touchEventsEnabled() && surface()) {
        handleTouchEvent(event);

#if QT_VERSION >= 0x050202
        static bool lipstick_touch_interception = qEnvironmentVariableIsEmpty("LIPSTICK_NO_TOUCH_INTERCEPTION");
        if (lipstick_touch_interception && event->type() == QEvent::TouchBegin) {
            // On TouchBegin, start intercepting
            if (event->isAccepted() && !m_interceptingTouch) {
                m_interceptingTouch = true;
                window()->installEventFilter(this);
            }
        }
#endif
    } else {
        event->ignore();
    }
}

void LipstickCompositorWindow::handleTouchEvent(QTouchEvent *event)
{
    QList<QTouchEvent::TouchPoint> points = event->touchPoints();

    if (m_mouseRegionValid && event->touchPointStates() & Qt::TouchPointPressed) {
        foreach (const QTouchEvent::TouchPoint &p, points) {
            if (!m_mouseRegion.contains(p.pos().toPoint())) {
                event->ignore();
                return;
            }
        }
    }

    QWaylandSurface *m_surface = surface();
    if (!m_surface) {
        event->ignore();
        return;
    }
    QWaylandInputDevice *inputDevice = m_surface->compositor()->defaultInputDevice();
    event->accept();
    if (inputDevice->mouseFocus() != this) {
        QPoint pointPos;
        if (!points.isEmpty())
            pointPos = points.at(0).pos().toPoint();
        inputDevice->setMouseFocus(this, pointPos, pointPos);
    }
    inputDevice->sendFullTouchEvent(event);
}

void LipstickCompositorWindow::handleTouchCancel()
{
    QWaylandSurface *m_surface = surface();
    if (!m_surface)
        return;
    QWaylandInputDevice *inputDevice = m_surface->compositor()->defaultInputDevice();
    if (inputDevice->mouseFocus() == this &&
            (!isVisible() || !isEnabled() || !touchEventsEnabled())) {
        inputDevice->sendTouchCancelEvent();
        inputDevice->setMouseFocus(0, QPointF());
    }
    if (QWindow *w = window())
        w->removeEventFilter(this);
    m_interceptingTouch = false;
}

void LipstickCompositorWindow::terminateProcess(int killTimeout)
{
    kill(processId(), SIGTERM);

    QTimer::singleShot(killTimeout, this, SLOT(killProcess()));
}

void LipstickCompositorWindow::killProcess()
{
    kill(processId(), SIGKILL);
}

void LipstickCompositorWindow::connectSurfaceSignals()
{
    foreach (const QMetaObject::Connection &connection, m_surfaceConnections) {
        disconnect(connection);
    }

    m_surfaceConnections.clear();
    if (surface()) {
        m_surfaceConnections << connect(surface(), SIGNAL(titleChanged()), SIGNAL(titleChanged()));
        m_surfaceConnections << connect(surface(), &QWaylandSurface::configure, this, &LipstickCompositorWindow::committed);
    }
}

static bool hwc_windowsurface_is_enabled();

typedef EGLBoolean (EGLAPIENTRYP Ptr_eglHybrisAquireNativeBufferWL)(EGLDisplay dpy, struct wl_resource *wlBuffer, EGLClientBuffer *buffer);
typedef EGLBoolean (EGLAPIENTRYP Ptr_eglHybrisNativeBufferHandle)(EGLDisplay dpy, EGLClientBuffer buffer, void **handle);
typedef EGLBoolean (EGLAPIENTRYP Ptr_eglHybrisReleaseNativeBuffer)(EGLClientBuffer buffer);
static Ptr_eglHybrisAquireNativeBufferWL eglHybrisAquireNativeBufferWL;
static Ptr_eglHybrisNativeBufferHandle eglHybrisNativeBufferHandle;
static Ptr_eglHybrisReleaseNativeBuffer eglHybrisReleaseNativeBuffer;

class LipstickCompositorWindowHwcNode : public HwcNode
{
public:
    LipstickCompositorWindowHwcNode(QQuickWindow *window) : HwcNode(window), eglBuffer(0) { }
    ~LipstickCompositorWindowHwcNode();
    EGLClientBuffer eglBuffer;
};

QSGNode *LipstickCompositorWindow::updatePaintNode(QSGNode *old, UpdatePaintNodeData *data)
{
    if (!hwc_windowsurface_is_enabled())
        return QWaylandSurfaceItem::updatePaintNode(old, data);

    if (old) {
       LipstickCompositorWindowHwcNode *hwcNode = static_cast<LipstickCompositorWindowHwcNode *>(old);
        QSGNode *contentNode = QWaylandSurfaceItem::updatePaintNode(hwcNode->firstChild(), data);
        if (contentNode == 0) {
            delete hwcNode;
            return 0;
        } else if (contentNode != hwcNode->firstChild()) {
            // No need to remove the old node as the updatePaintNode call will
            // already have deleted the old content node and it would thus have
            // already been taken out.
            hwcNode->appendChildNode(contentNode);
        }
        updateNode(hwcNode, contentNode);
        return hwcNode;
    }

    QSGNode *contentNode = QWaylandSurfaceItem::updatePaintNode(0, data);
    if (contentNode) {
        LipstickCompositorWindowHwcNode *hwcNode = new LipstickCompositorWindowHwcNode(window());
        hwcNode->appendChildNode(contentNode);
        updateNode(hwcNode, contentNode);
        return hwcNode;
    }

    return 0;
}

static bool hwc_windowsurface_is_enabled()
{
    if (!HwcRenderStage::isHwcEnabled())
        return false;

    static int checked = 0;
    if (!checked) {
        const char *aquireNativeBufferExtension = "EGL_HYBRIS_WL_aquire_native_buffer";
        const char *nativeBuffer2Extensions = "EGL_HYBRIS_native_buffer2";
        const char *extensions = eglQueryString(eglGetCurrentDisplay(), EGL_EXTENSIONS);

        if (strstr(extensions, aquireNativeBufferExtension) != 0) {
            eglHybrisAquireNativeBufferWL = (Ptr_eglHybrisAquireNativeBufferWL) eglGetProcAddress("eglHybrisAquireNativeBufferWL");
            checked |= 0x1;
        } else {
            qCDebug(LIPSTICK_LOG_HWC, "Missing required EGL extension: '%s'", aquireNativeBufferExtension);
        }
        if (strstr(extensions, nativeBuffer2Extensions) != 0) {
            eglHybrisNativeBufferHandle = (Ptr_eglHybrisNativeBufferHandle) eglGetProcAddress("eglHybrisNativeBufferHandle");
            eglHybrisReleaseNativeBuffer = (Ptr_eglHybrisReleaseNativeBuffer) eglGetProcAddress("eglHybrisReleaseNativeBuffer");
            checked |= 0x2;
        } else {
            qCDebug(LIPSTICK_LOG_HWC, "Missing required EGL extension: '%s'", nativeBuffer2Extensions);
        }

        // If both extensions were found
        if (checked == (0x1 | 0x2)) {
            qCDebug(LIPSTICK_LOG_HWC, "HWC composition of window surfaces is enabled");
        } else {
            qCDebug(LIPSTICK_LOG_HWC, "HWC composition of window surfaces is disabled");
        }

    }
    return checked > 0;
}

void hwc_windowsurface_release_native_buffer(void *, void *callbackData)
{
    eglHybrisReleaseNativeBuffer((EGLClientBuffer) callbackData);
}

void LipstickCompositorWindow::updateNode(LipstickCompositorWindowHwcNode *hwcNode, QSGNode *contentNode)
{
    struct QWlSurface_Accessor : public QtWayland::Surface {
        wl_resource *surfaceBufferHandle() const { return m_buffer ? m_buffer->waylandBufferHandle() : 0; }
    };
    Q_ASSERT(surface());
    Q_ASSERT(surface()->handle());
    QWlSurface_Accessor *s = static_cast<QWlSurface_Accessor *>(surface()->handle());

    Q_ASSERT(s->mapped() || s->surfaceBufferHandle());
    // Follow the unmaplock logic inside QtWayland. If a surface is still
    // visible but has a null buffer attached, we retain the copy we have
    // until further notice...
    if (!s->surfaceBufferHandle() && s->mapped()) {
        qCDebug(LIPSTICK_LOG_HWC, " - visible with attached 'null' buffer, retaining previous buffer, EGLClientBuffer=%p, gralloc=%p", hwcNode->eglBuffer, hwcNode->handle());
        return;
    }

    EGLDisplay display = eglGetCurrentDisplay();
    EGLClientBuffer eglBuffer = 0;
    void *handle = 0;
    eglHybrisAquireNativeBufferWL(display, s->surfaceBufferHandle(), &eglBuffer);
    Q_ASSERT(eglBuffer);
    eglHybrisNativeBufferHandle(eglGetCurrentDisplay(), eglBuffer, &handle);
    Q_ASSERT(handle);

    // If we're taking a new buffer into use when there already was
    // one, set up the old to be removed.
    if (hwcNode->handle() && hwcNode->handle() != handle) {
        qDebug(LIPSTICK_LOG_HWC, " - releasing old buffer, EGLClientBuffer=%p, gralloc=%p", hwcNode->eglBuffer, hwcNode->handle());
        Q_ASSERT(hwcNode->eglBuffer);
        hwcNode->renderStage()->signalOnBufferRelease(hwc_windowsurface_release_native_buffer, hwcNode->handle(), hwcNode->eglBuffer);
    }
    qDebug(LIPSTICK_LOG_HWC, " - setting buffers on HwcNode, EGLClientBuffer=%p, gralloc=%p", eglBuffer, handle);
    hwcNode->eglBuffer = eglBuffer;

    Q_ASSERT(contentNode->type() == QSGNode::GeometryNodeType);
    hwcNode->update(static_cast<QSGGeometryNode *>(contentNode), handle);
}



LipstickCompositorWindowHwcNode::~LipstickCompositorWindowHwcNode()
{
    Q_ASSERT(handle());
    Q_ASSERT(eglBuffer);
    renderStage()->signalOnBufferRelease(hwc_windowsurface_release_native_buffer, handle(), eglBuffer);
}

