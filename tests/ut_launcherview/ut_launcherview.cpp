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
#include <MApplicationWindow>
#include <MFlowLayoutPolicy>
#include <QGraphicsLinearLayout>
#include <MSceneManager>
#include "ut_launcherview.h"
#include "homewindowmonitor.h"
#include "launcher.h"
#include "launcherpage.h"
#include "launcherbuttonmodel.h"
#include "launcherbutton_stub.h"
#include "launcheraction_stub.h"
#include "launcherdatastore_stub.h"
#include "applicationpackagemonitor_stub.h"
#include "applicationpackagemonitorlistener_stub.h"
#include "homeapplication_stub.h"
#include "mainwindow_stub.h"
#include "launcherview.h"
#include "pagedviewport_stub.h"
#include "mockdatastore.h"
#include "pagepositionindicatorview.h"
#include "mpositionindicator.h"
#include "windowinfo_stub.h"
#include "x11wrapper_stub.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(PagedViewport)

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


void Ut_LauncherView::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_launcherview";
    app = new MApplication(argc, &app_name);
    appWindow = new MApplicationWindow;
}

void Ut_LauncherView::cleanupTestCase()
{
    delete appWindow;
    delete app;

}

void Ut_LauncherView::init()
{
    launcherDataStore = new LauncherDataStore(new MockDataStore, QStringList());
    controller = new Launcher;
    controller->setLauncherDataStore(launcherDataStore);
    controller->setApplicationPackageMonitorListener(new ApplicationPackageMonitorListener);
    view = new LauncherView(controller);
    controller->setView(view);
    showWindowCount = 0;
    hideWindowCount = 0;
    connect(this, SIGNAL(updateDataRequested(const QList<const char *>&)),
            view, SLOT(updateData(const QList<const char *>&)));
    gPagedViewportStub->stubReset();
}

void Ut_LauncherView::cleanup()
{
    if (controller != NULL) {
        delete controller;
        controller = NULL;
    }
    delete launcherDataStore;
}

void Ut_LauncherView::testPagedViewportObjectName()
{
    PagedViewport *viewport = dynamic_cast<PagedViewport *>(controller->childItems().at(0));
    QVERIFY(viewport != NULL);
    QCOMPARE(viewport->objectName(), QString("LauncherPagedViewport"));
}

void Ut_LauncherView::testSetButtons()
{
    QSharedPointer<LauncherButton> button1(new LauncherButton(""));
    QSharedPointer<LauncherButton> button2(new LauncherButton(""));
    QList< QSharedPointer<LauncherPage> > pages;
    QSharedPointer<LauncherPage> page(new LauncherPage());
    page->appendButton(button1);
    page->appendButton(button2);
    pages.append(page);
    controller->model()->setLauncherPages(pages);

    PagedViewport *viewport = dynamic_cast<PagedViewport *>(controller->childItems().at(0));
    QVERIFY(viewport != NULL);

    QGraphicsWidget* pannedWidget = viewport->widget();
    QVERIFY(pannedWidget != NULL);

    QGraphicsLayout* layout = pannedWidget->layout();

    QCOMPARE(layout->count(), 1);
    QCOMPARE(layout->itemAt(0), page.data());
}

void Ut_LauncherView::testAddPages()
{
    static const int NUM_ITEMS = 10;
    // add two pages
    QList< QSharedPointer<LauncherPage> > pages;
    for (int iter1 = 0; iter1 < NUM_ITEMS; iter1++){
        QSharedPointer<LauncherPage> page(new LauncherPage());
        for (int iter2 = 0; iter2 < NUM_ITEMS; iter2++) {
            QSharedPointer<LauncherButton> button(new LauncherButton(""));
            page->appendButton(button);
        }
        pages.append(page);
    }
    controller->model()->setLauncherPages(pages);

    PagedViewport *viewport = dynamic_cast<PagedViewport *>(controller->childItems().at(0));
    QVERIFY(viewport != NULL);

    QGraphicsWidget* pannedWidget = viewport->widget();
    QVERIFY(pannedWidget != NULL);

    QGraphicsLayout* layout = pannedWidget->layout();

    QCOMPARE(layout->count(), NUM_ITEMS);
    // verify layout order
    for (int i = 0; i < NUM_ITEMS; ++i) {
        QCOMPARE(layout->itemAt(i), pages.at(i).data());
    }
}

void Ut_LauncherView::testRemovingPages()
{
    QList< QSharedPointer<LauncherPage> > pages;
    QSharedPointer<LauncherPage> page1(new LauncherPage());
    QSharedPointer<LauncherPage> page2(new LauncherPage());
    pages.append(page1);
    pages.append(page2);
    controller->model()->setLauncherPages(pages);

    pages.removeOne(page1);
    controller->model()->setLauncherPages(pages);

    QGraphicsLayout* layout = dynamic_cast<PagedViewport *>(controller->childItems().at(0))->widget()->layout();
    QCOMPARE(layout->count(), 1);
    // verify that page is not deleted when there is still ref in QSharedPointer
    QVERIFY(!page1.isNull());
}

void Ut_LauncherView::testRemovingPagesFromLayoutInDestructor()
{
    QSharedPointer<LauncherPage> page(new LauncherPage);
    LauncherModel::LauncherPageList pages;
    pages.append(page);
    controller->model()->setLauncherPages(pages);

    delete controller;
    controller = NULL;
    // verify that page is not deleted when there is still ref in QSharedPointer
    QVERIFY(!page.isNull());
}

void Ut_LauncherView::testSignalConnection()
{
    QVERIFY(disconnect(controller, SIGNAL(focusToPageRequested(uint)), view, SLOT(focusToPage(uint))));
    connect(controller, SIGNAL(focusToPageRequested(uint)), view, SLOT(focusToPage(uint)));
}

void Ut_LauncherView::testSetPage()
{
    // Test that focusToFirstPageRequested() signal goes to correct slot from
    // which pagedViewport->setPage() is called.
    controller->setPage(2);

    QCOMPARE(gPagedViewportStub->stubCallCount("setPage"), 1);
    QCOMPARE(gPagedViewportStub->stubLastCallTo("setPage").parameter<uint>(0), (uint)2);
}

void Ut_LauncherView::testUpdateData()
{
    QSharedPointer<LauncherPage> page1(new LauncherPage);
    QSharedPointer<LauncherPage> page2(new LauncherPage);
    LauncherModel::LauncherPageList pages;
    pages.append(page1);
    pages.append(page2);
    controller->model()->setLauncherPages(pages);

    MLayout* mainLayout = dynamic_cast<MLayout *>(
        dynamic_cast<PagedViewport *>(
            controller->childItems().at(0))->widget()->layout());
    QVERIFY(mainLayout != NULL);
    QCOMPARE(mainLayout->count(), 2);
    mainLayout->removeItem(page1.data());
    mainLayout->removeItem(page2.data());
    QCOMPARE(mainLayout->count(), 0);

    QList<const char*> modifications;
    modifications.append(LauncherModel::LauncherPages);
    emit updateDataRequested(modifications);
    QCOMPARE(mainLayout->count(), 2);

    mainLayout->removeItem(page1.data());
    mainLayout->removeItem(page2.data());
    QCOMPARE(mainLayout->count(), 0);

    QList<const char*> modifications2;
    modifications.append("does not match");
    emit updateDataRequested(modifications2);
    QCOMPARE(mainLayout->count(), 0);
}

void Ut_LauncherView::testFocusToButton()
{
    const QString desktop1 = "button1.desktop";
    QSharedPointer<LauncherPage> page1(new LauncherPage);
    QSharedPointer<LauncherPage> page2(new LauncherPage);
    QSharedPointer<LauncherButton> button1(new LauncherButton(desktop1));
    page2->appendButton(button1);

    LauncherModel::LauncherPageList pages;
    pages.append(page1);
    pages.append(page2);
    controller->model()->setLauncherPages(pages);

    gLauncherButtonStub->stubSetReturnValue("desktopEntry", desktop1);
    view->focusToButton(desktop1);
    QCOMPARE(gPagedViewportStub->stubLastCallTo("setPage").parameter<uint>(0), (uint)1);

    gPagedViewportStub->stubReset();
    view->focusToButton("");
    QCOMPARE(gPagedViewportStub->stubCallCount("setPage"), 0);
}


QTEST_APPLESS_MAIN(Ut_LauncherView)
