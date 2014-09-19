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

#ifdef HAVE_CONTENTACTION
#include <contentaction.h>
#endif

#include <QWaylandInputDevice>
#include <QDesktopServices>
#include <QtSensors/QOrientationSensor>
#include <QClipboard>
#include <QMimeData>
#include <QtGui/qpa/qplatformnativeinterface.h>
#include "homeapplication.h"
#include "windowmodel.h"
#include "lipstickcompositorprocwindow.h"
#include "lipstickcompositor.h"
#include "lipstickcompositoradaptor.h"
#include "lipsticksettings.h"
#include <qpa/qwindowsysteminterface.h>

LipstickCompositor *LipstickCompositor::m_instance = 0;

LipstickCompositor::LipstickCompositor()
: QWaylandCompositor(this), m_totalWindowCount(0), m_nextWindowId(1), m_homeActive(true), m_shaderEffect(0),
  m_fullscreenSurface(0), m_directRenderingActive(false), m_topmostWindowId(0), m_screenOrientation(Qt::PrimaryOrientation), m_sensorOrientation(Qt::PrimaryOrientation), m_displayState(new MeeGo::QmDisplayState(this)), m_retainedSelection(0), m_compositorSettings("nemomobile", "lipstick"), m_previousDisplayState(m_displayState->get()), m_updatesEnabled(true), m_onUpdatesDisabledUnfocusedWindowId(0),
  m_displayOn(true)
{
    setColor(Qt::black);
    setRetainedSelectionEnabled(true);

    if (m_instance) qFatal("LipstickCompositor: Only one compositor instance per process is supported");
    m_instance = this;

    connect(this, SIGNAL(visibleChanged(bool)), this, SLOT(onVisibleChanged(bool)));
    QObject::connect(this, SIGNAL(afterRendering()), this, SLOT(windowSwapped()));
    connect(m_displayState, SIGNAL(displayStateChanged(MeeGo::QmDisplayState::DisplayState)), this, SLOT(reactOnDisplayStateChanges(MeeGo::QmDisplayState::DisplayState)));
    QObject::connect(HomeApplication::instance(), SIGNAL(aboutToDestroy()), this, SLOT(homeApplicationAboutToDestroy()));
    connect(this, &QQuickWindow::beforeSynchronizing, this, &LipstickCompositor::startFrame, Qt::DirectConnection);

    m_orientationSensor = new QOrientationSensor(this);
    QObject::connect(m_orientationSensor, SIGNAL(readingChanged()), this, SLOT(setScreenOrientationFromSensor()));
    if (!m_orientationSensor->connectToBackend()) {
        qWarning() << "Could not connect to the orientation sensor backend";
    } else {
        if (!m_orientationSensor->start())
            qWarning() << "Could not start the orientation sensor";
    }
    emit HomeApplication::instance()->homeActiveChanged();

    QDesktopServices::setUrlHandler("http", this, "openUrl");
    QDesktopServices::setUrlHandler("https", this, "openUrl");
    QDesktopServices::setUrlHandler("mailto", this, "openUrl");

    connect(QGuiApplication::clipboard(), SIGNAL(dataChanged()), SLOT(clipboardDataChanged()));

    new LipstickCompositorAdaptor(this);

    QDBusConnection systemBus = QDBusConnection::systemBus();
    if (!systemBus.registerService("org.nemomobile.compositor")) {
        qWarning("Unable to register D-Bus service org.nemomobile.compositor: %s", systemBus.lastError().message().toUtf8().constData());
    }
    if (!systemBus.registerObject("/", this)) {
        qWarning("Unable to register object at path /: %s", systemBus.lastError().message().toUtf8().constData());
    }
}

LipstickCompositor::~LipstickCompositor()
{
    // ~QWindow can a call into onVisibleChanged and QWaylandCompositor after we
    // are destroyed, so disconnect it.
    disconnect(this, SIGNAL(visibleChanged(bool)), this, SLOT(onVisibleChanged(bool)));
    delete m_shaderEffect;
}

LipstickCompositor *LipstickCompositor::instance()
{
    return m_instance;
}

void LipstickCompositor::homeApplicationAboutToDestroy()
{
    m_instance = 0;
    delete this;
}

void LipstickCompositor::classBegin()
{
}

void LipstickCompositor::onVisibleChanged(bool visible)
{
    if (!visible) {
#if QT_VERSION >= QT_VERSION_CHECK(5,2,0)
        sendFrameCallbacks(surfaces());
#else
        frameFinished(0);
#endif
    }
}

void LipstickCompositor::componentComplete()
{
    QWaylandCompositor::setOutputGeometry(QRect(0, 0, width(), height()));
}

void LipstickCompositor::surfaceCreated(QWaylandSurface *surface)
{
    Q_UNUSED(surface)
    connect(surface, SIGNAL(mapped()), this, SLOT(surfaceMapped()));
    connect(surface, SIGNAL(unmapped()), this, SLOT(surfaceUnmapped()));
    connect(surface, SIGNAL(sizeChanged()), this, SLOT(surfaceSizeChanged()));
    connect(surface, SIGNAL(titleChanged()), this, SLOT(surfaceTitleChanged()));
    connect(surface, SIGNAL(windowPropertyChanged(QString,QVariant)), this, SLOT(windowPropertyChanged(QString)));
    connect(surface, SIGNAL(raiseRequested()), this, SLOT(surfaceRaised()));
    connect(surface, SIGNAL(lowerRequested()), this, SLOT(surfaceLowered()));
#if QT_VERSION >= QT_VERSION_CHECK(5,2,0)
    connect(surface, SIGNAL(damaged(QRegion)), this, SLOT(surfaceDamaged(QRegion)));
#else
    connect(surface, SIGNAL(damaged(QRect)), this, SLOT(surfaceDamaged(QRect)));
#endif
}

bool LipstickCompositor::openUrl(WaylandClient *client, const QUrl &url)
{
    Q_UNUSED(client)
    return openUrl(url);
}

bool LipstickCompositor::openUrl(const QUrl &url)
{
#if defined(HAVE_CONTENTACTION)
    ContentAction::Action action = url.scheme() == "file"? ContentAction::Action::defaultActionForFile(url.toString()) : ContentAction::Action::defaultActionForScheme(url.toString());
    if (action.isValid()) {
        action.trigger();
    }
    return action.isValid();
#else
    Q_UNUSED(url)
    return false;
#endif
}

void LipstickCompositor::retainedSelectionReceived(QMimeData *mimeData)
{
    if (!m_retainedSelection)
        m_retainedSelection = new QMimeData;

    // Make a copy to allow QClipboard to take ownership of our data
    m_retainedSelection->clear();
    foreach (const QString &format, mimeData->formats())
        m_retainedSelection->setData(format, mimeData->data(format));

    QGuiApplication::clipboard()->setMimeData(m_retainedSelection.data());
}

int LipstickCompositor::windowCount() const
{
    return m_mappedSurfaces.count();
}

int LipstickCompositor::ghostWindowCount() const
{
    return m_totalWindowCount - windowCount();
}

bool LipstickCompositor::homeActive() const
{
    return m_homeActive;
}

void LipstickCompositor::setHomeActive(bool a)
{
    if (a == m_homeActive)
        return;

    m_homeActive = a;

    emit homeActiveChanged();
    emit HomeApplication::instance()->homeActiveChanged();
}

bool LipstickCompositor::debug() const
{
    static enum { Yes, No, Unknown } status = Unknown;
    if (status == Unknown) {
        QByteArray v = qgetenv("LIPSTICK_COMPOSITOR_DEBUG");
        bool value = !v.isEmpty() && v != "0" && v != "false";
        if (value) status = Yes;
        else status = No;
    }
    return status == Yes;
}

QObject *LipstickCompositor::windowForId(int id) const
{
    LipstickCompositorWindow *window = m_mappedSurfaces.value(id, 0);
    return window;
}

void LipstickCompositor::closeClientForWindowId(int id)
{
    LipstickCompositorWindow *window = m_mappedSurfaces.value(id, 0);
    if (window && window->surface())
        destroyClientForSurface(window->surface());
}

QWaylandSurface *LipstickCompositor::surfaceForId(int id) const
{
    LipstickCompositorWindow *window = m_mappedSurfaces.value(id, 0);
    return window?window->surface():0;
}

int LipstickCompositor::windowIdForLink(QWaylandSurface *s, uint link) const
{
    for (QHash<int, LipstickCompositorWindow *>::ConstIterator iter = m_mappedSurfaces.begin();
         iter != m_mappedSurfaces.end(); ++iter) {

        QWaylandSurface *windowSurface = iter.value()->surface();

        if (windowSurface && windowSurface->processId() == s->processId() &&
            windowSurface->windowProperties().value("WINID", uint(0)).toUInt() == link)
            return iter.value()->windowId();
    }

    return 0;
}

void LipstickCompositor::clearKeyboardFocus()
{
    defaultInputDevice()->setKeyboardFocus(0);
}

void LipstickCompositor::setDisplayOff()
{
    m_displayState->set(MeeGo::QmDisplayState::Off);
}

#if QT_VERSION >= QT_VERSION_CHECK(5,2,0)
void LipstickCompositor::surfaceDamaged(const QRegion &)
#else
void LipstickCompositor::surfaceDamaged(const QRect &)
#endif
{
    if (!isVisible()) {
        // If the compositor is not visible, do not throttle.
        // make it conditional to QT_WAYLAND_COMPOSITOR_NO_THROTTLE?
#if QT_VERSION >= QT_VERSION_CHECK(5,2,0)
        sendFrameCallbacks(surfaces());
#else
        frameFinished(0);
#endif
    }
}

void LipstickCompositor::setFullscreenSurface(QWaylandSurface *surface)
{
    if (surface == m_fullscreenSurface)
        return;

    // Prevent flicker when returning to composited mode
    if (!surface && m_fullscreenSurface && m_fullscreenSurface->surfaceItem())
        m_fullscreenSurface->surfaceItem()->update();

    m_fullscreenSurface = surface;

    emit fullscreenSurfaceChanged();
}

QObject *LipstickCompositor::clipboard() const
{
    return QGuiApplication::clipboard();
}

void LipstickCompositor::setTopmostWindowId(int id)
{
    if (id != m_topmostWindowId) {
        m_topmostWindowId = id;
        emit topmostWindowIdChanged();
    }
}

void LipstickCompositor::surfaceAboutToBeDestroyed(QWaylandSurface *surface)
{
    Q_ASSERT(surface);
    LipstickCompositorWindow *item = static_cast<LipstickCompositorWindow *>(surface->surfaceItem());
    surface->setSurfaceItem(0);

    if (surface == m_fullscreenSurface)
        setFullscreenSurface(0);

    if (item) {
        int id = item->windowId();

        int gc = ghostWindowCount();
        m_mappedSurfaces.remove(item->windowId());

        emit windowCountChanged();
        emit windowRemoved(item);

        item->m_windowClosed = true;
        item->tryRemove();

        if (gc != ghostWindowCount())
            emit ghostWindowCountChanged();

        windowRemoved(id);

        emit availableWinIdsChanged();
    }
}

void LipstickCompositor::surfaceMapped()
{
    QWaylandSurface *surface = qobject_cast<QWaylandSurface *>(sender());
    //Ignore surface if it's not a window surface
    if (!surface->hasShellSurface())
        return;

    QVariantMap properties = surface->windowProperties();
    QString category = properties.value("CATEGORY").toString();

    if (surface->surfaceItem())
        return;

    // The surface was mapped for the first time
    int id = m_nextWindowId++;
    LipstickCompositorWindow *item = new LipstickCompositorWindow(id, category, surface, contentItem());
    item->setSize(surface->size());
    QObject::connect(item, SIGNAL(destroyed(QObject*)), this, SLOT(windowDestroyed()));
    m_totalWindowCount++;
    m_mappedSurfaces.insert(id, item);

    item->setTouchEventsEnabled(true);

    emit windowCountChanged();
    emit windowAdded(item);

    windowAdded(id);

    emit availableWinIdsChanged();
}

void LipstickCompositor::surfaceUnmapped()
{
    surfaceUnmapped(qobject_cast<QWaylandSurface *>(sender()));
}

void LipstickCompositor::surfaceSizeChanged()
{
    QWaylandSurface *surface = qobject_cast<QWaylandSurface *>(sender());

    LipstickCompositorWindow *window = static_cast<LipstickCompositorWindow *>(surface->surfaceItem());
    if (window)
        window->setSize(surface->size());
}

void LipstickCompositor::surfaceTitleChanged()
{
    QWaylandSurface *surface = qobject_cast<QWaylandSurface *>(sender());
    LipstickCompositorWindow *window = static_cast<LipstickCompositorWindow *>(surface->surfaceItem());

    if (window) {
        emit window->titleChanged();

        int windowId = window->windowId();

        for (int ii = 0; ii < m_windowModels.count(); ++ii)
            m_windowModels.at(ii)->titleChanged(windowId);
    }
}

void LipstickCompositor::surfaceRaised()
{
    QWaylandSurface *surface = qobject_cast<QWaylandSurface *>(sender());
    LipstickCompositorWindow *window = static_cast<LipstickCompositorWindow *>(surface->surfaceItem());

    if (window) {
        emit windowRaised(window);
    }
}

void LipstickCompositor::surfaceLowered()
{
    QWaylandSurface *surface = qobject_cast<QWaylandSurface *>(sender());
    LipstickCompositorWindow *window = static_cast<LipstickCompositorWindow *>(surface->surfaceItem());

    if (window) {
        emit windowLowered(window);
    }
}

void LipstickCompositor::startFrame()
{
#if QT_VERSION >= QT_VERSION_CHECK(5,2,0)
    frameStarted();
#endif
}

void LipstickCompositor::windowSwapped()
{
#if QT_VERSION >= QT_VERSION_CHECK(5,2,0)
    sendFrameCallbacks(surfaces());
#else
    frameFinished(0);
#endif
}

void LipstickCompositor::windowDestroyed()
{
    m_totalWindowCount--;
    emit ghostWindowCountChanged();
}

void LipstickCompositor::windowPropertyChanged(const QString &property)
{
    QWaylandSurface *surface = qobject_cast<QWaylandSurface *>(sender());

    if (debug())
        qDebug() << "Window properties changed:" << surface << surface->windowProperties();

    if (property == QLatin1String("MOUSE_REGION")) {
        LipstickCompositorWindow *window = static_cast<LipstickCompositorWindow *>(surface->surfaceItem());
        if (window)
            window->refreshMouseRegion();
    } else if (property == QLatin1String("GRABBED_KEYS")) {
        LipstickCompositorWindow *window = static_cast<LipstickCompositorWindow *>(surface->surfaceItem());
        if (window)
            window->refreshGrabbedKeys();
    }
}

void LipstickCompositor::surfaceUnmapped(QWaylandSurface *surface)
{
    if (surface == m_fullscreenSurface)
        setFullscreenSurface(0);

    LipstickCompositorWindow *window = static_cast<LipstickCompositorWindow *>(surface->surfaceItem());
    if (window)
        emit windowHidden(window);
}

void LipstickCompositor::surfaceUnmapped(LipstickCompositorProcWindow *item)
{
    int id = item->windowId();

    int gc = ghostWindowCount();
    m_mappedSurfaces.remove(item->windowId());

    emit windowCountChanged();
    emit windowRemoved(item);

    item->m_windowClosed = true;
    item->tryRemove();

    if (gc != ghostWindowCount())
        emit ghostWindowCountChanged();

    windowRemoved(id);

    emit availableWinIdsChanged();
}

void LipstickCompositor::windowAdded(int id)
{
    for (int ii = 0; ii < m_windowModels.count(); ++ii)
        m_windowModels.at(ii)->addItem(id);
}

void LipstickCompositor::windowRemoved(int id)
{
    for (int ii = 0; ii < m_windowModels.count(); ++ii)
        m_windowModels.at(ii)->remItem(id);
}

QQmlComponent *LipstickCompositor::shaderEffectComponent()
{
    const char *qml_source =
        "import QtQuick 2.0\n"
        "ShaderEffect {\n"
            "property QtObject window\n"
            "property ShaderEffectSource source: ShaderEffectSource { sourceItem: window }\n"
        "}";

    if (!m_shaderEffect) {
        m_shaderEffect = new QQmlComponent(qmlEngine(this));
        m_shaderEffect->setData(qml_source, QUrl());
    }
    return m_shaderEffect;
}

void LipstickCompositor::setScreenOrientation(Qt::ScreenOrientation screenOrientation)
{
    if (m_screenOrientation != screenOrientation) {
        if (debug())
            qDebug() << "Setting screen orientation on QWaylandCompositor";

        QWaylandCompositor::setScreenOrientation(screenOrientation);
        QWindowSystemInterface::handleScreenOrientationChange(qApp->primaryScreen(),screenOrientation);

        m_screenOrientation = screenOrientation;
        emit screenOrientationChanged();
    }
}

void LipstickCompositor::reactOnDisplayStateChanges(MeeGo::QmDisplayState::DisplayState state)
{
    int on = -1;
    if (state == MeeGo::QmDisplayState::On) {
        emit displayOn();
        on = 1;
    } else if (state == MeeGo::QmDisplayState::Off) {
        QCoreApplication::postEvent(this, new QTouchEvent(QEvent::TouchCancel));
        emit displayOff();
        on = 0;
    }

    bool changeInDimming = (state == MeeGo::QmDisplayState::Dimmed) != (m_previousDisplayState == MeeGo::QmDisplayState::Dimmed);

    if (on != -1 && m_displayOn != on) {
        m_displayOn = on;
        emit displayIsOnChanged();
    }

    m_previousDisplayState = state;

    if (changeInDimming) {
        emit displayDimmedChanged();
    }
}

void LipstickCompositor::setScreenOrientationFromSensor()
{
    QOrientationReading* reading = m_orientationSensor->reading();

    if (debug())
        qDebug() << "Screen orientation changed " << reading->orientation();

    Qt::ScreenOrientation sensorOrientation = m_sensorOrientation;
    switch (reading->orientation()) {
        case QOrientationReading::TopUp:
            sensorOrientation = Qt::PortraitOrientation;
            break;
        case QOrientationReading::TopDown:
            sensorOrientation = Qt::InvertedPortraitOrientation;
            break;
        case QOrientationReading::LeftUp:
            sensorOrientation = Qt::InvertedLandscapeOrientation;
            break;
        case QOrientationReading::RightUp:
            sensorOrientation = Qt::LandscapeOrientation;
            break;
        case QOrientationReading::FaceUp:
        case QOrientationReading::FaceDown:
            /* Keep screen orientation at previous state */
            break;
        case QOrientationReading::Undefined:
        default:
            sensorOrientation = Qt::PrimaryOrientation;
            break;
    }

    if (sensorOrientation != m_sensorOrientation) {
        m_sensorOrientation = sensorOrientation;
        emit sensorOrientationChanged();
    }
}

void LipstickCompositor::clipboardDataChanged()
{
    const QMimeData *mimeData = QGuiApplication::clipboard()->mimeData();
    if (mimeData && mimeData != m_retainedSelection)
        overrideSelection(const_cast<QMimeData *>(mimeData));
}

void LipstickCompositor::setUpdatesEnabled(bool enabled)
{
    if (m_updatesEnabled != enabled) {
        m_updatesEnabled = enabled;

        if (!m_updatesEnabled) {
            emit displayAboutToBeOff();
            LipstickCompositorWindow *topmostWindow = qobject_cast<LipstickCompositorWindow *>(windowForId(topmostWindowId()));
            if (topmostWindow != 0 && topmostWindow->hasFocus()) {
                m_onUpdatesDisabledUnfocusedWindowId = topmostWindow->windowId();
                clearKeyboardFocus();
            }
            hide();
            QGuiApplication::platformNativeInterface()->nativeResourceForIntegration("DisplayOff");
        } else {
            QGuiApplication::platformNativeInterface()->nativeResourceForIntegration("DisplayOn");
            emit displayAboutToBeOn();
            showFullScreen();
            if (m_onUpdatesDisabledUnfocusedWindowId > 0) {
                if (!LipstickSettings::instance()->lockscreenVisible()) {
                    LipstickCompositorWindow *topmostWindow = qobject_cast<LipstickCompositorWindow *>(windowForId(topmostWindowId()));
                    if (topmostWindow != 0 && topmostWindow->windowId() == m_onUpdatesDisabledUnfocusedWindowId) {
                        topmostWindow->takeFocus();
                    }
                }
                m_onUpdatesDisabledUnfocusedWindowId = 0;
            }
        }
    }
}
