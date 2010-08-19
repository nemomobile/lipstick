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
#ifndef X11WRAPPER_STUB
#define X11WRAPPER_STUB

#include "x11wrapper.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class X11WrapperStub : public StubBase
{
public:
    virtual Atom XInternAtom(Display *display, const char *atom_name, Bool only_if_exists);
    virtual int XSelectInput(Display *display, Window w, long event_mask);
    virtual Status XGetWindowAttributes(Display *display, Window w, XWindowAttributes *window_attributes_return);
    virtual int XGetWindowProperty(Display *display, Window w, Atom property, long long_offset, long long_length, Bool del, Atom req_type, Atom *actual_type_return, int *actual_format_return, unsigned long *nitems_return, unsigned long *bytes_after_return, unsigned char **prop_return);
    virtual int XFree(void *data);
    virtual Status XGetWMName(Display *display, Window w, XTextProperty *text_prop_return);
    virtual Status XGetTextProperty(Display *display, Window w, XTextProperty *text_prop_return, Atom property);
    virtual XWMHints *XGetWMHints(Display *display, Window w);
    virtual int XFreePixmap(Display *display, Pixmap pixmap);
    virtual Pixmap XCompositeNameWindowPixmap(Display *dpy, Window window);
    virtual Damage XDamageCreate(Display *dpy, Drawable drawable, int level);
    virtual void XDamageSubtract(Display *display, Damage damage, XserverRegion repair, XserverRegion parts);
    virtual void XDamageDestroy(Display *dpy, Damage damage);
    virtual int XSync(Display *display, Bool discard);
    virtual XErrorHandler XSetErrorHandler(XErrorHandler handler);
    virtual int XChangeProperty(Display *display, Window w, Atom property, Atom type, int format, int mode, unsigned char *data, int nelements);
    virtual Status XSendEvent(Display *display, Window w, Bool propagate, long event_mask, XEvent *event_send);
};

// 2. IMPLEMENT STUB
Atom X11WrapperStub::XInternAtom(Display *display, const char *atom_name, Bool only_if_exists)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Display * >(display));
    params.append(new Parameter<const char * >(atom_name));
    params.append(new Parameter<Bool >(only_if_exists));
    stubMethodEntered("XInternAtom", params);
    return stubReturnValue<Atom>("XInternAtom");
}

int X11WrapperStub::XSelectInput(Display *display, Window w, long event_mask)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Display * >(display));
    params.append(new Parameter<Window >(w));
    params.append(new Parameter<long >(event_mask));
    stubMethodEntered("XSelectInput", params);
    return stubReturnValue<int>("XSelectInput");
}

Status X11WrapperStub::XGetWindowAttributes(Display *display, Window w, XWindowAttributes *window_attributes_return)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Display * >(display));
    params.append(new Parameter<Window >(w));
    params.append(new Parameter<XWindowAttributes * >(window_attributes_return));
    stubMethodEntered("XGetWindowAttributes", params);
    return stubReturnValue<Status>("XGetWindowAttributes");
}

int X11WrapperStub::XGetWindowProperty(Display *display, Window w, Atom property, long long_offset, long long_length, Bool del, Atom req_type, Atom *actual_type_return, int *actual_format_return, unsigned long *nitems_return, unsigned long *bytes_after_return, unsigned char **prop_return)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Display * >(display));
    params.append(new Parameter<Window >(w));
    params.append(new Parameter<Atom >(property));
    params.append(new Parameter<long >(long_offset));
    params.append(new Parameter<long >(long_length));
    params.append(new Parameter<Bool >(del));
    params.append(new Parameter<Atom >(req_type));
    params.append(new Parameter<Atom * >(actual_type_return));
    params.append(new Parameter<int * >(actual_format_return));
    params.append(new Parameter<unsigned long * >(nitems_return));
    params.append(new Parameter<unsigned long * >(bytes_after_return));
    params.append(new Parameter<unsigned char ** >(prop_return));
    stubMethodEntered("XGetWindowProperty", params);
    return stubReturnValue<int>("XGetWindowProperty");
}

int X11WrapperStub::XFree(void *data)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<void * >(data));
    stubMethodEntered("XFree", params);
    return stubReturnValue<int>("XFree");
}

Status X11WrapperStub::XGetWMName(Display *display, Window w, XTextProperty *text_prop_return)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Display * >(display));
    params.append(new Parameter<Window >(w));
    params.append(new Parameter<XTextProperty * >(text_prop_return));
    stubMethodEntered("XGetWMName", params);
    return stubReturnValue<Status>("XGetWMName");
}

Status X11WrapperStub::XGetTextProperty(Display *display, Window w, XTextProperty *text_prop_return, Atom property)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Display * >(display));
    params.append(new Parameter<Window >(w));
    params.append(new Parameter<XTextProperty * >(text_prop_return));
    params.append(new Parameter<Atom >(property));
    stubMethodEntered("XGetTextProperty", params);
    return stubReturnValue<Status>("XGetTextProperty");
}

XWMHints *X11WrapperStub::XGetWMHints(Display *display, Window w)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Display * >(display));
    params.append(new Parameter<Window >(w));
    stubMethodEntered("XGetWMHints", params);
    return stubReturnValue<XWMHints *>("XGetWMHints");
}

int X11WrapperStub::XFreePixmap(Display *display, Pixmap pixmap)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Display * >(display));
    params.append(new Parameter<Pixmap >(pixmap));
    stubMethodEntered("XFreePixmap", params);
    return stubReturnValue<int>("XFreePixmap");
}

Pixmap X11WrapperStub::XCompositeNameWindowPixmap(Display *dpy, Window window)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Display * >(dpy));
    params.append(new Parameter<Window >(window));
    stubMethodEntered("XCompositeNameWindowPixmap", params);
    return stubReturnValue<Pixmap>("XCompositeNameWindowPixmap");
}

Damage X11WrapperStub::XDamageCreate(Display *dpy, Drawable drawable, int level)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Display * >(dpy));
    params.append(new Parameter<Drawable >(drawable));
    params.append(new Parameter<int >(level));
    stubMethodEntered("XDamageCreate", params);
    return stubReturnValue<Damage>("XDamageCreate");
}

void X11WrapperStub::XDamageSubtract(Display *display, Damage damage, XserverRegion repair, XserverRegion parts)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Display * >(display));
    params.append(new Parameter<Damage >(damage));
    params.append(new Parameter<XserverRegion >(repair));
    params.append(new Parameter<XserverRegion >(parts));
    stubMethodEntered("XDamageSubtract", params);
}

void X11WrapperStub::XDamageDestroy(Display *dpy, Damage damage)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Display * >(dpy));
    params.append(new Parameter<Damage >(damage));
    stubMethodEntered("XDamageDestroy", params);
}

int X11WrapperStub::XSync(Display *display, Bool discard)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Display * >(display));
    params.append(new Parameter<Bool >(discard));
    stubMethodEntered("XSync", params);
    return stubReturnValue<int>("XSync");
}

XErrorHandler X11WrapperStub::XSetErrorHandler(XErrorHandler handler)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<XErrorHandler >(handler));
    stubMethodEntered("XSetErrorHandler", params);
    return stubReturnValue<XErrorHandler>("XSetErrorHandler");
}

int X11WrapperStub::XChangeProperty(Display *display, Window w, Atom property, Atom type, int format, int mode, unsigned char *data, int nelements)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Display * >(display));
    params.append(new Parameter<Window >(w));
    params.append(new Parameter<Atom >(property));
    params.append(new Parameter<Atom >(type));
    params.append(new Parameter<int >(format));
    params.append(new Parameter<int >(mode));
    params.append(new Parameter<unsigned char * >(data));
    params.append(new Parameter<int >(nelements));
    stubMethodEntered("XChangeProperty", params);
    return stubReturnValue<int>("XChangeProperty");
}

Status X11WrapperStub::XSendEvent(Display *display, Window w, Bool propagate, long event_mask, XEvent *event_send)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Display * >(display));
    params.append(new Parameter<Window >(w));
    params.append(new Parameter<Bool >(propagate));
    params.append(new Parameter<long >(event_mask));
    params.append(new Parameter<XEvent >(*event_send));
    stubMethodEntered("XSendEvent", params);
    return stubReturnValue<Status>("XSendEvent");
}



// 3. CREATE A STUB INSTANCE
X11WrapperStub gDefaultX11WrapperStub;
X11WrapperStub *gX11WrapperStub = &gDefaultX11WrapperStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
Atom X11Wrapper::XInternAtom(Display *display, const char *atom_name, Bool only_if_exists)
{
    return gX11WrapperStub->XInternAtom(display, atom_name, only_if_exists);
}

int X11Wrapper::XSelectInput(Display *display, Window w, long event_mask)
{
    return gX11WrapperStub->XSelectInput(display, w, event_mask);
}

Status X11Wrapper::XGetWindowAttributes(Display *display, Window w, XWindowAttributes *window_attributes_return)
{
    return gX11WrapperStub->XGetWindowAttributes(display, w, window_attributes_return);
}

int X11Wrapper::XGetWindowProperty(Display *display, Window w, Atom property, long long_offset, long long_length, Bool del, Atom req_type, Atom *actual_type_return, int *actual_format_return, unsigned long *nitems_return, unsigned long *bytes_after_return, unsigned char **prop_return)
{
    return gX11WrapperStub->XGetWindowProperty(display, w, property, long_offset, long_length, del, req_type, actual_type_return, actual_format_return, nitems_return, bytes_after_return, prop_return);
}

int X11Wrapper::XFree(void *data)
{
    return gX11WrapperStub->XFree(data);
}

Status X11Wrapper::XGetWMName(Display *display, Window w, XTextProperty *text_prop_return)
{
    return gX11WrapperStub->XGetWMName(display, w, text_prop_return);
}

Status X11Wrapper::XGetTextProperty(Display *display, Window w, XTextProperty *text_prop_return, Atom property)
{
    return gX11WrapperStub->XGetTextProperty(display, w, text_prop_return, property);
}

XWMHints *X11Wrapper::XGetWMHints(Display *display, Window w)
{
    return gX11WrapperStub->XGetWMHints(display, w);
}

int X11Wrapper::XFreePixmap(Display *display, Pixmap pixmap)
{
    return gX11WrapperStub->XFreePixmap(display, pixmap);
}

Pixmap X11Wrapper::XCompositeNameWindowPixmap(Display *dpy, Window window)
{
    return gX11WrapperStub->XCompositeNameWindowPixmap(dpy, window);
}

Damage X11Wrapper::XDamageCreate(Display *dpy, Drawable drawable, int level)
{
    return gX11WrapperStub->XDamageCreate(dpy, drawable, level);
}

void X11Wrapper::XDamageSubtract(Display *display, Damage damage, XserverRegion repair, XserverRegion parts)
{
    gX11WrapperStub->XDamageSubtract(display, damage, repair, parts);
}

void X11Wrapper::XDamageDestroy(Display *dpy, Damage damage)
{
    gX11WrapperStub->XDamageDestroy(dpy, damage);
}

int X11Wrapper::XSync(Display *display, Bool discard)
{
    return gX11WrapperStub->XSync(display, discard);
}

XErrorHandler X11Wrapper::XSetErrorHandler(XErrorHandler handler)
{
    return gX11WrapperStub->XSetErrorHandler(handler);
}

int X11Wrapper::XChangeProperty(Display *display, Window w, Atom property, Atom type, int format, int mode, unsigned char *data, int nelements)
{
    return gX11WrapperStub->XChangeProperty(display, w, property, type, format, mode, data, nelements);
}

Status X11Wrapper::XSendEvent(Display *display, Window w, Bool propagate, long event_mask, XEvent *event_send)
{
    return gX11WrapperStub->XSendEvent(display, w, propagate, event_mask, event_send);
}


#endif
