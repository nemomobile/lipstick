/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
#ifndef X11WRAPPER_STUB
#define X11WRAPPER_STUB

#include "x11wrapper.h"
#include <stubbase.h>

struct XGetWindowPropertyDataReturnValueData;

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
    virtual XWMHints *XGetWMHints(Display *display, Window w);
    virtual int XFreePixmap(Display *display, Pixmap pixmap);
    virtual Damage XDamageCreate(Display *dpy, Drawable drawable, int level);
    virtual void XDamageDestroy(Display *dpy, Damage damage);
    virtual int XSync(Display *display, Bool discard);
    virtual XErrorHandler XSetErrorHandler(XErrorHandler handler);
    virtual int XChangeProperty(Display *display, Window w, Atom property, Atom type, int format, int mode, unsigned char *data, int nelements);
    virtual Status XSendEvent(Display *display, Window w, Bool propagate, long event_mask, XEvent *event_send);
    virtual Window XCreateWindow(Display *display, Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, int depth, unsigned int class_, Visual *visual, unsigned long valuemask, XSetWindowAttributes *attributes);
    virtual int XDestroyWindow(Display *display, Window w);
    virtual int XMapRaised(Display *display, Window w);
    virtual int XUnmapWindow(Display *display, Window w);
    virtual int XStoreName(Display *display, Window w, char *window_name);
    virtual int XGrabKeyboard(Display *display, Window grab_window, Bool owner_events, int pointer_mode, int keyboard_mode, Time time);
    virtual int XGrabPointer(Display *display, Window grab_window, Bool owner_events, unsigned int event_mask,
                              int pointer_mode, int keyboard_mode, Window confine_to, Cursor cursor, Time time);
    virtual int XDeleteProperty(Display *display, Window w, Atom property);
    virtual XserverRegion XFixesCreateRegion(Display *dpy, XRectangle *rectangles, int nrectangles);
    virtual void XFixesSetWindowShapeRegion(Display *dpy, Window win, int shape_kind, int x_off, int y_off, XserverRegion region);
    virtual void XFixesDestroyRegion(Display *dpy, XserverRegion region);

    virtual void stubSetXGetWindowPropertyDataReturnValues(Window window, XGetWindowPropertyDataReturnValueData data);
    virtual void stubReset() const;
};

// 2. IMPLEMENT STUB
Atom X11WrapperStub::XInternAtom(Display *display, const char *atom_name, Bool only_if_exists)
{
    static QHash<QString, Atom> atomMap;
    static int last_atom = 1;

    QList<ParameterBase *> params;
    params.append(new Parameter<Display * >(display));
    params.append(new Parameter<const char * >(atom_name));
    params.append(new Parameter<Bool >(only_if_exists));
    stubMethodEntered("XInternAtom", params);

    if (!atomMap.contains(atom_name))
        atomMap[atom_name] = last_atom++;

    return atomMap[atom_name];
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

struct XGetWindowPropertyDataReturnValueData {
    Atom type;
    int format;
    unsigned char* data;
    int return_value;
};
QHash<Window, XGetWindowPropertyDataReturnValueData> xGetWindowPropertyDataReturnValueHash;
void X11WrapperStub::stubSetXGetWindowPropertyDataReturnValues(Window window, XGetWindowPropertyDataReturnValueData data)
{
    xGetWindowPropertyDataReturnValueHash.insert(window, data);
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

    int return_value = stubReturnValue<int>("XGetWindowProperty");

    if(xGetWindowPropertyDataReturnValueHash.contains(w)) {
        XGetWindowPropertyDataReturnValueData data = xGetWindowPropertyDataReturnValueHash.value(w);
        *actual_type_return = data.type;
        *actual_format_return = data.format;
        *prop_return = data.data;
        return_value = data.return_value;
    }

    return return_value;
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

Damage X11WrapperStub::XDamageCreate(Display *dpy, Drawable drawable, int level)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Display * >(dpy));
    params.append(new Parameter<Drawable >(drawable));
    params.append(new Parameter<int >(level));
    stubMethodEntered("XDamageCreate", params);
    return stubReturnValue<Damage>("XDamageCreate");
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
    params.append(new Parameter<QByteArray>(QByteArray(reinterpret_cast<const char*>(data), (format/8)*nelements)));
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

Window X11WrapperStub::XCreateWindow(Display *display, Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, int depth, unsigned int class_, Visual *visual, unsigned long valuemask, XSetWindowAttributes *attributes)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Display *>(display));
    params.append(new Parameter<Window>(parent));
    params.append(new Parameter<int>(x));
    params.append(new Parameter<int>(y));
    params.append(new Parameter<unsigned int>(width));
    params.append(new Parameter<unsigned int>(height));
    params.append(new Parameter<unsigned int>(border_width));
    params.append(new Parameter<int>(depth));
    params.append(new Parameter<unsigned int>(class_));
    params.append(new Parameter<Visual *>(visual));
    params.append(new Parameter<unsigned long>(valuemask));
    params.append(new Parameter<XSetWindowAttributes>(*attributes));
    stubMethodEntered("XCreateWindow", params);
    return stubReturnValue<Window>("XCreateWindow");
}

int X11WrapperStub::XDestroyWindow(Display *display, Window w)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Display *>(display));
    params.append(new Parameter<Window>(w));
    stubMethodEntered("XDestroyWindow", params);
    return stubReturnValue<int>("XDestroyWindow");
}

int X11WrapperStub::XMapRaised(Display *display, Window w)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Display *>(display));
    params.append(new Parameter<Window>(w));
    stubMethodEntered("XMapRaised", params);
    return stubReturnValue<int>("XMapRaised");
}

int X11WrapperStub::XUnmapWindow(Display *display, Window w)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Display *>(display));
    params.append(new Parameter<Window>(w));
    stubMethodEntered("XUnmapWindow", params);
    return stubReturnValue<int>("XUnmapWindow");
}

int X11WrapperStub::XStoreName(Display *display, Window w, char *window_name)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Display *>(display));
    params.append(new Parameter<Window>(w));
    params.append(new Parameter<char *>(window_name));
    stubMethodEntered("XStoreName", params);
    return stubReturnValue<int>("XStoreName");
}

int X11WrapperStub::XGrabKeyboard(Display *display, Window grab_window, Bool owner_events, int pointer_mode, int keyboard_mode, Time time)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Display *>(display));
    params.append(new Parameter<Window>(grab_window));
    params.append(new Parameter<Bool>(owner_events));
    params.append(new Parameter<int>(pointer_mode));
    params.append(new Parameter<int>(keyboard_mode));
    params.append(new Parameter<Time>(time));
    stubMethodEntered("XGrabKeyboard", params);
    return stubReturnValue<int>("XGrabKeyboard");
}

int X11WrapperStub::XGrabPointer(Display *display, Window grab_window, Bool owner_events, unsigned int event_mask,
                              int pointer_mode, int keyboard_mode, Window confine_to, Cursor cursor, Time time)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Display *>(display));
    params.append(new Parameter<Window>(grab_window));
    params.append(new Parameter<Bool>(owner_events));
    params.append(new Parameter<int>(event_mask));
    params.append(new Parameter<int>(pointer_mode));
    params.append(new Parameter<int>(keyboard_mode));
    params.append(new Parameter<Window>(confine_to));
    params.append(new Parameter<Cursor>(cursor));
    params.append(new Parameter<Time>(time));
    stubMethodEntered("XGrabPointer", params);
    return stubReturnValue<int>("XGrabPointer");
}

int X11WrapperStub::XDeleteProperty(Display *display, Window w, Atom property)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Display *>(display));
    params.append(new Parameter<Window>(w));
    params.append(new Parameter<Atom>(property));
    stubMethodEntered("XDeleteProperty", params);
    return stubReturnValue<int>("XDeleteProperty");
}

XserverRegion X11WrapperStub::XFixesCreateRegion(Display *dpy, XRectangle *rectangles, int nrectangles)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Display *>(dpy));
    params.append(new Parameter<XRectangle *>(rectangles));
    params.append(new Parameter<int>(nrectangles));
    stubMethodEntered("XFixesCreateRegion", params);
    return stubReturnValue<XserverRegion>("XFixesCreateRegion");
}

void X11WrapperStub::XFixesSetWindowShapeRegion(Display *dpy, Window win, int shape_kind, int x_off, int y_off, XserverRegion region)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Display *>(dpy));
    params.append(new Parameter<Window>(win));
    params.append(new Parameter<int>(shape_kind));
    params.append(new Parameter<int>(x_off));
    params.append(new Parameter<int>(y_off));
    params.append(new Parameter<XserverRegion>(region));
    stubMethodEntered("XFixesSetWindowShapeRegion", params);
}

void X11WrapperStub::XFixesDestroyRegion(Display *dpy, XserverRegion region)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Display *>(dpy));
    params.append(new Parameter<XserverRegion>(region));
    stubMethodEntered("XFixesDestroyRegion", params);
}

void X11WrapperStub::stubReset() const
{
    StubBase::stubReset();
    xGetWindowPropertyDataReturnValueHash.clear();
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

XWMHints *X11Wrapper::XGetWMHints(Display *display, Window w)
{
    return gX11WrapperStub->XGetWMHints(display, w);
}

int X11Wrapper::XFreePixmap(Display *display, Pixmap pixmap)
{
    return gX11WrapperStub->XFreePixmap(display, pixmap);
}

Damage X11Wrapper::XDamageCreate(Display *dpy, Drawable drawable, int level)
{
    return gX11WrapperStub->XDamageCreate(dpy, drawable, level);
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

Window X11Wrapper::XCreateWindow(Display *display, Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, int depth, unsigned int class_, Visual *visual, unsigned long valuemask, XSetWindowAttributes *attributes)
{
    return gX11WrapperStub->XCreateWindow(display, parent, x, y, width, height, border_width, depth, class_, visual, valuemask, attributes);
}

int X11Wrapper::XDestroyWindow(Display *display, Window w)
{
    return gX11WrapperStub->XDestroyWindow(display, w);
}

int X11Wrapper::XMapRaised(Display *display, Window w)
{
    return gX11WrapperStub->XMapRaised(display, w);
}

int X11Wrapper::XUnmapWindow(Display *display, Window w)
{
    return gX11WrapperStub->XUnmapWindow(display, w);
}

int X11Wrapper::XStoreName(Display *display, Window w, char *window_name)
{
    return gX11WrapperStub->XStoreName(display, w, window_name);
}

int X11Wrapper::XGrabKeyboard(Display *display, Window grab_window, Bool owner_events, int pointer_mode, int keyboard_mode, Time time)
{
    return gX11WrapperStub->XGrabKeyboard(display, grab_window, owner_events, pointer_mode, keyboard_mode, time);
}

int X11Wrapper::XGrabPointer(Display *display, Window grab_window, Bool owner_events, unsigned int event_mask,
                              int pointer_mode, int keyboard_mode, Window confine_to, Cursor cursor, Time time)
{
    return gX11WrapperStub->XGrabPointer(display, grab_window, owner_events, event_mask,
                                         pointer_mode, keyboard_mode, confine_to, cursor, time);
}

int X11Wrapper::XDeleteProperty(Display *display, Window w, Atom property) {
    return gX11WrapperStub->XDeleteProperty(display, w, property);
}

XserverRegion X11Wrapper::XFixesCreateRegion(Display *dpy, XRectangle *rectangles, int nrectangles)
{
    return gX11WrapperStub->XFixesCreateRegion(dpy, rectangles, nrectangles);
}

void X11Wrapper::XFixesSetWindowShapeRegion(Display *dpy, Window win, int shape_kind, int x_off, int y_off, XserverRegion region)
{
    gX11WrapperStub->XFixesSetWindowShapeRegion(dpy, win, shape_kind, x_off, y_off, region);
}

void X11Wrapper::XFixesDestroyRegion(Display *dpy, XserverRegion region)
{
    gX11WrapperStub->XFixesDestroyRegion(dpy, region);
}

#endif
