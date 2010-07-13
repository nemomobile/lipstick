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

#ifndef X11WRAPPER_H_
#define X11WRAPPER_H_

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xdamage.h>

class X11Wrapper
{
public:
    static Atom XInternAtom(Display *display, const char *atom_name, Bool only_if_exists);
    static int XSelectInput(Display *display, Window w, long event_mask);
    static Status XGetWindowAttributes(Display *display, Window w, XWindowAttributes *window_attributes_return);
    static int XGetWindowProperty(Display *display, Window w, Atom property, long long_offset, long long_length, Bool del, Atom req_type, Atom *actual_type_return, int *actual_format_return, unsigned long *nitems_return, unsigned long *bytes_after_return, unsigned char **prop_return);
    static int XFree(void *data);
    static Status XGetWMName(Display *display, Window w, XTextProperty *text_prop_return);
    static Status XGetTextProperty(Display *display, Window w, XTextProperty *text_prop_return, Atom property);
    static XWMHints *XGetWMHints(Display *display, Window w);
    static int XFreePixmap(Display *display, Pixmap pixmap);
    static Pixmap XCompositeNameWindowPixmap(Display *dpy, Window window);
    static Damage XDamageCreate(Display *dpy, Drawable drawable, int level);
    static void XDamageDestroy(Display *dpy, Damage damage);
    static int XSync(Display *display, Bool discard);
    static XErrorHandler XSetErrorHandler(XErrorHandler handler);
    static int XChangeProperty(Display *display, Window w, Atom property, Atom type, int format, int mode, unsigned char *data, int nelements);
    static Status XSendEvent(Display *display, Window w, Bool propagate, long event_mask, XEvent *event_send);
    static void XDamageSubtract(Display *dpy, Damage damage, XserverRegion repair, XserverRegion parts);
    static Status XGetTransientForHint(Display *display, Window w, Window *prop_window_return);
};

#endif /* X11WRAPPER_H_ */
