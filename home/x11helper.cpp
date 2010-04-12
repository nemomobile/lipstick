/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include "x11helper.h"
#include "x11wrapper.h"

#ifdef Q_WS_X11

QVector<Atom> X11Helper::getNetWmState(Display *display, Window window)
{
    QVector<Atom> returnValue;

    Atom actualType;
    int actualFormat;
    ulong propertyLength;
    ulong bytesLeft;
    uchar *propertyData = 0;
    Atom netWmState = X11Wrapper::XInternAtom(display, "_NET_WM_STATE", False);

    // Don't read anything, just get the size of the property data
    if (X11Wrapper::XGetWindowProperty(display, window, netWmState, 0, 0,
                                       False, XA_ATOM, &actualType, &actualFormat,
                                       &propertyLength, &bytesLeft, &propertyData) == Success
            && actualType == XA_ATOM && actualFormat == 32) {
        returnValue.resize(bytesLeft / sizeof(Atom));
        X11Wrapper::XFree(propertyData);

        // Fetch all data
        if (X11Wrapper::XGetWindowProperty(display, window, netWmState, 0, returnValue.size(),
                                           False, XA_ATOM, &actualType, &actualFormat,
                                           &propertyLength, &bytesLeft, &propertyData) == Success) {
            if (propertyLength != (ulong)returnValue.size()) {
                returnValue.resize(propertyLength);
            }

            // Put it into the return value
            if (!returnValue.isEmpty()) {
                memcpy(returnValue.data(), propertyData, returnValue.size() * sizeof(Atom));
            }
            X11Wrapper::XFree(propertyData);
        } else {
            returnValue.clear();
        }
    }

    return returnValue;
}

#endif
