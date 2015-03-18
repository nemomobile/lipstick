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

class QWaylandUnmapLock;

class LipstickCompositor;
class LipstickCompositorWindow;
class LIPSTICK_EXPORT WindowPixmapItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(int windowId READ windowId WRITE setWindowId NOTIFY windowIdChanged)
    Q_PROPERTY(bool opaque READ opaque WRITE setOpaque NOTIFY opaqueChanged)
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(QSize windowSize READ windowSize WRITE setWindowSize NOTIFY windowSizeChanged)
    Q_PROPERTY(qreal xOffset READ xOffset WRITE setXOffset NOTIFY xOffsetChanged)
    Q_PROPERTY(qreal yOffset READ yOffset WRITE setYOffset NOTIFY yOffsetChanged)
    Q_PROPERTY(qreal xScale READ xScale WRITE setXScale NOTIFY xScaleChanged)
    Q_PROPERTY(qreal yScale READ yScale WRITE setYScale NOTIFY yScaleChanged)
    Q_PROPERTY(bool surfaceDestroyed READ surfaceDestroyed NOTIFY surfaceDestroyedChanged)

public:
    WindowPixmapItem();
    ~WindowPixmapItem();

    int windowId() const;
    void setWindowId(int);

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

    bool surfaceDestroyed() const;

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *);
    virtual void geometryChanged(const QRectF &, const QRectF &);

signals:
    void windowIdChanged();
    void opaqueChanged();
    void radiusChanged();
    void windowSizeChanged();
    void xOffsetChanged();
    void yOffsetChanged();
    void xScaleChanged();
    void yScaleChanged();
    void surfaceDestroyedChanged();

private:
    void updateItem();
    void configure(bool hasBuffer);

    QScopedPointer<LipstickCompositorWindow> m_item;
    QQuickItem *m_shaderEffect;
    int m_id;
    bool m_opaque;
    qreal m_radius;
    qreal m_xOffset;
    qreal m_yOffset;
    qreal m_xScale;
    qreal m_yScale;
    QWaylandUnmapLock *m_unmapLock;
    bool m_hasBuffer;
    QSGTextureProvider *m_textureProvider;
};

#endif // WINDOWPIXMAPITEM_H
