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

#include <sys/time.h>

#include <QMutexLocker>

#include "lipstickrecorder.h"
#include "lipstickcompositor.h"

static uint32_t getTime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

static const QEvent::Type FrameEventType = (QEvent::Type)QEvent::registerEventType();
static const QEvent::Type FailedEventType = (QEvent::Type)QEvent::registerEventType();

class FrameEvent : public QEvent
{
public:
    FrameEvent(uint32_t t)
        : QEvent(FrameEventType)
        , time(t)
    { }
    uint32_t time;
};

class FailedEvent : public QEvent
{
public:
    FailedEvent(int r)
        : QEvent(FailedEventType)
        , result(r)
    { }
    int result;
};

LipstickRecorderManager::LipstickRecorderManager()
                       : QWaylandGlobalInterface()
{
}

const wl_interface* LipstickRecorderManager::interface() const
{
    return &lipstick_recorder_manager_interface;
}

void LipstickRecorderManager::recordFrame(QWindow *window)
{
    QMutexLocker lock(&m_mutex);
    if (m_requests.isEmpty())
        return;

    uchar *pixels;
    uint32_t time = getTime();
    foreach (LipstickRecorder *recorder, m_requests.values(window)) {
        wl_shm_buffer *buffer = recorder->buffer();
        pixels = static_cast<uchar *>(wl_shm_buffer_get_data(buffer));
        int width = wl_shm_buffer_get_width(buffer);
        int height = wl_shm_buffer_get_height(buffer);

        if (width != window->width() || height != window->height()) {
            qApp->postEvent(recorder, new FailedEvent(QtWaylandServer::lipstick_recorder::result_bad_buffer));
            continue;
        }

        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        qApp->postEvent(recorder, new FrameEvent(time));

        m_requests.remove(window, recorder);
    }
}

void LipstickRecorderManager::requestFrame(QWindow *window, LipstickRecorder *recorder)
{
    QMutexLocker lock(&m_mutex);
    m_requests.insert(window, recorder);
}

void LipstickRecorderManager::remove(QWindow *window, LipstickRecorder *recorder)
{
    QMutexLocker lock(&m_mutex);
    m_requests.remove(window, recorder);
}

void LipstickRecorderManager::bind(wl_client *client, quint32 version, quint32 id)
{
    Q_UNUSED(version)

    add(client, id, version);
}

void LipstickRecorderManager::lipstick_recorder_manager_create_recorder(Resource *resource, uint32_t id, ::wl_resource *output)
{
    // TODO: we should find out the window associated with this output, but there isn't
    // a way to do that in qtcompositor yet. Just ignore it for now and use the one window we have.
    Q_UNUSED(output)

    new LipstickRecorder(this, resource->client(), id, LipstickCompositor::instance());
}


LipstickRecorder::LipstickRecorder(LipstickRecorderManager *manager, wl_client *client, quint32 id, QQuickWindow *window)
                : QtWaylandServer::lipstick_recorder(client, id, 1)
                , m_manager(manager)
                , m_bufferResource(Q_NULLPTR)
                , m_client(client)
                , m_window(window)
{
}

LipstickRecorder::~LipstickRecorder()
{
    m_manager->remove(m_window, this);
}

void LipstickRecorder::lipstick_recorder_destroy_resource(Resource *resource)
{
    Q_UNUSED(resource)
    delete this;
}

void LipstickRecorder::lipstick_recorder_destroy(Resource *resource)
{
    wl_resource_destroy(resource->handle);
}

void LipstickRecorder::lipstick_recorder_record_frame(Resource *resource, ::wl_resource *buffer)
{
    Q_UNUSED(resource)
    if (m_bufferResource) {
        send_cancelled(buffer);
    }
    m_bufferResource = buffer;
    m_buffer = wl_shm_buffer_get(buffer);
    if (m_buffer) {
        m_manager->requestFrame(m_window, this);
    } else {
        m_bufferResource = Q_NULLPTR;
        send_failed(result_bad_buffer, buffer);
    }
}

void LipstickRecorder::lipstick_recorder_repaint(Resource *resource)
{
    Q_UNUSED(resource)
    if (m_bufferResource) {
        m_window->update();
    }
}

bool LipstickRecorder::event(QEvent *e)
{
    if (e->type() == FrameEventType) {
        FrameEvent *fe = static_cast<FrameEvent *>(e);
        send_frame(m_bufferResource, fe->time, QtWaylandServer::lipstick_recorder::transform_y_inverted);
    } else if (e->type() == FailedEventType) {
        FailedEvent *fe = static_cast<FailedEvent *>(e);
        send_failed(fe->result, m_bufferResource);
    } else {
        return QObject::event(e);
    }

    m_bufferResource = Q_NULLPTR;
    wl_client_flush(client());
    return true;
}
