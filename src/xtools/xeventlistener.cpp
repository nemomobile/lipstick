/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include "homeapplication.h"
#include "xeventlistener.h"

QList<XEventListener *> xEventListeners;

static bool processXEventListeners(XEvent *event)
{
    foreach (XEventListener* listener, xEventListeners)
        if (listener->handleXEvent(*event))
            return true;

    return false;
}

// technically private API, but this is the easiest way to get to it, and we can deal with it changing
typedef bool(*QX11FilterFunction)(XEvent *event);
extern void qt_installX11EventFilter(QX11FilterFunction func);

XEventListener::XEventListener()
{
    static bool initialized = false;
    if (!initialized) {
        qt_installX11EventFilter(processXEventListeners);
        initialized = true;
    }

    xEventListeners.append(this);
}

XEventListener::~XEventListener()
{
    xEventListeners.removeOne(this);
}
