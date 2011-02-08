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

#include <MFlowLayoutPolicy>
#include <QGraphicsLinearLayout>
#include <MSceneManager>
#include "ut_launcherpageview.h"
#include "homewindowmonitor.h"
#include "launcherpagemodel.h"
#include "launcherpagestyle.h"
#include "launcher.h"
#include "launcheraction_stub.h"
#include "launcherbutton_stub.h"
#include "homeapplication_stub.h"
#include "mainwindow_stub.h"
#include "launcherpageview.h"
#include "windowinfo_stub.h"
#include "x11wrapper_stub.h"

// MSceneWindow stubs
int showWindowCount = 0;
void MSceneWindow::appear(MSceneWindow::DeletionPolicy)
{
    showWindowCount++;
}

int hideWindowCount = 0;
void MSceneWindow::disappear()
{
    hideWindowCount++;
}

// MWindow stubs
MSceneManager *MWindow::sceneManager()
{
    return NULL;
}

void Ut_LauncherPageView::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_launcherpageview";
    app = new MApplication(argc, &app_name);
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
    connect(this, SIGNAL(updateDataRequested(const QList<const char *>&)),
            view, SLOT(updateData(const QList<const char *>&)));
}

void Ut_LauncherPageView::cleanup()
{
    if (controller != NULL) {
        delete controller;
        controller = NULL;
    }
    gLauncherButtonStub->stubReset();
}

void Ut_LauncherPageView::testAddButtonsToPage()
{
    QSharedPointer<LauncherButton> widget1(new LauncherButton(""));
    QSharedPointer<LauncherButton> widget2(new LauncherButton(""));
    QSharedPointer<LauncherButton> widget3(new LauncherButton(""));
    QSharedPointer<LauncherButton> widget4(new LauncherButton(""));
    LauncherPageModel::LauncherButtonList widgets;
    widgets.append(widget1);
    controller->model()->setLauncherButtons(widgets);
    // add several widgets so that the order is tested better
    widgets.append(widget2);
    widgets.append(widget3);
    widgets.append(widget4);
    controller->model()->setLauncherButtons(widgets);

    MLayout* mainLayout = dynamic_cast<MLayout *>(controller->layout());
    QVERIFY(mainLayout != NULL);

    QCOMPARE(mainLayout->count(), widgets.count());
    for (int i = 0; i < mainLayout->count(); i++) {
        QCOMPARE(mainLayout->itemAt(i), widgets.at(i).data());
    }
}

void Ut_LauncherPageView::testRemovingButtonFromLayout()
{
    QSharedPointer<LauncherButton> widget1(new LauncherButton(""));
    QSharedPointer<LauncherButton> widget2(new LauncherButton(""));
    LauncherPageModel::LauncherButtonList widgets;
    widgets.append(widget1);
    widgets.append(widget2);
    controller->model()->setLauncherButtons(widgets);

    widgets.removeOne(widget1);
    controller->model()->setLauncherButtons(widgets);

    MLayout* mainLayout = dynamic_cast<MLayout *>(controller->layout());
    QVERIFY(mainLayout != NULL);
    QCOMPARE(mainLayout->count(), 1);
    QCOMPARE(mainLayout->itemAt(0), widget2.data());
    // verify that button destructor has not been called when there is still ref in QSharedPointer
    QCOMPARE(gLauncherButtonStub->stubCallCount("~LauncherButton"), 0);
}

void Ut_LauncherPageView::testRemovingButtonFromLayoutInDestructor()
{
    QSharedPointer<LauncherButton> widget(new LauncherButton(""));
    LauncherPageModel::LauncherButtonList widgets;
    widgets.append(widget);
    controller->model()->setLauncherButtons(widgets);

    delete controller;
    controller = NULL;

    // verify that button destructor has not been called when there is still ref in QSharedPointer
    QCOMPARE(gLauncherButtonStub->stubCallCount("~LauncherButton"), 0);
}

void Ut_LauncherPageView::testUpdateData()
{
    QSharedPointer<LauncherButton> widget1(new LauncherButton(""));
    QSharedPointer<LauncherButton> widget2(new LauncherButton(""));
    LauncherPageModel::LauncherButtonList widgets;
    widgets.append(widget1);
    widgets.append(widget2);
    controller->model()->setLauncherButtons(widgets);

    MLayout* mainLayout = dynamic_cast<MLayout *>(controller->layout());
    QVERIFY(mainLayout != NULL);
    QCOMPARE(mainLayout->count(), 2);
    mainLayout->removeItem(widget1.data());
    mainLayout->removeItem(widget2.data());
    QCOMPARE(mainLayout->count(), 0);

    QList<const char*> modifications;
    modifications.append(LauncherPageModel::LauncherButtons);
    emit updateDataRequested(modifications);
    QCOMPARE(mainLayout->count(), 2);

    mainLayout->removeItem(widget1.data());
    mainLayout->removeItem(widget2.data());
    QCOMPARE(mainLayout->count(), 0);

    QList<const char*> modifications2;
    modifications.append("does not match");
    emit updateDataRequested(modifications2);
    QCOMPARE(mainLayout->count(), 0);
}

QTEST_APPLESS_MAIN(Ut_LauncherPageView)
