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
#ifndef WINDOWINFO_STUB
#define WINDOWINFO_STUB

#include "windowinfo.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class WindowInfoStub : public StubBase
{
public:
    virtual void WindowInfoConstructor(QString &title, Window window, XWindowAttributes windowAttributes, Pixmap icon, WindowInfo::WindowPriority priority);
    virtual void WindowInfoDestructor();
    virtual const QString &title() const;
    virtual Window window() const;
    virtual XWindowAttributes windowAttributes() const;
    virtual Pixmap icon() const;
    QString _title ;
    Window _window ;
    XWindowAttributes _attributes ;
    Pixmap _pixmap ;
};

// 2. IMPLEMENT STUB
void WindowInfoStub::WindowInfoConstructor(QString &title, Window window, XWindowAttributes windowAttributes, Pixmap icon, WindowInfo::WindowPriority priority)
{
    Q_UNUSED(title);
    Q_UNUSED(window);
    Q_UNUSED(windowAttributes);
    Q_UNUSED(icon);
    Q_UNUSED(priority);
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

XWindowAttributes WindowInfoStub::windowAttributes() const
{
    stubMethodEntered("windowAttributes");
    return stubReturnValue<XWindowAttributes>("windowAttributes");
}

Pixmap WindowInfoStub::icon() const
{
    stubMethodEntered("icon");
    return stubReturnValue<Pixmap>("icon");
}



// 3. CREATE A STUB INSTANCE
WindowInfoStub gDefaultWindowInfoStub;
WindowInfoStub *gWindowInfoStub = &gDefaultWindowInfoStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
WindowInfo::WindowInfo(QString &title, Window window, XWindowAttributes windowAttributes, Pixmap icon, WindowInfo::WindowPriority priority)
{
    gWindowInfoStub->WindowInfoConstructor(title, window, windowAttributes, icon, priority);
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

XWindowAttributes WindowInfo::windowAttributes() const
{
    return gWindowInfoStub->windowAttributes();
}

Pixmap WindowInfo::icon() const
{
    return gWindowInfoStub->icon();
}


#endif
