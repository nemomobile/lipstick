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

#include <string.h>
#include "ut_windowinfo.h"
#include "windowinfo.h"
#include "x11wrapper.h"

#define ATOM_TYPE 1
#define ATOM_TYPE_NORMAL 2
#define ATOM_TYPE_DESKTOP 3
#define ATOM_TYPE_TEXT_PROPERTY 4
#define ATOM_STATE 5

Atom X11Wrapper::XInternAtom(Display *, const char* atom_name, Bool)
{
    if (strcmp(atom_name, "_NET_WM_WINDOW_TYPE") == 0) {
        return ATOM_TYPE;
    } else if (strcmp(atom_name, "_NET_WM_NAME") == 0) {
        return ATOM_TYPE_TEXT_PROPERTY;
    } else if (strcmp(atom_name, "_NET_WM_STATE") == 0) {
        return ATOM_STATE;
    }
    return 0;
}

int X11Wrapper::XSelectInput(Display *, Window , long)
{
    return 0;
}

Status X11Wrapper::XGetWindowAttributes(Display *, Window, XWindowAttributes *)
{
    return 0;
}

int X11Wrapper::XGetWindowProperty(Display *, Window , Atom property, long , long , Bool , Atom , Atom *, int *, unsigned long *nitems_return, unsigned long *, unsigned char **prop_return)
{
    if (property == ATOM_TYPE) {
        *nitems_return = 2;
        *prop_return = new unsigned char[2 * sizeof(Atom)];
        Atom* atom = (Atom *) * prop_return;
        atom[0] = ATOM_TYPE_NORMAL;
        atom[1] = ATOM_TYPE_DESKTOP;
        return Success;
    } else if (property == ATOM_STATE) {
        *nitems_return = 1;
        *prop_return = new unsigned char[1 * sizeof(Atom)];
        Atom* atom = (Atom *) * prop_return;
        atom[0] = ATOM_STATE;
        return Success;
    }
    return BadAtom;
}

int X11Wrapper::XFree(void *data)
{
    if (data != NULL) {
        delete [](unsigned char *)data;
    }
    return 0;
}

bool textValueFromGetTextProperty = true;
Status X11Wrapper::XGetWMName(Display *, Window, XTextProperty *textProperty)
{
    if (textValueFromGetTextProperty) {
        return 0;
    }
    QString textValue("WindowTitleXGetWMName");
    std::string::size_type strSize = textValue.toStdString().length();
    textProperty->value = new unsigned char[strSize + 1];
    strncpy((char *)textProperty->value, textValue.toStdString().c_str(), strSize + 1);
    return 1;
}

int gXGetTextPropertyCallCount = 0;
Status X11Wrapper::XGetTextProperty(Display *, Window , XTextProperty *text_prop_return, Atom atom)
{
    ++gXGetTextPropertyCallCount;

    if (!textValueFromGetTextProperty || atom != ATOM_TYPE_TEXT_PROPERTY) {
        return 0;
    }
    QString textValue("WindowTitleFromXGetTextProperty");
    std::string::size_type strSize = textValue.toStdString().length();
    text_prop_return->value = new unsigned char[strSize + 1];
    strncpy((char *)text_prop_return->value, textValue.toStdString().c_str(), strSize + 1);
    return 1;
}

XWMHints *X11Wrapper::XGetWMHints(Display *, Window)
{
    return 0;
}

int X11Wrapper::XFreePixmap(Display *, Pixmap)
{
    return 0;
}

Pixmap X11Wrapper::XCompositeNameWindowPixmap(Display *, Window)
{
    return 0;
}

Damage X11Wrapper::XDamageCreate(Display *, Drawable, int)
{
    return 0;
}

void X11Wrapper::XDamageDestroy(Display *, Damage)
{

}

int X11Wrapper::XSync(Display *, Bool)
{
    return 0;
}

XErrorHandler X11Wrapper::XSetErrorHandler(XErrorHandler)
{
    return 0;
}

int X11Wrapper::XChangeProperty(Display *, Window, Atom, Atom, int, int, unsigned char *, int)
{
    return 0;
}

Status X11Wrapper::XSendEvent(Display *, Window, Bool, long, XEvent *)
{
    return 0;
}

Window x11WrapperTransientForHint;
Status X11Wrapper::XGetTransientForHint(Display *, Window, Window *prop_return)
{
    *prop_return = x11WrapperTransientForHint;
    return 1;
}

void Ut_WindowInfo::initTestCase()
{
}

void Ut_WindowInfo::cleanupTestCase()
{
}

void Ut_WindowInfo::init()
{
    x11WrapperTransientForHint = 0;
    WindowInfo::initializeAtoms();
    windowInfo = new WindowInfo(1);
    gXGetTextPropertyCallCount = 0;
}

void Ut_WindowInfo::cleanup()
{
    delete windowInfo;
}

void Ut_WindowInfo::testGetters()
{
    textValueFromGetTextProperty = true;
    bool updated = windowInfo->updateWindowTitle();
    QVERIFY(updated);
    QCOMPARE(windowInfo->title(), QString("WindowTitleFromXGetTextProperty"));
    updated = false;
    textValueFromGetTextProperty = false;
    updated = windowInfo->updateWindowTitle();
    QVERIFY(updated);
    QCOMPARE(windowInfo->title(), QString("WindowTitleXGetWMName"));
    QCOMPARE(windowInfo->types().count(), 2);
    QCOMPARE(windowInfo->states().count(), 1);

    x11WrapperTransientForHint = 5;
    windowInfo->updateWindowProperties();
    QCOMPARE(windowInfo->transientFor(), x11WrapperTransientForHint);
}

void Ut_WindowInfo::testCreatingAnotherWindowInfoWithSameWindowIdDoesNotQueryX()
{
    WindowInfo anotherWindowInfo(windowInfo->window());
    QCOMPARE(gXGetTextPropertyCallCount, 0);
}

QTEST_MAIN(Ut_WindowInfo)
