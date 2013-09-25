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

#ifndef LIPSTICKCOMPOSITORWINDOW_H
#define LIPSTICKCOMPOSITORWINDOW_H

#include <QWaylandSurfaceItem>
#include "lipstickglobal.h"

class LIPSTICK_EXPORT LipstickCompositorWindow : public QWaylandSurfaceItem
{
    Q_OBJECT

    Q_PROPERTY(int windowId READ windowId CONSTANT)
    Q_PROPERTY(bool isInProcess READ isInProcess CONSTANT)

    Q_PROPERTY(bool delayRemove READ delayRemove WRITE setDelayRemove NOTIFY delayRemoveChanged)
    Q_PROPERTY(QVariant userData READ userData WRITE setUserData NOTIFY userDataChanged)

    Q_PROPERTY(QString category READ category CONSTANT)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(qint64 processId READ processId CONSTANT)

    Q_PROPERTY(QRect mouseRegionBounds READ mouseRegionBounds NOTIFY mouseRegionBoundsChanged)

public:
    LipstickCompositorWindow(int windowId, const QString &, QWaylandSurface *surface, QQuickItem *parent = 0);

    QVariant userData() const;
    void setUserData(QVariant);

    int windowId() const;
    qint64 processId() const;

    bool delayRemove() const;
    void setDelayRemove(bool);

    QString category() const;
    virtual QString title() const;
    virtual bool isInProcess() const;

    QRect mouseRegionBounds() const;

protected:
    virtual bool event(QEvent *);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    virtual void touchEvent(QTouchEvent *event);

signals:
    void userDataChanged();
    void titleChanged();
    void delayRemoveChanged();
    void mouseRegionBoundsChanged();

private slots:
    void handleTouchCancel();

private:
    friend class LipstickCompositor;
    friend class WindowPixmapItem;
    void imageAddref();
    void imageRelease();

    bool canRemove() const;
    void tryRemove();
    void refreshMouseRegion();

    int m_windowId;
    QString m_category;
    int m_ref;
    bool m_delayRemove:1;
    bool m_windowClosed:1;
    bool m_removePosted:1;
    bool m_mouseRegionValid:1;
    QVariant m_data;
    QRegion m_mouseRegion;
};

#endif // LIPSTICKCOMPOSITORWINDOW_H
