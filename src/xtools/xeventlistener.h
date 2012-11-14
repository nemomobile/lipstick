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

#ifndef XEVENTLISTENER_H_
#define XEVENTLISTENER_H_

#include <X11/Xlib.h>
#include "lipstickglobal.h"

/*!
 * An interface for listening to X events.
 * Objects of this class receive X events throughout their lifecycle as the events
 * arrive.
 */
class LIPSTICK_EXPORT XEventListener
{
public:
    /*!
     * Constructs a new listener and starts receiving X events.
     */
    XEventListener();

    /*!
     * Destructor.
     */
    virtual ~XEventListener();

    /*!
     * A handler method for X events.
     * Return \c true if the event got handled and \c false if not.
     * \param event the X event
     * \return \c true if the event got handled
     */
    virtual bool handleXEvent(const XEvent &event) = 0;
};

#endif /* X11EVENTLISTENER_H_ */
