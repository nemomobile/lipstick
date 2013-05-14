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

#include "windowproperty.h"

#include "lipstickcompositor.h"

WindowProperty::WindowProperty()
: m_windowId(0), m_waitingRefProperty(false)
{
    LipstickCompositor *c = LipstickCompositor::instance();
    if (!c)
        qWarning("WindowProperty: Compositor must be created before WindowProperty");
}

int WindowProperty::windowId() const
{
    return m_windowId;
}

void WindowProperty::setWindowId(int window)
{
    if (m_windowId == window)
        return;

    m_windowId = window;

    if (m_surface) {
        QObject::disconnect(m_surface, SIGNAL(windowPropertyChanged(QString,QVariant)), 
                            this, SLOT(windowPropertyChanged(QString)));
        QObject::disconnect(m_surface, SIGNAL(destroyed(QObject *)),
                            this, SIGNAL(valueChanged()));
        m_surface = 0;
    }

    LipstickCompositor *c = LipstickCompositor::instance();
    if (c) m_surface = c->surfaceForId(window);

    if (m_surface) {
        QObject::connect(m_surface, SIGNAL(windowPropertyChanged(QString,QVariant)), 
                         this, SLOT(windowPropertyChanged(QString)));
        QObject::connect(m_surface, SIGNAL(destroyed(QObject *)),
                         this, SIGNAL(valueChanged()));
    }
    
    disconnectRef();

    emit windowIdChanged();
    emit valueChanged();
}

void WindowProperty::windowPropertyChanged(const QString &name)
{
    if (name == m_property)
        emit valueChanged();
}

QString WindowProperty::property() const
{
    return m_property;
}

void WindowProperty::setProperty(const QString &p)
{
    if (m_property == p)
        return;
    
    m_property = p;
    emit propertyChanged();
    emit valueChanged();
}

void WindowProperty::connectRef()
{
    LipstickCompositor *c = LipstickCompositor::instance();
    if (c && !m_waitingRefProperty) {
        m_waitingRefProperty = true;
        QObject::connect(c, SIGNAL(availableWinIdsChanged()), this, SLOT(availableWinIdsChanged()));
    }
}

void WindowProperty::disconnectRef()
{
    LipstickCompositor *c = LipstickCompositor::instance();
    if (c && m_waitingRefProperty) {
        m_waitingRefProperty = false;
        QObject::disconnect(c, SIGNAL(availableWinIdsChanged()), this, SLOT(availableWinIdsChanged()));
    }
}

QVariant WindowProperty::value()
{
    if (!m_surface)
        return QVariant();


    QVariant rv = m_surface->windowProperties().value(m_property);
    if (rv.type() == QVariant::String && rv.toString().startsWith("__winref:")) {
        QString refId = rv.toString().mid(9);
        uint id = refId.toUInt();

        if (id) {
            int win = LipstickCompositor::instance()->windowIdForLink(m_surface, id);
            if (win == 0)
                connectRef();
            else
                disconnectRef();

            return QVariant(win);
        } else {
            disconnectRef();

            return QVariant(0);
        }
    } else {
        disconnectRef();

        return rv;
    }
}

void WindowProperty::availableWinIdsChanged()
{
    if (value().isValid())
        emit valueChanged();
}
