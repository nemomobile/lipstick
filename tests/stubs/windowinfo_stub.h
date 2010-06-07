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

#include "windowinfo.h"
#include <stubbase.h>

Atom WindowInfo::TypeAtom;
Atom WindowInfo::StateAtom;
Atom WindowInfo::NormalAtom;
Atom WindowInfo::DesktopAtom;
Atom WindowInfo::NotificationAtom;
Atom WindowInfo::DialogAtom;
Atom WindowInfo::CallAtom;
Atom WindowInfo::DockAtom;
Atom WindowInfo::MenuAtom;
Atom WindowInfo::SkipTaskbarAtom;
Atom WindowInfo::NameAtom;


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class WindowInfoStub : public StubBase
{
public:
    virtual void WindowInfoConstructor(Window window);
    virtual void WindowInfoConstructor();
    virtual void WindowInfoDestructor();
    virtual const QString &title() const;
    virtual QList<Atom> types() const;
    virtual QList<Atom> states() const;
    virtual Window window() const;
    virtual bool updateWindowTitle();
    virtual void updateWindowProperties();
    virtual WindowInfo::WindowPriority windowPriority() const;
    QString _title;
    Window _window;
    WindowInfo::WindowPriority _windowPriority;
};

// 2. IMPLEMENT STUB
void WindowInfoStub::WindowInfoConstructor(Window window)
{
    Q_UNUSED(window);
}

void WindowInfoStub::WindowInfoConstructor()
{
}

void WindowInfoStub::WindowInfoDestructor()
{

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

WindowInfo::WindowPriority WindowInfoStub::windowPriority() const
{
    stubMethodEntered("windowPriority");
    return stubReturnValue<WindowInfo::WindowPriority>("windowPriority");
}

// 3. CREATE A STUB INSTANCE
WindowInfoStub gDefaultWindowInfoStub;
WindowInfoStub *gWindowInfoStub = &gDefaultWindowInfoStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
WindowInfo::WindowInfo(Window window)
{
    gWindowInfoStub->WindowInfoConstructor(window);
}

WindowInfo::WindowInfo()
{
    gWindowInfoStub->WindowInfoConstructor();
}

WindowInfo::~WindowInfo()
{
    gWindowInfoStub->WindowInfoDestructor();
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

bool WindowInfo::updateWindowTitle()
{
    return gWindowInfoStub->updateWindowTitle();
}
    
void WindowInfo::updateWindowProperties()
{
    gWindowInfoStub->updateWindowProperties();
}

WindowInfo::WindowPriority WindowInfo::windowPriority() const
{
    return gWindowInfoStub->windowPriority();
}

bool operator==(const WindowInfo &wi1, const WindowInfo &wi2)
{
    return wi1.window() == wi2.window();
}
    
#endif
