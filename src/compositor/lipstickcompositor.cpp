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
#include "homeapplication.h"
#include "windowmodel.h"
#include "lipstickcompositorprocwindow.h"
#include "lipstickcompositor.h"

LipstickCompositor *LipstickCompositor::m_instance = 0;

LipstickCompositor::LipstickCompositor()
: QWaylandCompositor(this), m_totalWindowCount(0), m_nextWindowId(1), m_homeActive(true), m_shaderEffect(0),
  m_fullscreenSurface(0), m_directRenderingActive(false), m_topmostWindowId(0), m_screenOrientation(Qt::PrimaryOrientation), m_displayState(new MeeGo::QmDisplayState(this))
{
    setColor(Qt::black);

    if (m_instance) qFatal("LipstickCompositor: Only one compositor instance per process is supported");
    m_instance = this;

    QObject::connect(this, SIGNAL(frameSwapped()), this, SLOT(windowSwapped()));
    connect(m_displayState, SIGNAL(displayStateChanged(MeeGo::QmDisplayState::DisplayState)), this, SLOT(reactOnDisplayStateChanges(MeeGo::QmDisplayState::DisplayState)));

    emit HomeApplication::instance()->homeActiveChanged();

    QDesktopServices::setUrlHandler("http", this, "openUrl");
    QDesktopServices::setUrlHandler("https", this, "openUrl");
    QDesktopServices::setUrlHandler("mailto", this, "openUrl");
}

LipstickCompositor::~LipstickCompositor()
{
    delete m_shaderEffect;
}

LipstickCompositor *LipstickCompositor::instance()
{
    return m_instance;
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
}

void LipstickCompositor::openUrl(WaylandClient *client, const QUrl &url)
{
    Q_UNUSED(client)
    openUrl(url);
}

void LipstickCompositor::openUrl(const QUrl &url)
{
#if defined(HAVE_CONTENTACTION)
    ContentAction::Action::defaultActionForScheme(url.toString()).trigger();
#else
    Q_UNUSED(url)
#endif
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

void LipstickCompositor::displayOff()
{
    m_displayState->set(MeeGo::QmDisplayState::Off);
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
        QWaylandCompositor::setScreenOrientation(screenOrientation);

        m_screenOrientation = screenOrientation;
        emit screenOrientationChanged();
    }
}

void LipstickCompositor::reactOnDisplayStateChanges(MeeGo::QmDisplayState::DisplayState state)
{
    if (state == MeeGo::QmDisplayState::On) {
        emit displayIsOn();
    }
}
