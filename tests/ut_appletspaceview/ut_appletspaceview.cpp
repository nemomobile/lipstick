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

#include <MApplication>
#include <MWindow>
#include <MSceneWindow>
#include "ut_appletspaceview.h"
#include "appletspaceview.h"
#include "appletspace.h"

int Ut_AppletSpaceView::showWindowCount;
int Ut_AppletSpaceView::hideWindowCount;

// SceneWindow stubs
void MSceneWindow::appear(MSceneWindow::DeletionPolicy)
{
    Ut_AppletSpaceView::showWindowCount++;
}

void MSceneWindow::disappear()
{
    Ut_AppletSpaceView::hideWindowCount++;
}

void Ut_AppletSpaceView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_appletspaceview" };
    app = new MApplication(argc, app_name);
    window = new MWindow;
}

void Ut_AppletSpaceView::cleanupTestCase()
{
    delete window;
    delete app;
}

void Ut_AppletSpaceView::init()
{
    controller = new AppletSpace;
    view = new AppletSpaceView(controller);
    controller->setView(view);
    showWindowCount = 0;
    hideWindowCount = 0;
}

void Ut_AppletSpaceView::cleanup()
{
    delete controller;
}

void Ut_AppletSpaceView::testCloseButtonVisibility()
{
    controller->model()->setCloseButtonVisible(true);
    QCOMPARE(showWindowCount, 1);

    controller->model()->setCloseButtonVisible(false);
    QCOMPARE(hideWindowCount, 1);
}

QTEST_APPLESS_MAIN(Ut_AppletSpaceView)
