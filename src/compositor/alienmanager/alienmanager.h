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

#ifndef ALIENMANAGER_H
#define ALIENMANAGER_H

#include <QObject>
#include <QtCompositor/qwaylandglobalinterface.h>

#include "qwayland-server-alien-manager.h"

class AlienClient;
class AlienSurface;

class AlienManagerGlobal : public QObject, public QWaylandGlobalInterface
{
public:
    explicit AlienManagerGlobal(QObject *parent = nullptr);

    const wl_interface *interface() const Q_DECL_OVERRIDE;
    void bind(wl_client *client, uint32_t version, uint32_t id) Q_DECL_OVERRIDE;
};

class AlienManager : public QObject, public QtWaylandServer::alien_manager
{
public:
    AlienManager(wl_client *client, uint32_t version, uint32_t id, QObject *parent);
    ~AlienManager();

    void ping(uint32_t serial, QWaylandSurface *surface);

protected:
    void alien_manager_destroy_resource(Resource *resource) Q_DECL_OVERRIDE;
    void alien_manager_destroy(Resource *resource) Q_DECL_OVERRIDE;
    void alien_manager_create_alien_client(Resource *resource, uint32_t id, const QString &package) Q_DECL_OVERRIDE;
    void alien_manager_pong(Resource *resource, uint32_t serial) Q_DECL_OVERRIDE;

private:
    QMap<uint32_t, QWaylandSurface *> m_pings;
};

class AlienClient : public QObject, public QtWaylandServer::alien_client
{
public:
    AlienClient(AlienManager *mgr, wl_client *client, uint32_t version, uint32_t id, const QString &package);
    ~AlienClient();

    AlienManager *manager() const;

protected:
    void alien_client_destroy_resource(Resource *resource) Q_DECL_OVERRIDE;
    void alien_client_destroy(Resource *resource) Q_DECL_OVERRIDE;
    void alien_client_get_alien_surface(Resource *resource, uint32_t id, ::wl_resource *surface) Q_DECL_OVERRIDE;

private:
    QString m_package;
    AlienManager *m_manager;
};

#endif
