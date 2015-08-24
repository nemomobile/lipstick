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

class QWaylandSurface;
class QWaylandUnmapLock;

class LipstickCompositor;
class LipstickCompositorWindow;
class SnapshotTextureProvider;

class LIPSTICK_EXPORT WindowPixmapItem : public QQuickItem
{
    Q_OBJECT
    Q_ENUMS(RenderOption)
    Q_PROPERTY(int windowId READ windowId WRITE setWindowId NOTIFY windowIdChanged)
    Q_PROPERTY(bool hasPixmap READ hasPixmap NOTIFY hasPixmapChanged)
    Q_PROPERTY(bool opaque READ opaque WRITE setOpaque NOTIFY opaqueChanged)
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(QSize windowSize READ windowSize WRITE setWindowSize NOTIFY windowSizeChanged)
    Q_PROPERTY(qreal xOffset READ xOffset WRITE setXOffset NOTIFY xOffsetChanged)
    Q_PROPERTY(qreal yOffset READ yOffset WRITE setYOffset NOTIFY yOffsetChanged)
    Q_PROPERTY(qreal xScale READ xScale WRITE setXScale NOTIFY xScaleChanged)
    Q_PROPERTY(qreal yScale READ yScale WRITE setYScale NOTIFY yScaleChanged)
    Q_PROPERTY(int render READ renderOptions WRITE setRenderOptions NOTIFY renderOptionsChanged)

public:
    enum RenderOption {
        RenderWindow    = 0x01,
        RenderSnapshot  = 0x02
    };

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

    int renderOptions() const;
    void setRenderOptions(int options);

    bool event(QEvent *event);

protected:
    void itemChange(ItemChange change, const ItemChangeData &value);
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
    void handleWindowSizeChanged();
    void itemDestroyed(QObject *);

private:
    class SurfaceReference
    {
    public:
        SurfaceReference();
        ~SurfaceReference();

        QWaylandSurface *surface() { return m_surface; }
        QWaylandSurface *operator ->() { return m_surface; }
        operator QWaylandSurface *() { return m_surface; }
        void reset(QWaylandSurface *surface = 0);

        void lock();
        void unlock();

    private:
        QWaylandSurface *m_surface;
        QWaylandUnmapLock *m_lock;
    };


    void updateItem();
    void updateHasPixmap();
    void surfaceDestroyed();
    void configure(bool hasBuffer);
    void cleanupOpenGL();
    void scheduleCapture();
    void cancelCapture();
    void resetCapture();
    void captureSnapshot();

    QPointer<LipstickCompositorWindow> m_item;
    SurfaceReference m_surface;
    SurfaceReference m_captureSurface;
    QQuickItem *m_shaderEffect;
    int m_id;
    bool m_opaque;
    qreal m_radius;
    qreal m_xOffset;
    qreal m_yOffset;
    qreal m_xScale;
    qreal m_yScale;
    QSize m_windowSize;
    int m_renderOptions;
    bool m_hasBuffer;
    bool m_hasPixmap;
    bool m_surfaceDestroyed;
    bool m_haveSnapshot;
    bool m_clearCapture;
    bool m_clearNotified;
    SnapshotTextureProvider *m_textureProvider;

    static struct SnapshotProgram *s_snapshotProgram;
};

#endif // WINDOWPIXMAPITEM_H
