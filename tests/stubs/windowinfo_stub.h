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
#ifndef WINDOWINFO_STUB
#define WINDOWINFO_STUB

#include <stubbase.h>
#include "windowinfo.h"

Atom WindowInfo::TypeAtom = 1;
Atom WindowInfo::StateAtom = 2;
Atom WindowInfo::NormalAtom = 3;
Atom WindowInfo::DesktopAtom = 4;
Atom WindowInfo::NotificationAtom = 5;
Atom WindowInfo::DialogAtom = 6;
Atom WindowInfo::CallAtom = 7;
Atom WindowInfo::DockAtom = 8;
Atom WindowInfo::MenuAtom = 9;
Atom WindowInfo::SkipTaskbarAtom = 10;
Atom WindowInfo::NameAtom = 11;

class WindowData : public QSharedData
{
};

// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class WindowInfoStub : public StubBase
{
public:
    virtual void WindowInfoConstructor(Window window);
    virtual void WindowInfoConstructor(const WindowInfo &other);
    virtual void WindowInfoDestructor();
    virtual void initializeAtoms();
    virtual const QString &title() const;
    virtual QList<Atom> types() const;
    virtual QList<Atom> states() const;
    virtual Window window() const;
    virtual Window transientFor() const;
    virtual bool updateWindowTitle();
    virtual void updateWindowProperties();
};


// 2. IMPLEMENT STUB
void WindowInfoStub::WindowInfoConstructor(Window window)
{
    QList<ParameterBase*> params;
    params.append(new Parameter<Window>(window));
    stubMethodEntered("WindowInfoConstructor(Window)", params);
}

void WindowInfoStub::WindowInfoConstructor(const WindowInfo &other)
{
    QList<ParameterBase*> params;
    params.append(new Parameter<const WindowInfo&>(other));
    stubMethodEntered("WindowInfoCopyConstructor", params);
}

void WindowInfoStub::WindowInfoDestructor()
{

}

void WindowInfoStub::initializeAtoms()
{
    stubMethodEntered("initializeAtoms");
}

const QString &WindowInfoStub::title() const
{
    stubMethodEntered("title");
    return stubReturnValue<QString>("title");
}

Window WindowInfoStub::window() const
{
    stubMethodEntered("window");
    return stubReturnValue<Window>("window");
}

Window WindowInfoStub::transientFor() const
{
    stubMethodEntered("transientFor");
    return stubReturnValue<Window>("transientFor");
}

QList<Atom> WindowInfoStub::types() const
{
    stubMethodEntered("types");
    return stubReturnValue<QList<Atom> >("types");
}

QList<Atom> WindowInfoStub::states() const
{
    stubMethodEntered("states");
    return stubReturnValue<QList<Atom> >("states");
}

bool WindowInfoStub::updateWindowTitle()
{
    stubMethodEntered("updateWindowTitle");
    return stubReturnValue<bool>("updateWindowTitle");
}

void WindowInfoStub::updateWindowProperties()
{
    stubMethodEntered("updateWindowProperties");
}

// 3. CREATE A STUB INSTANCE
WindowInfoStub gDefaultWindowInfoStub;
WindowInfoStub *gWindowInfoStub = &gDefaultWindowInfoStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
WindowInfo::WindowInfo(Window window)
{
    gWindowInfoStub->WindowInfoConstructor(window);
}

WindowInfo::WindowInfo(const WindowInfo &other)
{
    gWindowInfoStub->WindowInfoConstructor(other);
}

WindowInfo::~WindowInfo()
{
    gWindowInfoStub->WindowInfoDestructor();
}

void WindowInfo::initializeAtoms()
{
    gWindowInfoStub->initializeAtoms();
}

const QString &WindowInfo::title() const
{
    return gWindowInfoStub->title();
}

Window WindowInfo::window() const
{
    return gWindowInfoStub->window();
}

QList<Atom> WindowInfo::types() const
{
    return gWindowInfoStub->types();
}

QList<Atom> WindowInfo::states() const
{
    return gWindowInfoStub->states();
}

Window WindowInfo::transientFor() const
{
    return gWindowInfoStub->transientFor();
}

bool WindowInfo::updateWindowTitle()
{
    return gWindowInfoStub->updateWindowTitle();
}

void WindowInfo::updateWindowProperties()
{
    gWindowInfoStub->updateWindowProperties();
}

bool operator==(const WindowInfo &wi1, const WindowInfo &wi2)
{
    return wi1.window() == wi2.window();
}

#endif
