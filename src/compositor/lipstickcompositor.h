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
#include <QWaylandQuickCompositor>
#include <QWaylandSurfaceItem>
#include <QPointer>
#include <MGConfItem>
#include <qmdisplaystate.h>

class WindowModel;
class LipstickCompositorWindow;
class LipstickCompositorProcWindow;
class QOrientationSensor;
class LipstickRecorderManager;
class LipstickKeymap;

class LIPSTICK_EXPORT LipstickCompositor : public QQuickWindow, public QWaylandQuickCompositor,
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
    Q_PROPERTY(LipstickKeymap *keymap READ keymap WRITE setKeymap NOTIFY keymapChanged)
    Q_PROPERTY(QObject* clipboard READ clipboard CONSTANT)
    Q_PROPERTY(QVariant orientationLock READ orientationLock NOTIFY orientationLockChanged)
    Q_PROPERTY(bool displayDimmed READ displayDimmed NOTIFY displayDimmedChanged)
    Q_PROPERTY(bool completed READ completed NOTIFY completedChanged)

public:
    LipstickCompositor();
    ~LipstickCompositor();

    static LipstickCompositor *instance();

    void classBegin() Q_DECL_OVERRIDE;
    void componentComplete() Q_DECL_OVERRIDE;
    void surfaceCreated(QWaylandSurface *surface) Q_DECL_OVERRIDE;
    bool openUrl(WaylandClient *client, const QUrl &url) Q_DECL_OVERRIDE;
    void retainedSelectionReceived(QMimeData *mimeData) Q_DECL_OVERRIDE;

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

    QVariant orientationLock() const { return m_orientationLock->value("dynamic"); }

    bool displayDimmed() const { return m_currentDisplayState == MeeGo::QmDisplayState::Dimmed; }

    LipstickKeymap *keymap() const;
    void setKeymap(LipstickKeymap *keymap);

    QObject *clipboard() const;

    bool debug() const;

    Q_INVOKABLE QObject *windowForId(int) const;
    Q_INVOKABLE void closeClientForWindowId(int);
    Q_INVOKABLE void clearKeyboardFocus();
    Q_INVOKABLE void setDisplayOff();
    Q_INVOKABLE QVariant settingsValue(const QString &key, const QVariant &defaultValue = QVariant()) const
        { return (key == "orientationLock") ? m_orientationLock->value(defaultValue) : MGConfItem("/lipstick/" + key).value(defaultValue); }

    LipstickCompositorProcWindow *mapProcWindow(const QString &title, const QString &category, const QRect &);
    LipstickCompositorProcWindow *mapProcWindow(const QString &title, const QString &category, const QRect &, QQuickItem *rootItem);

    QWaylandSurface *surfaceForId(int) const;

    bool completed();

    void setUpdatesEnabled(bool enabled);
    QWaylandSurfaceView *createView(QWaylandSurface *surf) Q_DECL_OVERRIDE;

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
    void orientationLockChanged();
    void displayDimmedChanged();

    void keymapChanged();

    void displayOn();
    void displayOff();
    void displayAboutToBeOn();
    void displayAboutToBeOff();

    void completedChanged();

private slots:
    void surfaceMapped();
    void surfaceUnmapped();
    void surfaceSizeChanged();
    void surfaceTitleChanged();
    void surfaceRaised();
    void surfaceLowered();
#if QT_VERSION >= QT_VERSION_CHECK(5,2,0)
    void surfaceDamaged(const QRegion &);
#else
    void surfaceDamaged(const QRect &);
#endif
    void windowSwapped();
    void windowDestroyed();
    void windowPropertyChanged(const QString &);
    bool openUrl(const QUrl &);
    void reactOnDisplayStateChanges(MeeGo::QmDisplayState::DisplayState state);
    void homeApplicationAboutToDestroy();
    void setScreenOrientationFromSensor();
    void clipboardDataChanged();
    void onVisibleChanged(bool visible);
    void onSurfaceDying();
    void updateKeymap();

    void initialize();

private:
    friend class LipstickCompositorWindow;
    friend class LipstickCompositorProcWindow;
    friend class WindowModel;
    friend class WindowPixmapItem;
    friend class WindowProperty;

    void surfaceUnmapped(LipstickCompositorWindow *item);

    int windowIdForLink(QWaylandSurface *, uint) const;

    void surfaceUnmapped(QWaylandSurface *);

    void windowAdded(int);
    void windowRemoved(int);
    void windowDestroyed(LipstickCompositorWindow *item);
    void readContent();

    QQmlComponent *shaderEffectComponent();

    static LipstickCompositor *m_instance;

    int m_totalWindowCount;
    QHash<int, LipstickCompositorWindow *> m_mappedSurfaces;
    QHash<int, LipstickCompositorWindow *> m_windows;

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
    QOrientationSensor* m_orientationSensor;
    QPointer<QMimeData> m_retainedSelection;
    MGConfItem *m_orientationLock;
    MeeGo::QmDisplayState::DisplayState m_currentDisplayState;
    bool m_updatesEnabled;
    bool m_completed;
    int m_onUpdatesDisabledUnfocusedWindowId;
    LipstickRecorderManager *m_recorder;
    LipstickKeymap *m_keymap;
};

#endif // LIPSTICKCOMPOSITOR_H
