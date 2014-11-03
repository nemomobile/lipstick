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

#ifndef LIPSTICKCOMPOSITORRECORDER_H
#define LIPSTICKCOMPOSITORRECORDER_H

#include <QObject>
#include <QMultiHash>
#include <QMutex>
#include <QWaylandGlobalInterface>

#include "qwayland-server-lipstick-recorder.h"

struct wl_shm_buffer;
struct wl_client;

class QWindow;
class QEvent;
class LipstickRecorder;

class LipstickRecorderManager : public QWaylandGlobalInterface, public QtWaylandServer::lipstick_recorder_manager
{
public:
    LipstickRecorderManager();

    const wl_interface* interface() const Q_DECL_OVERRIDE;

    void recordFrame(QWindow *window);
    void requestFrame(QWindow *window, LipstickRecorder *recorder);
    void remove(QWindow *window, LipstickRecorder *recorder);

protected:
    void bind(wl_client *client, quint32 version, quint32 id) Q_DECL_OVERRIDE;
    void lipstick_recorder_manager_create_recorder(Resource *resource, uint32_t id, ::wl_resource *output) Q_DECL_OVERRIDE;

private:
    QMultiHash<QWindow *, LipstickRecorder *> m_requests;
    QMutex m_mutex;
};

class LipstickRecorder : public QObject, public QtWaylandServer::lipstick_recorder
{
public:
    LipstickRecorder(LipstickRecorderManager *manager, wl_client *client, quint32 id, QWindow *window);
    ~LipstickRecorder();

    void sendFrame(int result, int time);
    wl_shm_buffer *buffer() const { return m_buffer; }
    wl_client *client() const { return m_client; }

protected:
    bool event(QEvent *e) Q_DECL_OVERRIDE;
    void lipstick_recorder_destroy_resource(Resource *resource) Q_DECL_OVERRIDE;
    void lipstick_recorder_destroy(Resource *resource) Q_DECL_OVERRIDE;
    void lipstick_recorder_record_frame(Resource *resource, ::wl_resource *buffer) Q_DECL_OVERRIDE;

private:
    LipstickRecorderManager *m_manager;
    wl_resource *m_bufferResource;
    wl_shm_buffer *m_buffer;
    wl_client *m_client;
    QWindow *m_window;
};

#endif
