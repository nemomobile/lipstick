/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** Copyright (C) 2012 Jolla Ltd.
** Contact: Robin Burchell <robin.burchell@jollamobile.com>
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

#ifndef EVENTEATER_H
#define EVENTEATER_H

#include <QObject>
#include <QAbstractEventDispatcher>
#include <QX11Info>
#include "xtools/xeventlistener.h"
#include "xtools/x11wrapper.h"

/*!
 * An EventEater window interrupts X pointer and key press events. On event received EventEater hides its window.
 */
class EventEater : public QObject, XEventListener {
    Q_OBJECT
public:

    //! Creates a new EventEater instance and its window.
    EventEater(QObject *parent = 0);

    //! Destroys the EventEater and its window.
    virtual ~EventEater();

    /*! Show the event eater window */
    void show();

    /*! Hide the event eater window */
    void hide();

    //! \reimp
    virtual bool handleXEvent(const XEvent &event);
    //! \reimp_end

private:
    Window window;

signals:
    //! Emitted when input event is received for the window.
    void inputEventReceived();

#ifdef UNIT_TEST
    friend class Ut_EventEater;
#endif
};

#endif
