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

#ifndef X11WRAPPER_H_
#define X11WRAPPER_H_

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xdamage.h>
#include <X11/extensions/Xfixes.h>

class X11Wrapper
{
public:
    static int XSync(Display *display, Bool discard);
    static XserverRegion XFixesCreateRegion(Display *dpy, XRectangle *rectangles, int nrectangles);
    static void XFixesSetWindowShapeRegion(Display *dpy, Window win, int shape_kind, int x_off, int y_off, XserverRegion region);
    static void XFixesDestroyRegion(Display *dpy, XserverRegion region);
};

#endif /* X11WRAPPER_H_ */
