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

#ifndef WINDOWPIXMAPITEM_H
#define WINDOWPIXMAPITEM_H

#include <QQuickItem>
#include <QPointer>
#include "lipstickglobal.h"

class QWaylandQuickSurface;
class QWaylandUnmapLock;

class LipstickCompositor;
class LipstickCompositorWindow;
class SnapshotTextureProvider;

class LIPSTICK_EXPORT WindowPixmapItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(int windowId READ windowId WRITE setWindowId NOTIFY windowIdChanged)
    Q_PROPERTY(bool hasPixmap READ hasPixmap NOTIFY hasPixmapChanged)
    Q_PROPERTY(bool opaque READ opaque WRITE setOpaque NOTIFY opaqueChanged)
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(QSize windowSize READ windowSize WRITE setWindowSize NOTIFY windowSizeChanged)
    Q_PROPERTY(qreal xOffset READ xOffset WRITE setXOffset NOTIFY xOffsetChanged)
    Q_PROPERTY(qreal yOffset READ yOffset WRITE setYOffset NOTIFY yOffsetChanged)
    Q_PROPERTY(qreal xScale READ xScale WRITE setXScale NOTIFY xScaleChanged)
    Q_PROPERTY(qreal yScale READ yScale WRITE setYScale NOTIFY yScaleChanged)

public:
    WindowPixmapItem();
    ~WindowPixmapItem();

    int windowId() const;
    void setWindowId(int);

    bool hasPixmap() const;

    bool opaque() const;
    void setOpaque(bool);

    qreal radius() const;
    void setRadius(qreal);

    QSize windowSize() const;
    void setWindowSize(const QSize &size);

    qreal xOffset() const;
    void setXOffset(qreal);

    qreal yOffset() const;
    void setYOffset(qreal);

    qreal xScale() const;
    void setXScale(qreal);

    qreal yScale() const;
    void setYScale(qreal);

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *);
    virtual void geometryChanged(const QRectF &, const QRectF &);

signals:
    void windowIdChanged();
    void hasPixmapChanged();
    void opaqueChanged();
    void radiusChanged();
    void windowSizeChanged();
    void xOffsetChanged();
    void yOffsetChanged();
    void xScaleChanged();
    void yScaleChanged();
    void renderOptionsChanged();

private slots:
    void surfaceDestroyed();
    void handleWindowSizeChanged();

private:
    class SurfaceReference
    {
    public:
        SurfaceReference();
        ~SurfaceReference();

        QWaylandQuickSurface *surface() { return m_surface; }
        QWaylandQuickSurface *operator ->() { return m_surface; }
        operator QWaylandQuickSurface *() { return m_surface; }
        void reset(QWaylandQuickSurface *surface = 0);

    private:
        QWaylandQuickSurface *m_surface;
        QWaylandUnmapLock *m_lock;
    };

    void configure(bool hasBuffer);
    void cleanupOpenGL();

    QPointer<LipstickCompositorWindow> m_item;
    SurfaceReference m_surface;
    QQuickItem *m_shaderEffect;
    int m_id;
    bool m_opaque;
    qreal m_radius;
    qreal m_xOffset;
    qreal m_yOffset;
    qreal m_xScale;
    qreal m_yScale;
    QSize m_windowSize;
    bool m_hasBuffer;
};

#endif // WINDOWPIXMAPITEM_H
