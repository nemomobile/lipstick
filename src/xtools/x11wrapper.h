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
    static Atom XInternAtom(Display *display, const char *atom_name, Bool only_if_exists);
    static int XSelectInput(Display *display, Window w, long event_mask);
    static Status XGetWindowAttributes(Display *display, Window w, XWindowAttributes *window_attributes_return);
    static int XGetWindowProperty(Display *display, Window w, Atom property, long long_offset, long long_length, Bool del, Atom req_type, Atom *actual_type_return, int *actual_format_return, unsigned long *nitems_return, unsigned long *bytes_after_return, unsigned char **prop_return);
    static int XFree(void *data);
    static Status XGetWMName(Display *display, Window w, XTextProperty *text_prop_return);
    static Status XGetTextProperty(Display *display, Window w, XTextProperty *text_prop_return, Atom property);
    static XWMHints *XGetWMHints(Display *display, Window w);
    static Pixmap XCreatePixmap(Display *display, Drawable drawable, unsigned int width, unsigned height, unsigned int depth);
    static int XFreePixmap(Display *display, Pixmap pixmap);
    static Damage XDamageCreate(Display *dpy, Drawable drawable, int level);
    static void XDamageDestroy(Display *dpy, Damage damage);
    static int XSync(Display *display, Bool discard);
    static XErrorHandler XSetErrorHandler(XErrorHandler handler);
    static int XChangeProperty(Display *display, Window w, Atom property, Atom type, int format, int mode, unsigned char *data, int nelements);
    static Status XSendEvent(Display *display, Window w, Bool propagate, long event_mask, XEvent *event_send);
    static Window XCreateWindow(Display *display, Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, int depth, unsigned int class_, Visual *visual, unsigned long valuemask, XSetWindowAttributes *attributes);
    static int XDestroyWindow(Display *display, Window w);
    static int XMapRaised(Display *display, Window w);
    static int XUnmapWindow(Display *display, Window w);
    static int XStoreName(Display *display, Window w, char *window_name);
    static int XGrabKeyboard(Display *display, Window grab_window, Bool owner_events, int pointer_mode, int keyboard_mode, Time time);
    static int XGrabPointer(Display *display, Window grab_window, Bool owner_events, unsigned int event_mask,
                              int pointer_mode, int keyboard_mode, Window confine_to, Cursor cursor, Time time);
    static int XDeleteProperty(Display *display, Window w, Atom property);
    static XserverRegion XFixesCreateRegion(Display *dpy, XRectangle *rectangles, int nrectangles);
    static void XFixesSetWindowShapeRegion(Display *dpy, Window win, int shape_kind, int x_off, int y_off, XserverRegion region);
    static void XFixesDestroyRegion(Display *dpy, XserverRegion region);
};

#endif /* X11WRAPPER_H_ */
