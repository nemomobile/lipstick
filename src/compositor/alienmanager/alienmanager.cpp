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

#include "alienmanager.h"
#include "aliensurface.h"

AlienManagerGlobal::AlienManagerGlobal(QObject *parent)
                  : QObject(parent)
{

}

const wl_interface *AlienManagerGlobal::interface() const
{
    return &alien_manager_interface;
}

void AlienManagerGlobal::bind(wl_client *client, uint32_t version, uint32_t id)
{
    new AlienManager(client, version, id, this);
}



AlienManager::AlienManager(wl_client *client, uint32_t version, uint32_t id, QObject *parent)
            : QObject(parent)
            , QtWaylandServer::alien_manager(client, id, version)
{
}

void AlienManager::ping(uint32_t serial, QWaylandSurface *surface)
{
    m_pings.insert(serial, surface);
    send_ping(serial);
}

void AlienManager::alien_manager_destroy_resource(Resource *resource)
{
    Q_UNUSED(resource)
    delete this;
}

void AlienManager::alien_manager_destroy(Resource *resource)
{
    wl_resource_destroy(resource->handle);
}

void AlienManager::alien_manager_create_alien_client(Resource *resource, uint32_t id, const QString &package)
{
    new AlienClient(this, resource->client(), wl_resource_get_version(resource->handle), id, package);
}

void AlienManager::alien_manager_pong(Resource *resource, uint32_t serial)
{
    Q_UNUSED(resource)
    QWaylandSurface *surf = m_pings.value(serial);
    if (surf)
        surf->pong();
}


AlienClient::AlienClient(AlienManager *mgr, wl_client *client, uint32_t version, uint32_t id, const QString &package)
           : QObject(mgr)
           , QtWaylandServer::alien_client(client, id, version)
           , m_package(package)
           , m_manager(mgr)
{
}

AlienManager *AlienClient::manager() const
{
    return m_manager;
}

void AlienClient::alien_client_destroy_resource(Resource *resource)
{
    Q_UNUSED(resource)
    delete this;
}

void AlienClient::alien_client_destroy(Resource *resource)
{
    wl_resource_destroy(resource->handle);
}

void AlienClient::alien_client_get_alien_surface(Resource *resource, uint32_t id, ::wl_resource *surface)
{
    QWaylandSurface *surf = QWaylandSurface::fromResource(surface);
    new AlienSurface(this, surf, wl_resource_get_version(resource->handle), id, m_package);
}
