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
#include "launcher.h"
#include "launcherpage.h"
#include "launcherbuttonmodel.h"
#include "launcherbutton_stub.h"
#include "launcheraction_stub.h"
#include "launcherdatastore_stub.h"
#include "homeapplication_stub.h"
#include "mainwindow_stub.h"
#include "launcherview.h"
#include "pagedviewport.h"
#include "mockdatastore.h"
#include "pagepositionindicatorview.h"
#include "mpositionindicator.h"
#include "windowinfo_stub.h"
#include "x11wrapper_stub.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(PagedViewport)

// MSceneWindow stubs
int showWindowCount = 0;
int pageWidth = 0;
int pageCount = 0;

void MSceneWindow::appear(MSceneWindow::DeletionPolicy)
{
    showWindowCount++;
}

int hideWindowCount = 0;
void MSceneWindow::disappear()
{
    hideWindowCount++;
}

//PagedViewport stubs
PagedViewport::PagedViewport(QGraphicsItem *parent) : MPannableViewport(parent),
        pagedPanning(NULL)
{
}

PagedViewport::~PagedViewport() {
}

void PagedViewport::panToPage(uint page)
{
    Q_UNUSED(page)
}

bool gFocusFirstPageCalled = false;
void PagedViewport::focusFirstPage() {
    gFocusFirstPageCalled = true;
}

/*
void PagedViewport::updatePageWidth(int width)
{
    pageWidth = width;
}
*/

void PagedViewport::updatePageCount(int pages)
{
    pageCount = pages;
}

void PagedViewport::setPanDirection(const Qt::Orientations &panDirection)
{
  Q_UNUSED(panDirection);
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
    launcherDataStore = new LauncherDataStore(new MockDataStore);
    controller = new Launcher(launcherDataStore);
    view = new LauncherView(controller);
    controller->setView(view);
    showWindowCount = 0;
    hideWindowCount = 0;
    gFocusFirstPageCalled = false;
}

void Ut_LauncherView::cleanup()
{
    if (controller == NULL) {
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
    QSharedPointer<LauncherButton> button1(new LauncherButton());
    QSharedPointer<LauncherButton> button2(new LauncherButton());
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
    // add two pages
    QList< QSharedPointer<LauncherPage> > pages;
    for (int iter1 = 0; iter1 < 2; iter1++){
        QSharedPointer<LauncherPage> page(new LauncherPage());
        for (int iter2 = 0; iter2 < 10; iter2++) {
            QSharedPointer<LauncherButton> button(new LauncherButton());
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

    QCOMPARE(layout->count(), 2);
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
    controller == NULL;
    // verify that page is not deleted when there is still ref in QSharedPointer
    QVERIFY(!page.isNull());
}

void Ut_LauncherView::testSignalConnection()
{
    QVERIFY(disconnect(controller, SIGNAL(focusToFirstPageRequested()), view, SLOT(focusFirstPage())));
    connect(controller, SIGNAL(focusToFirstPageRequested()), view, SLOT(focusFirstPage()));
}

void Ut_LauncherView::testFocusFirstPage()
{
    // Test that focusToFirstPageRequested() signal goes to correct slot from
    // which pagedViewport->focusFirstPage() is called
    controller->setFirstPage();
    QCOMPARE(gFocusFirstPageCalled, true);
}

QTEST_APPLESS_MAIN(Ut_LauncherView)
