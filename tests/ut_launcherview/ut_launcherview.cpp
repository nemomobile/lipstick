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
#include "ut_launcherview.h"
#include "launcher.h"
#include "launcherbutton_stub.h"
#include "homeapplication_stub.h"
#include "mainwindow_stub.h"
#include "launcherview.h"

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

void Ut_LauncherView::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_launcherview";
    app = new DuiApplication(argc, &app_name);
}

void Ut_LauncherView::cleanupTestCase()
{
    delete app;
}

void Ut_LauncherView::init()
{
    controller = new Launcher;
    view = new LauncherView(controller);
    controller->setView(view);
    showWindowCount = 0;
    hideWindowCount = 0;
}

void Ut_LauncherView::cleanup()
{
    delete controller;
}

void Ut_LauncherView::testSwitchingToSubCategory()
{
    // Switch to subcategory
    controller->model()->setCategory(LauncherModel::SubCategory);
    QCOMPARE(showWindowCount, 1);
}

void Ut_LauncherView::testSwitchingToRootCategory()
{
    // Switch to subcategory
    controller->model()->setCategory(LauncherModel::SubCategory);
    QCOMPARE(hideWindowCount, 0);

    // Switch back to root category
    controller->model()->setCategory(LauncherModel::RootCategory);
    QCOMPARE(hideWindowCount, 1);
}

void Ut_LauncherView::testSetWidgets()
{
    DuiWidget *widget1 = new DuiWidget;
    DuiWidget *widget2 = new DuiWidget;
    QList<DuiWidget *> widgets;
    widgets.append(widget1);
    widgets.append(widget2);
    controller->model()->setWidgets(widgets);

    QGraphicsLinearLayout *mainLayout = dynamic_cast<QGraphicsLinearLayout *>(controller->layout());
    QVERIFY(mainLayout != NULL);
    QCOMPARE(mainLayout->count(), 1);
    DuiLayout *layout = dynamic_cast<DuiLayout *>(mainLayout->itemAt(0));
    QVERIFY(layout != NULL);
    QCOMPARE(layout->count(), 2);
    QCOMPARE(layout->itemAt(0), widget1);
    QCOMPARE(layout->itemAt(1), widget2);
}

void Ut_LauncherView::testSetLayoutObjectName()
{
    controller->model()->setLayoutObjectName("TestName");
    QGraphicsLinearLayout *mainLayout = dynamic_cast<QGraphicsLinearLayout *>(controller->layout());
    QVERIFY(mainLayout != NULL);
    QCOMPARE(mainLayout->count(), 1);
    DuiLayout *layout = dynamic_cast<DuiLayout *>(mainLayout->itemAt(0));
    QCOMPARE(layout->policy()->objectName(), QString("TestName"));
}

QTEST_APPLESS_MAIN(Ut_LauncherView)
