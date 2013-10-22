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

#include <QCoreApplication>
#include <QWaylandCompositor>
#include <QWaylandInputDevice>
#include <QTimer>
#include <sys/types.h>
#include <signal.h>
#include "lipstickcompositor.h"
#include "lipstickcompositorwindow.h"

LipstickCompositorWindow::LipstickCompositorWindow(int windowId, const QString &category,
                                                   QWaylandSurface *surface, QQuickItem *parent)
: QWaylandSurfaceItem(surface, parent), m_windowId(windowId), m_category(category), m_ref(0),
  m_delayRemove(false), m_windowClosed(false), m_removePosted(false), m_mouseRegionValid(false)
{
    setFlags(QQuickItem::ItemIsFocusScope | flags());
    refreshMouseRegion();

    // Handle ungrab situations
    connect(this, SIGNAL(visibleChanged()), SLOT(handleTouchCancel()));
    connect(this, SIGNAL(enabledChanged()), SLOT(handleTouchCancel()));
    connect(this, SIGNAL(touchEventsEnabledChanged()), SLOT(handleTouchCancel()));

    connect(this, SIGNAL(surfaceChanged()), SLOT(connectSurfaceSignals()));
    connectSurfaceSignals();
}

QVariant LipstickCompositorWindow::userData() const
{
    return m_data;
}

void LipstickCompositorWindow::setUserData(QVariant data)
{
    if (m_data == data)
        return;

    m_data = data;
    emit userDataChanged();
}

int LipstickCompositorWindow::windowId() const
{
    return m_windowId;
}

qint64 LipstickCompositorWindow::processId() const
{
    if (surface()) return surface()->processId();
    else return 0;
}

bool LipstickCompositorWindow::delayRemove() const
{
    return m_delayRemove;
}

void LipstickCompositorWindow::setDelayRemove(bool delay)
{
    if (delay == m_delayRemove)
        return;

    m_delayRemove = delay;
    emit delayRemoveChanged();

    tryRemove();
}

QString LipstickCompositorWindow::category() const
{
    return m_category;
}

QString LipstickCompositorWindow::title() const
{
    if (surface())
        return surface()->title();
    return QString();
}

void LipstickCompositorWindow::imageAddref()
{
    ++m_ref;
}

void LipstickCompositorWindow::imageRelease()
{
    Q_ASSERT(m_ref);
    --m_ref;
    tryRemove();
}

bool LipstickCompositorWindow::canRemove() const
{
    return m_windowClosed && !m_delayRemove && m_ref == 0;
}

void LipstickCompositorWindow::tryRemove()
{
    if (canRemove() && !m_removePosted) {
        m_removePosted = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::User));
    }
}

QRect LipstickCompositorWindow::mouseRegionBounds() const
{
    if (m_mouseRegionValid)
        return m_mouseRegion.boundingRect();
    else
        return QRect(0, 0, width(), height());
}

void LipstickCompositorWindow::refreshMouseRegion()
{
    QWaylandSurface *s = surface();
    if (s) {
        QVariantMap properties = s->windowProperties();
        if (properties.contains(QLatin1String("MOUSE_REGION"))) {
            m_mouseRegion = s->windowProperties().value("MOUSE_REGION").value<QRegion>();
            m_mouseRegionValid = true;
            if (LipstickCompositor::instance()->debug())
                qDebug() << "Window" << windowId() << "mouse region set:" << m_mouseRegion;
        } else {
            m_mouseRegionValid = false;
            if (LipstickCompositor::instance()->debug())
                qDebug() << "Window" << windowId() << "mouse region cleared";
        }

        emit mouseRegionBoundsChanged();
    }
}

void LipstickCompositorWindow::refreshGrabbedKeys()
{
    QWaylandSurface *s = surface();
    if (s) {
        const QStringList grabbedKeys = s->windowProperties().value(
                    QLatin1String("GRABBED_KEYS")).value<QStringList>();

        if (m_grabbedKeys.isEmpty() && !grabbedKeys.isEmpty()) {
            qApp->installEventFilter(this);
        } else if (!m_grabbedKeys.isEmpty() && grabbedKeys.isEmpty()) {
            qApp->removeEventFilter(this);
        }

        m_grabbedKeys.clear();
        foreach (const QString &key, grabbedKeys)
            m_grabbedKeys.append(key.toInt());

        if (LipstickCompositor::instance()->debug())
            qDebug() << "Window" << windowId() << "grabbed keys changed:" << grabbedKeys;
    }
}

bool LipstickCompositorWindow::eventFilter(QObject *, QEvent *event)
{
    if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        QWaylandSurface *m_surface = surface();
        if (m_surface && m_grabbedKeys.contains(ke->key())) {
            QWaylandInputDevice *inputDevice = m_surface->compositor()->defaultInputDevice();
            inputDevice->sendFullKeyEvent(m_surface, ke);

            return true;
        }
    }
    return false;
}

bool LipstickCompositorWindow::isInProcess() const
{
    return false;
}

bool LipstickCompositorWindow::event(QEvent *e)
{
    bool rv = QWaylandSurfaceItem::event(e);
    if (e->type() == QEvent::User) {
        m_removePosted = false;
        if (canRemove()) delete this;
    }
    return rv;
}

void LipstickCompositorWindow::mousePressEvent(QMouseEvent *event)
{
    QWaylandSurface *m_surface = surface();
    if (m_surface && (!m_mouseRegionValid || m_mouseRegion.contains(event->pos()))) {
        QWaylandInputDevice *inputDevice = m_surface->compositor()->defaultInputDevice();
        if (inputDevice->mouseFocus() != m_surface)
            inputDevice->setMouseFocus(m_surface, event->pos(), event->globalPos());
        inputDevice->sendMousePressEvent(event->button(), event->pos(), event->globalPos());
    } else {
        event->ignore();
    }
}

void LipstickCompositorWindow::mouseMoveEvent(QMouseEvent *event)
{
    QWaylandSurface *m_surface = surface();
    if (m_surface){
        QWaylandInputDevice *inputDevice = m_surface->compositor()->defaultInputDevice();
        inputDevice->sendMouseMoveEvent(m_surface, event->pos(), event->globalPos());
    } else {
        event->ignore();
    }
}

void LipstickCompositorWindow::mouseReleaseEvent(QMouseEvent *event)
{
    QWaylandSurface *m_surface = surface();
    if (m_surface){
        QWaylandInputDevice *inputDevice = m_surface->compositor()->defaultInputDevice();
        inputDevice->sendMouseReleaseEvent(event->button(), event->pos(), event->globalPos());
    } else {
        event->ignore();
    }
}

void LipstickCompositorWindow::wheelEvent(QWheelEvent *event)
{
    QWaylandSurface *m_surface = surface();
    if (m_surface) {
        QWaylandInputDevice *inputDevice = m_surface->compositor()->defaultInputDevice();
        inputDevice->sendMouseWheelEvent(event->orientation(), event->delta());
    } else {
        event->ignore();
    }
}

void LipstickCompositorWindow::touchEvent(QTouchEvent *event)
{
    QWaylandSurface *m_surface = surface();
    if (touchEventsEnabled() && m_surface) {
        QList<QTouchEvent::TouchPoint> points = event->touchPoints();

        if (m_mouseRegionValid && points.count() == 1 &&
            event->touchPointStates() & Qt::TouchPointPressed &&
            !m_mouseRegion.contains(points.at(0).pos().toPoint())) {
            event->ignore();
            return;
        }

        QWaylandInputDevice *inputDevice = m_surface->compositor()->defaultInputDevice();
        event->accept();
        if (inputDevice->mouseFocus() != m_surface) {
            QPoint pointPos;
            if (!points.isEmpty())
                pointPos = points.at(0).pos().toPoint();
            inputDevice->setMouseFocus(m_surface, pointPos, pointPos);
        }
        inputDevice->sendFullTouchEvent(event);
    } else {
        event->ignore();
    }
}

void LipstickCompositorWindow::handleTouchCancel()
{
    QWaylandSurface *m_surface = surface();
    if (!m_surface)
        return;
    QWaylandInputDevice *inputDevice = m_surface->compositor()->defaultInputDevice();
    if (inputDevice->mouseFocus() == m_surface &&
            (!isVisible() || !isEnabled() || !touchEventsEnabled())) {
        inputDevice->sendTouchCancelEvent();
        inputDevice->setMouseFocus(0, QPointF());
    }
}

void LipstickCompositorWindow::terminateProcess(int killTimeout)
{
    kill(processId(), SIGTERM);

    QTimer::singleShot(killTimeout, this, SLOT(killProcess()));
}

void LipstickCompositorWindow::killProcess()
{
    kill(processId(), SIGKILL);
}

void LipstickCompositorWindow::connectSurfaceSignals()
{
    foreach (const QMetaObject::Connection &connection, m_surfaceConnections) {
        disconnect(connection);
    }

    m_surfaceConnections.clear();
    if (surface()) {
        m_surfaceConnections << connect(surface(), SIGNAL(titleChanged()), SIGNAL(titleChanged()));
    }
}
