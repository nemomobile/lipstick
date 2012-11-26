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

Atom X11Wrapper::XInternAtom(Display *display, const char *atom_name, Bool only_if_exists)
{
    return ::XInternAtom(display, atom_name, only_if_exists);
}

int X11Wrapper::XSelectInput(Display *display, Window w, long event_mask)
{
    return ::XSelectInput(display, w, event_mask);
}

Status X11Wrapper::XGetWindowAttributes(Display *display, Window w, XWindowAttributes *window_attributes_return)
{
    return ::XGetWindowAttributes(display, w, window_attributes_return);
}

int X11Wrapper::XGetWindowProperty(Display *display, Window w, Atom property, long long_offset, long long_length, Bool del, Atom req_type, Atom *actual_type_return, int *actual_format_return, unsigned long *nitems_return, unsigned long *bytes_after_return, unsigned char **prop_return)
{
    return ::XGetWindowProperty(display, w, property, long_offset, long_length, del, req_type, actual_type_return, actual_format_return, nitems_return, bytes_after_return, prop_return);
}

int X11Wrapper::XFree(void *data)
{
    return ::XFree(data);
}

Status X11Wrapper::XGetWMName(Display *display, Window w, XTextProperty *text_prop_return)
{
    return ::XGetWMName(display, w, text_prop_return);
}

Status X11Wrapper::XGetTextProperty(Display *display, Window w, XTextProperty *text_prop_return, Atom property)
{
    return ::XGetTextProperty(display, w, text_prop_return, property);
}

XWMHints *X11Wrapper::XGetWMHints(Display *display, Window w)
{
    return ::XGetWMHints(display, w);
}

Pixmap X11Wrapper::XCreatePixmap(Display *display, Drawable drawable, unsigned int width, unsigned height, unsigned int depth)
{
    return ::XCreatePixmap(display, drawable, width, height, depth);
}

int X11Wrapper::XFreePixmap(Display *display, Pixmap pixmap)
{
    return ::XFreePixmap(display, pixmap);
}

Damage X11Wrapper::XDamageCreate(Display *dpy, Drawable drawable, int level)
{
    return ::XDamageCreate(dpy, drawable, level);
}

void X11Wrapper::XDamageDestroy(Display *dpy, Damage damage)
{
    ::XDamageDestroy(dpy, damage);
}

int X11Wrapper::XSync(Display *display, Bool discard)
{
    return ::XSync(display, discard);
}

XErrorHandler X11Wrapper::XSetErrorHandler(XErrorHandler handler)
{
    return ::XSetErrorHandler(handler);
}

int X11Wrapper::XChangeProperty(Display *display, Window w, Atom property, Atom type, int format, int mode, unsigned char *data, int nelements)
{
    return ::XChangeProperty(display, w, property, type, format, mode, data, nelements);
}

Status X11Wrapper::XSendEvent(Display *display, Window w, Bool propagate, long event_mask, XEvent *event_send)
{
    return ::XSendEvent(display, w, propagate, event_mask, event_send);
}

Window X11Wrapper::XCreateWindow(Display *display, Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, int depth, unsigned int class_, Visual *visual, unsigned long valuemask, XSetWindowAttributes *attributes)
{
    return ::XCreateWindow(display, parent, x, y, width, height, border_width, depth, class_, visual, valuemask, attributes);
}

int X11Wrapper::XDestroyWindow(Display *display, Window w)
{
    return ::XDestroyWindow(display, w);
}

int X11Wrapper::XMapRaised(Display *display, Window w)
{
    return ::XMapRaised(display, w);
}

int X11Wrapper::XUnmapWindow(Display *display, Window w)
{
    return ::XUnmapWindow(display, w);
}

int X11Wrapper::XStoreName(Display *display, Window w, char *window_name)
{
    return ::XStoreName(display, w, window_name);
}

int X11Wrapper::XGrabKeyboard(Display *display, Window grab_window, Bool owner_events, int pointer_mode, int keyboard_mode, Time time)
{
    return ::XGrabKeyboard(display, grab_window, owner_events, pointer_mode, keyboard_mode, time);
}

int X11Wrapper::XGrabPointer(Display *display, Window grab_window, Bool owner_events, unsigned int event_mask,
                              int pointer_mode, int keyboard_mode, Window confine_to, Cursor cursor, Time time)
{
    return ::XGrabPointer(display, grab_window, owner_events, event_mask, pointer_mode, keyboard_mode, confine_to, cursor, time);
}

int X11Wrapper::XDeleteProperty(Display *display, Window w, Atom property)
{
    return ::XDeleteProperty(display, w, property);
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
