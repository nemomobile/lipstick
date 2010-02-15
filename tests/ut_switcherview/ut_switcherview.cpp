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

#include <DuiApplication>
#include <DuiWindow>
#include "ut_switcherview.h"
#include "switcherview.h"
#include "switcher_stub.h"

// DuiWidgetView stubs
SwitcherModel switcherModel;
DuiWidgetModel *DuiWidgetView::model()
{
    return &switcherModel;
}

const DuiWidgetModel *DuiWidgetView::model() const
{
    return &switcherModel;
}

// DuiWidgetView stubs
QRectF DuiWidgetView::boundingRect() const
{
    return QRectF(0, 0, 0, 0);
}

void Ut_SwitcherView::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_switcherview";
    app = new DuiApplication(argc, &app_name);
    window = new DuiWindow;
}

void Ut_SwitcherView::cleanupTestCase()
{
    delete window;
    delete app;
}

void Ut_SwitcherView::init()
{
    // Create test switcher
    switcher = new Switcher();
    m_subject = new SwitcherView(switcher);
    switcher->setView(m_subject);
    gSwitcherStub->stubReset();
}

void Ut_SwitcherView::cleanup()
{
    delete m_subject;
}

QTEST_APPLESS_MAIN(Ut_SwitcherView)
