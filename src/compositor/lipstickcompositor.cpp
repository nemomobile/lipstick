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
#include "homeapplication.h"
#include "windowmodel.h"
#include "lipstickcompositorprocwindow.h"
#include "lipstickcompositor.h"
#include <qpa/qwindowsysteminterface.h>

LipstickCompositor *LipstickCompositor::m_instance = 0;

LipstickCompositor::LipstickCompositor()
: QWaylandCompositor(this), m_totalWindowCount(0), m_nextWindowId(1), m_homeActive(true), m_shaderEffect(0),
  m_fullscreenSurface(0), m_directRenderingActive(false), m_topmostWindowId(0), m_screenOrientation(Qt::PrimaryOrientation), m_sensorOrientation(Qt::PrimaryOrientation), m_displayState(new MeeGo::QmDisplayState(this)), m_retainedSelection(0)
{
    setColor(Qt::black);
    setRetainedSelectionEnabled(true);

    if (m_instance) qFatal("LipstickCompositor: Only one compositor instance per process is supported");
    m_instance = this;

    QObject::connect(this, SIGNAL(frameSwapped()), this, SLOT(windowSwapped()));
    QObject::connect(this, SIGNAL(beforeSynchronizing()), this, SLOT(clearUpdateRequest()));
    connect(m_displayState, SIGNAL(displayStateChanged(MeeGo::QmDisplayState::DisplayState)), this, SLOT(reactOnDisplayStateChanges(MeeGo::QmDisplayState::DisplayState)));
    QObject::connect(HomeApplication::instance(), SIGNAL(aboutToDestroy()), this, SLOT(homeApplicationAboutToDestroy()));

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
}

LipstickCompositor::~LipstickCompositor()
{
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
    connect(surface, SIGNAL(damaged(QRect)), this, SLOT(surfaceDamaged(QRect)));
}

void LipstickCompositor::openUrl(WaylandClient *client, const QUrl &url)
{
    Q_UNUSED(client)
    openUrl(url);
}

void LipstickCompositor::openUrl(const QUrl &url)
{
#if defined(HAVE_CONTENTACTION)
    if (url.scheme() == "file") {
        ContentAction::Action::defaultActionForFile(url.toString()).trigger();
    } else {
        ContentAction::Action::defaultActionForScheme(url.toString()).trigger();
    }
#else
    Q_UNUSED(url)
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

void LipstickCompositor::surfaceDamaged(const QRect &)
{
    if (!isVisible()) {
        // If the compositor is not visible, do not throttle.
        // make it conditional to QT_WAYLAND_COMPOSITOR_NO_THROTTLE?
        frameFinished(0);
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

    const bool directRenderingSucceeded = setDirectRenderSurface(m_fullscreenSurface, openglContext());
    if (surface && !directRenderingSucceeded)
        qWarning() << Q_FUNC_INFO << "failed to set direct render surface";
    if ((surface && directRenderingSucceeded) != m_directRenderingActive) {
        m_directRenderingActive = surface && directRenderingSucceeded;
        emit directRenderingActiveChanged();
    }

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

void LipstickCompositor::clearUpdateRequest()
{
    // Called from render thread
    m_updateRequestPosted.store(0);
}

void LipstickCompositor::maybePostUpdateRequest()
{
    // Called from GUI thread
    if (m_updateRequestPosted.testAndSetOrdered(0, 1))
        qApp->postEvent(this, new QEvent(QEvent::User));
}

void LipstickCompositor::surfaceMapped()
{
    QWaylandSurface *surface = qobject_cast<QWaylandSurface *>(sender());
    //Ignore surface if it's not a window surface
    if (!surface->hasShellSurface())
        return;

    QVariantMap properties = surface->windowProperties();
    QString category = properties.value("CATEGORY").toString();

    if (surface->surfaceItem()) {
        // Always cause a repaint on surface mapped
        maybePostUpdateRequest();
        return;
    }

    // The surface was mapped for the first time
    int id = m_nextWindowId++;
    LipstickCompositorWindow *item = new LipstickCompositorWindow(id, category, surface, contentItem());
    item->setSize(surface->size());
    QObject::connect(item, SIGNAL(destroyed(QObject*)), this, SLOT(windowDestroyed()));

    // Whenever the item is damaged, cause a full repaint
    QObject::connect(item, SIGNAL(textureChanged()), this, SLOT(maybePostUpdateRequest()));
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

void LipstickCompositor::windowSwapped()
{
    frameFinished(m_fullscreenSurface);
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
    if (window) {
        emit windowHidden(window);

        // Always schedule an update
        maybePostUpdateRequest();
    }
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

bool LipstickCompositor::event(QEvent *e)
{
    // Update will eventually trigger a beforeSynchronizing signal,
    // clear the m_updateRequest there (what happens after synchronizing,
    // needs to be updated)
    if (e->type() == QEvent::User)
        update();

    return QQuickWindow::event(e);
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
    if (state == MeeGo::QmDisplayState::On) {
        emit displayOn();
    } else if (state == MeeGo::QmDisplayState::Off) {
        emit displayOff();
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
