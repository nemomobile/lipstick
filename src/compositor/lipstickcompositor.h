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

#ifndef LIPSTICKCOMPOSITOR_H
#define LIPSTICKCOMPOSITOR_H

#include <QQuickWindow>
#include "lipstickglobal.h"
#include <QQmlParserStatus>
#include <QWaylandCompositor>
#include <QWaylandSurfaceItem>

class WindowModel;
class LipstickCompositorWindow;
class LipstickCompositorProcWindow;

class LIPSTICK_EXPORT LipstickCompositor : public QQuickWindow, public QWaylandCompositor,
                                           public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(int windowCount READ windowCount NOTIFY windowCountChanged)
    Q_PROPERTY(int ghostWindowCount READ ghostWindowCount NOTIFY ghostWindowCountChanged)
    Q_PROPERTY(bool homeActive READ homeActive WRITE setHomeActive NOTIFY homeActiveChanged)
    Q_PROPERTY(bool debug READ debug CONSTANT)

public:
    LipstickCompositor();
    ~LipstickCompositor();

    static LipstickCompositor *instance();

    virtual void classBegin();
    virtual void componentComplete();
    virtual void surfaceCreated(QWaylandSurface *surface);

    int windowCount() const;
    int ghostWindowCount() const;

    bool homeActive() const;
    void setHomeActive(bool);

    bool debug() const;

    Q_INVOKABLE QObject *windowForId(int) const;
    Q_INVOKABLE void clearKeyboardFocus();

    LipstickCompositorProcWindow *mapProcWindow(const QString &, const QRect &);

signals:
    void windowAdded(QObject *window);
    void windowRemoved(QObject *window);

    void windowCountChanged();
    void ghostWindowCountChanged();

    void availableWinIdsChanged();

    void homeActiveChanged();

private slots:
    void surfaceDestroyed();
    void surfaceMapped();
    void surfaceUnmapped();
    void surfaceSizeChanged();
    void surfaceTitleChanged();
    void windowSwapped();
    void windowDestroyed();
    void windowPropertyChanged();

private:
    friend class LipstickCompositorWindow;
    friend class LipstickCompositorProcWindow;
    friend class WindowModel;
    friend class WindowPixmapItem;
    friend class WindowProperty;

    void surfaceUnmapped(LipstickCompositorProcWindow *item);

    QWaylandSurface *surfaceForId(int) const;
    int windowIdForLink(QWaylandSurface *, uint) const;

    void surfaceUnmapped(QWaylandSurface *);

    void windowAdded(int);
    void windowRemoved(int);

    QQmlComponent *shaderEffectComponent();

    static LipstickCompositor *m_instance;

    int m_totalWindowCount;
    QHash<int, LipstickCompositorWindow *> m_mappedSurfaces;

    int m_nextWindowId;
    QList<WindowModel *> m_windowModels;

    bool m_homeActive;

    QQmlComponent *m_shaderEffect;
};

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

protected:
    virtual bool event(QEvent *);

signals:
    void userDataChanged();
    void titleChanged();
    void delayRemoveChanged();

private:
    friend class LipstickCompositor;
    friend class WindowPixmapItem;
    void imageAddref();
    void imageRelease();

    bool canRemove() const;
    void tryRemove();

    int m_windowId;
    QString m_category;
    int m_ref;
    bool m_delayRemove:1;
    bool m_windowClosed:1;
    bool m_removePosted:1;
    QVariant m_data;
};

class LipstickCompositorProcWindow : public LipstickCompositorWindow
{
    Q_OBJECT
public:
    void hide();

    virtual bool isInProcess() const;
    virtual bool isTextureProvider() const { return false; }

    virtual QString title() const;
    void setTitle(const QString &);
private:
    friend class LipstickCompositor;
    LipstickCompositorProcWindow(int windowId, QQuickItem *parent = 0);

    QString m_title;
};

#endif // LIPSTICKCOMPOSITOR_H
