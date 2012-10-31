/***************************************************************************
**
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

#include "x11wrapper.h"

int X11Wrapper::XSync(Display *display, Bool discard)
{
    return ::XSync(display, discard);
}

XserverRegion X11Wrapper::XFixesCreateRegion(Display *dpy, XRectangle *rectangles, int nrectangles)
{
    return ::XFixesCreateRegion(dpy, rectangles, nrectangles);
}

void X11Wrapper::XFixesSetWindowShapeRegion(Display *dpy, Window win, int shape_kind, int x_off, int y_off, XserverRegion region)
{
    ::XFixesSetWindowShapeRegion(dpy, win, shape_kind, x_off, y_off, region);
}

void X11Wrapper::XFixesDestroyRegion(Display *dpy, XserverRegion region)
{
    ::XFixesDestroyRegion(dpy, region);
}
