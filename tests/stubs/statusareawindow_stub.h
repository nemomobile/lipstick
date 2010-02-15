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
#ifndef STATUSAREAWINDOW_STUB
#define STATUSAREAWINDOW_STUB

#include "statusareawindow.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class StatusAreaWindowStub : public StubBase
{
public:
    virtual void StatusAreaWindowConstructor(QWidget *parent);
    virtual void StatusAreaWindowDestructor();
    virtual StatusArea *statusArea() const;
    virtual void rotate(const Dui::Orientation &orientation);
};

// 2. IMPLEMENT STUB
void StatusAreaWindowStub::StatusAreaWindowConstructor(QWidget *parent)
{
    Q_UNUSED(parent);

}
void StatusAreaWindowStub::StatusAreaWindowDestructor()
{

}
StatusArea *StatusAreaWindowStub::statusArea() const
{
    stubMethodEntered("statusArea");
    return stubReturnValue<StatusArea *>("statusArea");
}

void StatusAreaWindowStub::rotate(const Dui::Orientation &orientation)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const Dui::Orientation & >(orientation));
    stubMethodEntered("rotate", params);
}



// 3. CREATE A STUB INSTANCE
StatusAreaWindowStub gDefaultStatusAreaWindowStub;
StatusAreaWindowStub *gStatusAreaWindowStub = &gDefaultStatusAreaWindowStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
StatusAreaWindow::StatusAreaWindow(QWidget *parent)
{
    gStatusAreaWindowStub->StatusAreaWindowConstructor(parent);
}

StatusAreaWindow::~StatusAreaWindow()
{
    gStatusAreaWindowStub->StatusAreaWindowDestructor();
}

StatusArea *StatusAreaWindow::statusArea() const
{
    return gStatusAreaWindowStub->statusArea();
}

void StatusAreaWindow::rotate(const Dui::Orientation &orientation)
{
    gStatusAreaWindowStub->rotate(orientation);
}


#endif
