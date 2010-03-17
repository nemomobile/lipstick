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

#include <DuiFlowLayoutPolicy>
#include <QGraphicsLinearLayout>
#include <DuiSceneManager>
#include "ut_launcherpageview.h"
#include "launcherpagemodel.h"
#include "launcherpagestyle.h"
#include "launcher.h"
#include "launcherbutton_stub.h"
#include "homeapplication_stub.h"
#include "mainwindow_stub.h"
#include "launcherpageview.h"

// DuiSceneWindow stubs
int showWindowCount = 0;
void DuiSceneWindow::appear(DuiSceneWindow::DeletionPolicy)
{
    showWindowCount++;
}

int hideWindowCount = 0;
void DuiSceneWindow::disappear()
{
    hideWindowCount++;
}

// DuiWindow stubs
DuiSceneManager *DuiWindow::sceneManager()
{
    return NULL;
}

void Ut_LauncherPageView::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_launcherpageview";
    app = new DuiApplication(argc, &app_name);
}

void Ut_LauncherPageView::cleanupTestCase()
{
    delete app;
}

void Ut_LauncherPageView::init()
{
    controller = new LauncherPage;
    view = new LauncherPageView(controller);
    controller->setView(view);
    showWindowCount = 0;
    hideWindowCount = 0;
}

void Ut_LauncherPageView::cleanup()
{
    delete controller;
}

void Ut_LauncherPageView::testAddButtonsToPage()
{
    QSharedPointer<LauncherButton> widget1(new LauncherButton);
    QSharedPointer<LauncherButton> widget2(new LauncherButton);
    LauncherPageModel::LauncherButtonList widgets;
    widgets.append(widget1);
    widgets.append(widget2);
    controller->model()->setLauncherButtons(widgets);

    DuiLayout* mainLayout = dynamic_cast<DuiLayout *>(controller->layout());
    QVERIFY(mainLayout != NULL);

    QCOMPARE(mainLayout->count(), 2);
    QCOMPARE(mainLayout->itemAt(0), widget1.data());
    QCOMPARE(mainLayout->itemAt(1), widget2.data());
}

QTEST_APPLESS_MAIN(Ut_LauncherPageView)
