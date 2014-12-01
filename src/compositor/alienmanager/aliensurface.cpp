/***************************************************************************
**
** Copyright (C) 2014 Jolla Ltd.
** Contact: Giulio Camuffo <giulio.camuffo@jollamobile.com>
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

#include <QtCompositor/QWaylandSurface>

#include "lipstickcompositor.h"
#include "aliensurface.h"
#include "alienmanager.h"
#include "lipsticksurfaceinterface.h"

AlienSurface::AlienSurface(AlienClient *client, QWaylandSurface *surface, uint32_t version, uint32_t id, const QString &package)
            : QObject(client)
            , QWaylandSurfaceInterface(surface)
            , QtWaylandServer::alien_surface(reinterpret_cast<wl_client *>(surface->client()), id, version)
            , m_client(client)
            , m_hidden(false)
            , m_coverized(false)
            , m_lastSize(0, 0)
            , m_serial(0)
{
    LipstickCompositor *compositor = LipstickCompositor::instance();
    sendConfigure(compositor->width(), compositor->height());
    connect(surface, &QWaylandSurface::configure, this, &AlienSurface::configure);
    setSurfaceType(QWaylandSurface::Toplevel);
    setSurfaceClassName(package);
}

AlienSurface::~AlienSurface()
{
    wl_resource_set_implementation(resource()->handle, nullptr, nullptr, nullptr);
    surface()->setMapped(false);
}

bool AlienSurface::runOperation(QWaylandSurfaceOp *op)
{
    switch (op->type()) {
        case QWaylandSurfaceOp::Close:
            send_close();
            return true;
        case QWaylandSurfaceOp::SetVisibility: {
            QWindow::Visibility v = static_cast<QWaylandSurfaceSetVisibilityOp *>(op)->visibility();
            m_hidden = v == QWindow::Hidden;
            m_coverized = v == QWindow::Minimized;
            updateStates();
            return true;
        }
        case QWaylandSurfaceOp::Ping:
            m_client->manager()->ping(static_cast<QWaylandSurfacePingOp *>(op)->serial(), surface());
            return true;
        case QWaylandSurfaceOp::Resize: {
            QSize s = static_cast<QWaylandSurfaceResizeOp *>(op)->size();
            sendConfigure(s.width(), s.height());
            return true;
        }
        case LipstickOomScoreOp::Type:
            m_client->send_oom_score(static_cast<LipstickOomScoreOp *>(op)->score());
            return true;
        default:
            break;
    }
    return false;
}

void AlienSurface::alien_surface_destroy_resource(Resource *resource)
{
    Q_UNUSED(resource)
    delete this;
}

void AlienSurface::alien_surface_destroy(Resource *resource)
{
    wl_resource_destroy(resource->handle);
}

void AlienSurface::alien_surface_set_title(Resource *resource, const QString &title)
{
    Q_UNUSED(resource)
    setSurfaceTitle(title);
}

void AlienSurface::alien_surface_ack_configure(Resource *resource, uint32_t serial)
{
    Q_UNUSED(resource);
    if (serial == m_serial) {
        m_serial = 0;
    }
}

void AlienSurface::alien_surface_request_state(Resource *resource, wl_array *array, uint32_t serial)
{
    Q_UNUSED(resource);
    bool wasHiddenOrCover = m_hidden | m_coverized;
    m_hidden = false;

    for (uint32_t *ptr = (uint32_t *)array->data; (char *)ptr < (char *)array->data + array->size; ++ptr) {
        if (*ptr == ALIEN_SURFACE_STATE_HIDDEN) {
            m_hidden = true;
        }
    }
    // do some focus stealing prevenction
    if (wasHiddenOrCover && !m_hidden && serial == wl_display_get_serial(surface()->compositor()->waylandDisplay())) {
        emit surface()->raiseRequested();
    } else {
        updateStates();
    }
}

void AlienSurface::alien_surface_set_minimized(Resource *resource)
{
    Q_UNUSED(resource)
    emit surface()->lowerRequested();
}

void AlienSurface::configure(bool hasBuffer)
{
    if (hasBuffer && m_serial == 0) {
        m_lastSize = surface()->size();
    }
    surface()->setMapped(hasBuffer);
}

void AlienSurface::sendConfigure(int w, int h)
{
    QVector<uint32_t> states;
    if (m_hidden) {
        states << ALIEN_SURFACE_STATE_HIDDEN;
    }
    if (m_coverized) {
        states << ALIEN_SURFACE_STATE_COVER;
    }
    QByteArray data = QByteArray::fromRawData((char *)states.data(), states.size() * sizeof(uint32_t));
    m_serial = wl_display_next_serial(surface()->compositor()->waylandDisplay());

    send_configure(w, h, data, m_serial);
    m_lastSize = QSize(w, h);
}

void AlienSurface::updateStates()
{
    sendConfigure(m_lastSize.width(), m_lastSize.height());
}
