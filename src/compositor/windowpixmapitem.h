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
#include "lipstickglobal.h"

class LipstickCompositor;
class LipstickCompositorWindow;
class LIPSTICK_EXPORT WindowPixmapItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(int windowId READ windowId WRITE setWindowId NOTIFY windowIdChanged)
    Q_PROPERTY(bool opaque READ opaque WRITE setOpaque NOTIFY opaqueChanged)
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(QSize windowSize READ windowSize WRITE setWindowSize NOTIFY windowSizeChanged)

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

protected:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *);
    virtual void geometryChanged(const QRectF &, const QRectF &);

signals:
    void windowIdChanged();
    void opaqueChanged();
    void radiusChanged();
    void windowSizeChanged();

private:
    void updateItem();

    LipstickCompositorWindow *m_item;
    QQuickItem *m_shaderEffect;
    int m_id;
    bool m_opaque;
    qreal m_radius;
};

#endif // WINDOWPIXMAPITEM_H
