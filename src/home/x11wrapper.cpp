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

int X11Wrapper::XFreePixmap(Display *display, Pixmap pixmap)
{
    return ::XFreePixmap(display, pixmap);
}

Pixmap X11Wrapper::XCompositeNameWindowPixmap(Display *dpy, Window window)
{
    return ::XCompositeNameWindowPixmap(dpy, window);
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

void X11Wrapper::XDamageSubtract(Display *dpy, Damage damage, XserverRegion repair, XserverRegion parts)
{
    ::XDamageSubtract(dpy, damage, repair, parts);
}

Status X11Wrapper::XGetTransientForHint(Display *display, Window w, Window *prop_window_return)
{
    return ::XGetTransientForHint(display, w, prop_window_return);
}
