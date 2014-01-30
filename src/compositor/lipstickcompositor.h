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
#include <QPointer>
#include <qmdisplaystate.h>

class WindowModel;
class LipstickCompositorWindow;
class LipstickCompositorProcWindow;
class QOrientationSensor;

class LIPSTICK_EXPORT LipstickCompositor : public QQuickWindow, public QWaylandCompositor,
                                           public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(int windowCount READ windowCount NOTIFY windowCountChanged)
    Q_PROPERTY(int ghostWindowCount READ ghostWindowCount NOTIFY ghostWindowCountChanged)
    Q_PROPERTY(bool homeActive READ homeActive WRITE setHomeActive NOTIFY homeActiveChanged)
    Q_PROPERTY(bool debug READ debug CONSTANT)
    Q_PROPERTY(QWaylandSurface* fullscreenSurface READ fullscreenSurface WRITE setFullscreenSurface NOTIFY fullscreenSurfaceChanged)
    Q_PROPERTY(bool directRenderingActive READ directRenderingActive NOTIFY directRenderingActiveChanged)
    Q_PROPERTY(int topmostWindowId READ topmostWindowId WRITE setTopmostWindowId NOTIFY topmostWindowIdChanged)
    Q_PROPERTY(Qt::ScreenOrientation screenOrientation READ screenOrientation WRITE setScreenOrientation NOTIFY screenOrientationChanged)
    Q_PROPERTY(Qt::ScreenOrientation sensorOrientation READ sensorOrientation NOTIFY sensorOrientationChanged)
    Q_PROPERTY(QObject* clipboard READ clipboard CONSTANT)

public:
    LipstickCompositor();
    ~LipstickCompositor();

    static LipstickCompositor *instance();

    virtual void classBegin();
    virtual void componentComplete();
    virtual void surfaceCreated(QWaylandSurface *surface);
    virtual void openUrl(WaylandClient *client, const QUrl &url);
    virtual void retainedSelectionReceived(QMimeData *mimeData);

    int windowCount() const;
    int ghostWindowCount() const;

    bool homeActive() const;
    void setHomeActive(bool);

    QWaylandSurface *fullscreenSurface() const { return m_fullscreenSurface; }
    void setFullscreenSurface(QWaylandSurface *surface);
    bool directRenderingActive() const { return m_directRenderingActive; }

    int topmostWindowId() const { return m_topmostWindowId; }
    void setTopmostWindowId(int id);

    Qt::ScreenOrientation screenOrientation() const { return m_screenOrientation; }
    void setScreenOrientation(Qt::ScreenOrientation screenOrientation);

    Qt::ScreenOrientation sensorOrientation() const { return m_sensorOrientation; }

    QObject *clipboard() const;

    bool debug() const;

    Q_INVOKABLE QObject *windowForId(int) const;
    Q_INVOKABLE void closeClientForWindowId(int);
    Q_INVOKABLE void clearKeyboardFocus();
    Q_INVOKABLE void setDisplayOff();

    LipstickCompositorProcWindow *mapProcWindow(const QString &title, const QString &category, const QRect &);

    QWaylandSurface *surfaceForId(int) const;

signals:
    void windowAdded(QObject *window);
    void windowRemoved(QObject *window);
    void windowRaised(QObject *window);
    void windowLowered(QObject *window);
    void windowHidden(QObject *window);

    void windowCountChanged();
    void ghostWindowCountChanged();

    void availableWinIdsChanged();

    void homeActiveChanged();
    void fullscreenSurfaceChanged();
    void directRenderingActiveChanged();
    void topmostWindowIdChanged();
    void screenOrientationChanged();
    void sensorOrientationChanged();

    void displayOn();
    void displayOff();
    void displayAboutToBeOn();

protected:
    virtual bool event(QEvent *);
    virtual void surfaceAboutToBeDestroyed(QWaylandSurface *surface);

private slots:
    void clearUpdateRequest();
    void maybePostUpdateRequest();
    void surfaceMapped();
    void surfaceUnmapped();
    void surfaceSizeChanged();
    void surfaceTitleChanged();
    void surfaceRaised();
    void surfaceLowered();
    void surfaceDamaged(const QRect &);
    void windowSwapped();
    void windowDestroyed();
    void windowPropertyChanged(const QString &);
    void openUrl(const QUrl &);
    void reactOnDisplayStateChanges(MeeGo::QmDisplayState::DisplayState state);
    void homeApplicationAboutToDestroy();
    void setScreenOrientationFromSensor();
    void clipboardDataChanged();

private:
    friend class LipstickCompositorWindow;
    friend class LipstickCompositorProcWindow;
    friend class WindowModel;
    friend class WindowPixmapItem;
    friend class WindowProperty;

    void surfaceUnmapped(LipstickCompositorProcWindow *item);

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
    QWaylandSurface *m_fullscreenSurface;
    bool m_directRenderingActive;
    int m_topmostWindowId;
    Qt::ScreenOrientation m_screenOrientation;
    Qt::ScreenOrientation m_sensorOrientation;
    MeeGo::QmDisplayState *m_displayState;
    QAtomicInt m_updateRequestPosted;
    QOrientationSensor* m_orientationSensor;
    QPointer<QMimeData> m_retainedSelection;
};

#endif // LIPSTICKCOMPOSITOR_H
