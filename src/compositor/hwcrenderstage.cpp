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

#include "hwcrenderstage.h"
#include "hwcinterface.h"

#include "lipstickcompositor.h"

#include <private/qguiapplication_p.h>
#include <qpa/qplatformintegration.h>
#include <qpa/qplatformnativeinterface.h>

#include <qsgnode.h>
#include <qsgtexturematerial.h>

#include <EGL/egl.h>

Q_LOGGING_CATEGORY(LIPSTICK_LOG_HWC, "qt.lipstick.hwc")

const QEvent::Type HWC_BufferRelease = (QEvent::Type) (QEvent::User + 1);

class HwcBufferReleaseEvent : public QEvent
{
public:
    HwcBufferReleaseEvent(void *b) : QEvent(HWC_BufferRelease), buffer(b) { }
    void *buffer;
};

// Any number above QSGNode::RenderNode will strictly do..
#define QSG_HWC_NODE_TYPE ((QSGNode::NodeType) 1000)

HwcNode::HwcNode(QQuickWindow *window)
    : QSGNode(QSG_HWC_NODE_TYPE)
    , m_contentNode(0)
    , m_buffer_handle(0)
    , m_x(0)
    , m_y(0)
    , m_blocked(false)
{
    qsgnode_set_description(this, QStringLiteral("hwcnode"));
    m_renderStage = (HwcRenderStage *) QQuickWindowPrivate::get(window)->customRenderStage;
    Q_ASSERT(m_renderStage);
}

HwcNode::~HwcNode()
{
    m_renderStage->hwcNodeDeleted(this);
}

void HwcNode::update(QSGGeometryNode *node, void *handle)
{
    m_contentNode = node;
    m_buffer_handle = handle;
}

void HwcNode::setBlocked(bool blocked)
{
    if (m_blocked != blocked) {
        m_blocked = blocked;
        markDirty(DirtySubtreeBlocked);
    }
}

QRect HwcNode::bounds() const
{
    Q_ASSERT(m_contentNode);
    QSGGeometry *g = m_contentNode->geometry();
    QSGGeometry::TexturedPoint2D *v = g->vertexDataAsTexturedPoint2D();
    float x1 = v[0].x;        // v0--v2
    float y1 = v[0].y;        // |  / |
    float x2 = v[2].x;        // | /  |
    float y2 = v[1].y;        // v1--v3
    return QRect(x1, y1, x2-x1, y2-y1);
}

#ifndef QT_NO_DEBUG
static inline bool hwc_renderstage_isRect(QSGGeometry::TexturedPoint2D *v)
{
    return v[0].x == v[1].x && v[2].x == v[3].x
           && v[0].y == v[2].y && v[1].y == v[3].y
           && v[0].tx == v[1].tx && v[2].tx == v[3].tx
           && v[0].ty == v[2].ty && v[1].ty == v[3].ty;
}

static inline void hwc_renderstage_check_node(QSGNode *node)
{
    static const QSGMaterialType *opaqueTextureType = QSGOpaqueTextureMaterial().type();
    static const QSGMaterialType *alphaTextureType = QSGTextureMaterial().type();
    static const QSGGeometry::Attribute *attributes = QSGGeometry::defaultAttributes_TexturedPoint2D().attributes;

    Q_ASSERT(node->type() == QSG_HWC_NODE_TYPE);
    QSGGeometryNode *gn = static_cast<HwcNode *>(node)->contentNode();
    Q_ASSERT(gn);
    QSGGeometry *g = gn->geometry();
    Q_ASSERT(g->vertexCount() == 4);
    Q_ASSERT(g->indexCount() == 0);
    Q_ASSERT(g->attributes() == attributes);
    Q_ASSERT(hwc_renderstage_isRect(g->vertexDataAsTexturedPoint2D()));
    Q_ASSERT(gn->opaqueMaterial() && gn->opaqueMaterial()->type() == opaqueTextureType);
    Q_ASSERT(gn->material()->type() == alphaTextureType);
}
#else
#define hwc_renderstage_check_node(node)
#endif

bool HwcRenderStage::m_hwcEnabled = false;

void HwcRenderStage::initialize(LipstickCompositor *lipstick)
{
    static bool enabled = qgetenv("LIPSTICK_HARDWARE_COMPOSITOR").toInt() != 0;
    QPlatformNativeInterface *iface = QGuiApplicationPrivate::platform_integration->nativeInterface();
    if (!enabled) {
        qDebug() << "Hardware Compositor support is disabled";
        return;
    }
    Q_ASSERT(iface);
    EGLDisplay eglDisplay = iface->nativeResourceForIntegration("egldisplay");
    Q_ASSERT(eglDisplay);

    qCDebug(LIPSTICK_LOG_HWC, "EGL Extensions: %s", eglQueryString(eglDisplay, EGL_EXTENSIONS));

    void *compositor = iface->nativeResourceForIntegration(HWC_INTERFACE_STRING);
    if (!compositor) {
        qDebug("Hardware Compositor is not enabled, missing native resource named: '%s'", HWC_INTERFACE_STRING);
        return;
    }
    Q_ASSERT(compositor);
    QQuickWindowPrivate::get(lipstick)->customRenderStage = new HwcRenderStage(lipstick, compositor);
    qDebug() << "Hardware Compositor support is enabled";
    m_hwcEnabled = true;
}

static HwcInterface::LayerList *hwc_renderstage_create_list(const QVector<HwcNode *> &nodes)
{
    int size = sizeof(HwcInterface::LayerList) + sizeof(HwcInterface::Layer) * nodes.size();
    HwcInterface::LayerList *list = (HwcInterface::LayerList *) malloc(size);
    memset(list, 0, size);
    list->layerCount = nodes.size();
    for (int i=0; i<nodes.size(); ++i) {
        HwcInterface::Layer &l = list->layers[i];
        HwcNode *n = nodes.at(i);
        QRect r = n->bounds();
        l.sx = 0;
        l.sy = 0;
        l.tx = r.x() + n->x();
        l.ty = r.y() + n->y();
        l.sw = l.tw = r.width();
        l.sh = l.th = r.height();
        l.handle = n->handle();
    }
    return list;
}

static void hwc_renderstage_buffer_available(void *handle, void *hwc)
{
    // ignore for now.. We're only using this for static bg images atm..
    static_cast<HwcRenderStage *>(hwc)->bufferReleased(handle);
}

static void hwc_renderstage_delete_list(HwcInterface::LayerList *list)
{
    free(list);
}

static void hwc_renderstage_invalidate(void *hwc)
{
    static_cast<HwcRenderStage *>(hwc)->invalidated();
}

static void hwc_renderstage_dump_layerlist(HwcInterface::LayerList *list)
{
    qCDebug(LIPSTICK_LOG_HWC, " - eglRenderingEnabled: %d", list->eglRenderingEnabled);
    for (int i=0; i<list->layerCount; ++i) {
        const HwcInterface::Layer &l = list->layers[i];
        qCDebug(LIPSTICK_LOG_HWC, " - layer %d: target=(%d,%d, %dx%d), source=(%d,%d, %dx%d) handle=%p, accepted=%d",
                i,
                l.tx, l.ty, l.tw, l.th,
                l.sx, l.sy, l.sw, l.sh,
                l.handle, l.accepted);
    }
}


HwcRenderStage::HwcRenderStage(LipstickCompositor *lipstick, void *compositorHandle)
    : m_lipstick(lipstick)
    , m_window(lipstick)
    , m_hwc(reinterpret_cast<HwcInterface::Compositor *>(compositorHandle))
    , m_hwcBypass(0)
    , m_invalidated(0)
    , m_invalidationCountdown(0)
    , m_scheduledLayerList(false)
{
    m_hwc->setReleaseLayerListCallback(hwc_renderstage_delete_list);
    m_hwc->setBufferAvailableCallback(hwc_renderstage_buffer_available, this);
    m_hwc->setInvalidateCallback(hwc_renderstage_invalidate, this);
}

HwcRenderStage::~HwcRenderStage()
{
}


bool HwcRenderStage::render()
{
    QQuickWindowPrivate *d = QQuickWindowPrivate::get(m_window);
    if (d->renderer && d->renderer->rootNode()) {

        if (m_invalidated.testAndSetRelaxed(1, 0)) {
            m_invalidationCountdown = 5;
            disableHwc();
            return false;
        }

        if (m_hwcBypass.load()) {
            disableHwc();
            return false;;
        }

        QSGRootNode *rootNode = d->renderer->rootNode();
        m_nodesToTry.clear();
        bool layersOnly = checkSceneGraph(rootNode) && m_nodesToTry.size() > 0;

        bool isUsingLayersOnly = m_layerList && m_layerList == m_hwc->acceptedLayerList() && !m_layerList->eglRenderingEnabled;

        if (m_nodesToTry.size()) {

            bool scheduleAgain = (m_nodesInList != m_nodesToTry) || (layersOnly != isUsingLayersOnly);

            // After an invalidate, there will be a few frames where the HWC
            // refuses our layer lists, so we need to keep trying to convince
            // it.
            if (m_invalidationCountdown > 0 && m_layerList != m_hwc->acceptedLayerList()) {
                --m_invalidationCountdown;
                qCDebug(LIPSTICK_LOG_HWC, "HwcRenderStage::render(): invalidation countdown: %d", m_invalidationCountdown);
                scheduleAgain = true;
            }

            if (scheduleAgain) {
                m_layerList = hwc_renderstage_create_list(m_nodesToTry);
                m_layerList->eglRenderingEnabled = !layersOnly;
                if (LIPSTICK_LOG_HWC().isDebugEnabled()) {
                    qCDebug(LIPSTICK_LOG_HWC, "HwcRenderStage::render(), scheduling new layer list (using GL)");
                    hwc_renderstage_dump_layerlist(m_layerList);
                }
                foreach (HwcNode *n, m_nodesInList)
                    n->setBlocked(false);
                m_nodesInList = m_nodesToTry;
                m_scheduledLayerList = true;
                m_hwc->scheduleLayerList(m_layerList);
            }

            if (m_layerList && m_layerList == m_hwc->acceptedLayerList()) {
                m_invalidationCountdown = 0;
                if (m_scheduledLayerList) {
                    // newly accepted, toggle blocking in the scene graph...
                    if (LIPSTICK_LOG_HWC().isDebugEnabled()) {
                        qCDebug(LIPSTICK_LOG_HWC, "HwcRenderStage::render(), layer list was accepted (using HWC%s)",
                                m_layerList->eglRenderingEnabled ? "+GL" : "");
                        hwc_renderstage_dump_layerlist(m_layerList);
                    }
                    m_scheduledLayerList = false;
                    for (int i=0; i<m_nodesToTry.size(); ++i) {
                        HwcNode *n = m_nodesInList[i];
                        n->setBlocked(m_layerList->layers[i].accepted);
                    }
                }

                m_buffersInUseMutex.lock();
                for (int i=0; i<m_nodesInList.size(); ++i) {
                    HwcInterface::Layer &l = m_layerList->layers[i];
                    if (l.accepted) {
                        l.handle = m_nodesInList.at(i)->handle();
                        storeBuffer(l.handle);
                    } else {
                        l.handle = 0;
                    }
                }
                m_buffersInUseMutex.unlock();

                if (!m_layerList->eglRenderingEnabled) {
                    // Our list is the only content, so skip SG render stage.
                    return true;
                }
            }

        } else {
            if (LIPSTICK_LOG_HWC().isDebugEnabled()) {
                static bool once = false;
                if (m_layerList || !once) {
                    qCDebug(LIPSTICK_LOG_HWC, "HwcRenderStage::render(), no layers (using GL)");
                    once = true;
                }
            }
            disableHwc();
        }
    }
    return false;
}

bool HwcRenderStage::swap()
{
    if (m_layerList && m_layerList == m_hwc->acceptedLayerList()) {
        m_hwc->swapLayerList(m_layerList);
        return true;
    }
    return false;
}

// see disabledHwc docs below..
void HwcRenderStage::setBypassHwc(bool bypass)
{
    m_hwcBypass = int(bypass);
}

/*
    Called during the custom render stage's render if m_hwcBypass is set to true.
    This is typically used during screenshotting to make sure all HWC content is
    being rendered through textures.

    A note about threading. The flag to disable this is a QAtomicInt. It will
    be set before rendering and turned off once the result has been grabbed.
    This is safe because if the m_hwcBypass is set while rendering of an
    actual frame is about to happen, then we end up disabling HWC for that
    frame which means it is a bit slower to draw, but it still comes out ok.

    If we set the bypass flag during rendering of a frame, nothing will happen
    until the next render pass which should be the actual pass to grab stuff.
    In this pass it is set and HWC is disabled.

    Once the bypass flag is unset, rendering will continue as normal and we'll
    schedule a new layer list and should enter back into HWC composition mode
    within a frame or two and all is back to normal.
 */
void HwcRenderStage::disableHwc()
{
    if (!m_layerList)
        return;
    qCDebug(LIPSTICK_LOG_HWC, "hwc was turned off");
    foreach (HwcNode *n, m_nodesInList)
        n->setBlocked(false);
    m_nodesInList.clear();
    m_layerList = 0;
    // Tell the HwcInterface that we're no longer going to use the old list.
    m_hwc->scheduleLayerList(0);
}

void HwcRenderStage::hwcNodeDeleted(HwcNode *node)
{
    for (int i=0; i<m_nodesInList.size(); ) {
        if (m_nodesInList.at(i) == node) {
            m_nodesInList.remove(i);
        } else {
            ++i;
        }
    }
    disableHwc();
}

// We cannot use QMatrix4x4::optimize + translate bits or qFuzzyCompare because
// matrices have a high degree of floating point error and we need to take that
// into account when checking. Hence do a manual check using a fairly rough
// threshold.
static bool hwc_renderstage_isTranslate(const QMatrix4x4 &m)
{
    #define M0(r,c) (qAbs(m(r,c)) < T)
    #define M1(r,c) (qAbs(m(r,c) - 1) < T)
    static const qreal T = 0.0001;
    return M1(0,0) && M0(0,1) && M0(0,2)
        && M0(1,0) && M1(1,1) && M0(1,2)
        && M0(2,0) && M0(2,1) && M1(2,2) && M0(2,3)
        && M0(3,0) && M0(3,1) && M0(3,2) && M1(3,3);
    #undef M0
    #undef M1
}

/*
    Iterates over the scene graph in a left-to-right depth first manner,
    (back-to-front in visual terms). When it finds composer compatible
    nodes, these are added to the composition list.

    When a node is hit that is not hwcomposer compatible, the function starts
    returning false.

    Returns true if all content in the scene graph could be rendered by the
    hardware composer.

    Returns false if there exists content in the graph that must be rendered
    using EGL.

    The nodes to compose (if any) will be listed in m_nodesToTry.
 */

bool HwcRenderStage::checkSceneGraph(QSGNode *node)
{
    if (node->type() == QSG_HWC_NODE_TYPE) {

        HwcNode *hwcNode = static_cast<HwcNode *>(node);
        QQuickWindowPrivate *d = QQuickWindowPrivate::get(m_window);
        QSGRootNode *root = d->renderer->rootNode();
        Q_ASSERT(hwcNode->contentNode()); // It shouldn't be in the tree otherwise...

        // get x/y offset on screen and check for transformations...
        QSGNode *p = hwcNode->contentNode()->parent();
        QMatrix4x4 cm;
        while (p != root) {
            if (p->type() == QSGNode::TransformNodeType) {
                QSGTransformNode *tn = static_cast<QSGTransformNode *>(p);
                const QMatrix4x4 &m = tn->matrix();
                cm = m * cm;
            }
            p = p->parent();
        }

        if (!hwc_renderstage_isTranslate(cm))
            return true;

        hwc_renderstage_check_node(hwcNode);
        hwcNode->setPos(cm(0, 3), cm(1, 3));
        m_nodesToTry << hwcNode;

        // HwcNodes have only the one child, which is whatever node that holds
        // the buffer, so we don't want to traverse downwards from here.
        return true;

    } else if (node->isSubtreeBlocked()) {
        return true;

    } else if (node->type() == QSGNode::GeometryNodeType
               || (node->type() == QSGNode::OpacityNodeType && static_cast<QSGOpacityNode *>(node)->opacity() < 1.0f)
               || node->type() == QSGNode::ClipNodeType
               || node->type() == QSGNode::RenderNodeType) {
            return false;
    }

    for (QSGNode *child = node->firstChild(); child; child = child->nextSibling()) {
        if (!checkSceneGraph(child))
            return false;
    }

    return true;
}

void HwcRenderStage::storeBuffer(void *handle)
{
    // m_buffersInUseMutex must be locked during this call. See render()
    foreach (const BufferAndResource &b, m_buffersInUse)
        if (b.handle == handle)
            return;
    BufferAndResource b;
    b.callback = 0;
    b.callbackData = 0;
    b.handle = handle;
    m_buffersInUse << b;
}

void HwcRenderStage::signalOnBufferRelease(BufferReleaseCallback callback, void *handle, void *callbackData)
{
    QMutexLocker locker(&m_buffersInUseMutex);

    // Check if the buffer is in use and store for signalling later
    for (int i=0; i<m_buffersInUse.size(); ++i) {
        BufferAndResource &b = m_buffersInUse[i];
        if (b.handle == handle) {
            b.callbackData = callbackData;
            b.callback = callback;
            return;
        }
    }

    // Buffer is not in use so we can signal right away.
    callback(handle, callbackData);
}

void HwcRenderStage::bufferReleased(void *handle)
{
    QMutexLocker locker(&m_buffersInUseMutex);

    // Look up the buffer in the "in use" list and signal present
    for (int i=0; i<m_buffersInUse.size(); ++i) {
        BufferAndResource &b = m_buffersInUse[i];
        if (b.handle == handle) {
            if (b.callback)
                b.callback(b.handle, b.callbackData);
            m_buffersInUse.remove(i);
            return;
        }
    }
}

void HwcRenderStage::invalidated()
{
    m_invalidated = 1;
    m_window->update();
}

