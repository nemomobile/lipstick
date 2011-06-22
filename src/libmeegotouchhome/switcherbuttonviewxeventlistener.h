/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of mhome.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#ifndef SWITCHERBUTTONVIEWXEVENTLISTENER_H
#define SWITCHERBUTTONVIEWXEVENTLISTENER_H

#include "xeventlistener.h"
#include "switcherbuttonview.h"

/*!
 * An X event listener for the switcher button view. Reacts to visibility
 * notify events.
 */
class SwitcherButtonViewXEventListener : public XEventListener
{
    //! The SwitcherButtonView object that owns this listener
    SwitcherButtonView &parent;

public:
    /*!
     * Constructs a new listener for switcher button view.
     * \param parent the owner of this object.
     */
    SwitcherButtonViewXEventListener(SwitcherButtonView &parent) :
            parent(parent)
    {
    }

    virtual ~SwitcherButtonViewXEventListener()
    {
    }

    //! \reimp
    virtual bool handleXEvent(const XEvent &event)
    {
        bool handled = false;

        if (event.type == VisibilityNotify && event.xvisibility.state == VisibilityFullyObscured && event.xvisibility.send_event == True) {
            handled = parent.windowFullyObscured(event.xvisibility.window);
        }

        return handled;
    }
    //! \reimp_end
};
#endif
