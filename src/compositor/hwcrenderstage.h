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

#ifndef HWCRENDERSTAGE
#define HWCRENDERSTAGE

#include <private/qquickwindow_p.h>

Q_DECLARE_LOGGING_CATEGORY(LIPSTICK_LOG_HWC)

class LipstickCompositor;
class HwcRenderStage;

namespace HwcInterface {
    class Compositor;
    class LayerList;
}

class HwcNode : public QSGNode
{
public:
    HwcNode(QQuickWindow *window);
    ~HwcNode();

    QRect bounds() const;

    void update(QSGGeometryNode *node, void *handle);

    void *handle() const { return m_buffer_handle; }
    bool isSubtreeBlocked() const { return m_blocked; }
    void setBlocked(bool b);

    float x() const { return m_x; }
    float y() const { return m_y; }
    void setPos(float x, float y) {
        m_x = x;
        m_y = y;
    }

    QSGGeometryNode *contentNode() const { return m_contentNode; }
    HwcRenderStage *renderStage() const { return m_renderStage; }

private:
    HwcRenderStage *m_renderStage;
    QSGGeometryNode *m_contentNode;
    void *m_buffer_handle;
    float m_x, m_y;
    bool m_blocked;
};

class HwcRenderStage : public QObject, public QQuickCustomRenderStage
{
    Q_OBJECT

public:
    HwcRenderStage(LipstickCompositor *lipstick, void *hwcHandle);
    ~HwcRenderStage();
    bool render() Q_DECL_OVERRIDE;
    bool swap() Q_DECL_OVERRIDE;

    typedef void (*BufferReleaseCallback)(void *bufferHandle, void *callbackData);
    void signalOnBufferRelease(BufferReleaseCallback callback, void *handle, void *callbackData);

    static void initialize(LipstickCompositor *lipstick);
    static bool isHwcEnabled() { return m_hwcEnabled; }

    void bufferReleased(void *);
    void setBypassHwc(bool bypass);
    void hwcNodeDeleted(HwcNode *node);
    void invalidated();

    // LipstickCompositorWindowHwcNode will post events to this object for the
    // sole purpose of having them delivered on the GUI thread when the LCW
    // instance might have been destroyed might have been destroyed. Keep this
    // in mind if HwcRenderStage::event() ever gets implemented.

private:
    bool checkSceneGraph(QSGNode *node);
    void storeBuffer(void *handle);
    void disableHwc();

    LipstickCompositor *m_lipstick;
    QQuickWindow *m_window;

    HwcInterface::Compositor *m_hwc;
    QVector<HwcNode *> m_nodesInList;
    QVector<HwcNode *> m_nodesToTry;
    QAtomicInt m_hwcBypass;
    QAtomicInt m_invalidated;
    int m_invalidationCountdown; // R&W on render thread only

    struct BufferAndResource {
        void *handle;
        BufferReleaseCallback callback;
        void *callbackData;
    };
    QVector<BufferAndResource> m_buffersInUse;
    QMutex m_buffersInUseMutex;

    HwcInterface::LayerList *m_layerList; // R&W on render thread only

    bool m_scheduledLayerList;

    static bool m_hwcEnabled;
};

#endif // HWCRENDERSTAGE