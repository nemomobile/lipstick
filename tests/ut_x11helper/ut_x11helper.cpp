/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of duihome.
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

#include <QtTest/QtTest>
#include "ut_x11helper.h"
#include "x11helper.h"
#include "x11wrapper.h"

static const Atom NET_WM_STATE_ATOM = 1;

// static variables in the test class
bool Ut_X11Helper::propertyQuerySucceeds = true;
bool Ut_X11Helper::propertySizeQuerySucceeds = true;

// X11Wrapper stubs
Atom X11Wrapper::XInternAtom(Display *display, const char *atom_name, Bool only_if_exists)
{
    Atom returnValue = 0;

    if (display == (Display *)1 && only_if_exists == False) {
        if (QString(atom_name) == "_NET_WM_STATE") {
            returnValue = NET_WM_STATE_ATOM;
        }
    }

    return returnValue;
}

// This is the list of atoms that gets reported to the window
const QVector<Atom> gNetWmStateAtoms(QVector<Atom>() << 1 << 2);
const int gNetWmStateAtomsCount = gNetWmStateAtoms.count();

int X11Wrapper::XGetWindowProperty(Display *display, Window w, Atom property, long long_offset, long long_length, Bool del, Atom req_type, Atom *actual_type_return, int *actual_format_return, unsigned long *nitems_return, unsigned long *bytes_after_return, unsigned char **prop_return)
{
    int returnValue = Success;

    // Let's set some common attributes. These are modified later if needed
    *actual_type_return = XA_ATOM;
    *actual_format_return = 32;

    if (display != (Display *)1 || w != 1 || property != NET_WM_STATE_ATOM || del != False || req_type != XA_ATOM) {
        returnValue = BadAtom;
    } else {
        if (long_offset == 0 && long_length == 0) {
            // Since long_length == 0, the requester wants to know the number of items available
            *bytes_after_return = sizeof(Atom) * gNetWmStateAtomsCount;
            // These rest don't really matter in this request, but let's set them anyway
            *nitems_return = 0;
            *prop_return = new unsigned char[0];

            if (!Ut_X11Helper::propertySizeQuerySucceeds) {
                returnValue = BadAtom;
            }
        } else if (long_offset == 0 && long_length == gNetWmStateAtomsCount) {
            // Since long_length == gNetWmStateAtomsCount, the requester wants to know the actual data. Let's return it
            *nitems_return = gNetWmStateAtomsCount;
            *prop_return = new unsigned char[gNetWmStateAtomsCount * sizeof(Atom)];
            Atom *atom = (Atom *) * prop_return;
            for (int i = 0; i < gNetWmStateAtomsCount; ++i) {
                atom[i] = gNetWmStateAtoms.at(i);
            }
            // These rest don't really matter in this request, but let's set them anyway
            *bytes_after_return = sizeof(Atom) * gNetWmStateAtomsCount;

            if (!Ut_X11Helper::propertyQuerySucceeds) {
                returnValue = BadAtom;
            }
        }
    }

    return returnValue;
}

int X11Wrapper::XFree(void *data)
{
    if (data != NULL) {
        delete[](unsigned char *)data;
    }
    return 0;
}

void Ut_X11Helper::initTestCase()
{
}

void Ut_X11Helper::cleanupTestCase()
{
}

void Ut_X11Helper::init()
{
    propertyQuerySucceeds = true;
    propertySizeQuerySucceeds = true;
}

void Ut_X11Helper::cleanup()
{
}

void Ut_X11Helper::testGetNetWmState()
{
    QVector<Atom> atoms = X11Helper::getNetWmState((Display *)1, 1);
    QCOMPARE(atoms, gNetWmStateAtoms);
}

void Ut_X11Helper::testGetNetWmStatePropertyQueryFails()
{
    propertyQuerySucceeds = false;
    QVector<Atom> atoms = X11Helper::getNetWmState((Display *)1, 1);
    QCOMPARE(atoms.count(), 0);
}

void Ut_X11Helper::testGetNetWmStateProperySizeQueryFails()
{
    propertySizeQuerySucceeds = false;
    QVector<Atom> atoms = X11Helper::getNetWmState((Display *)1, 1);
    QCOMPARE(atoms.count(), 0);
}

QTEST_APPLESS_MAIN(Ut_X11Helper)
