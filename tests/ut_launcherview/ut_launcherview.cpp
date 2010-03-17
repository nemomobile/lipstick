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
#include <DuiApplicationWindow>
#include <DuiFlowLayoutPolicy>
#include <QGraphicsLinearLayout>
#include <DuiSceneManager>
#include "ut_launcherview.h"
#include "launcher.h"
#include "launcherpage.h"
#include "launcherbuttonmodel.h"
#include "launcherbutton_stub.h"
#include "homeapplication_stub.h"
#include "mainwindow_stub.h"
#include "launcherview.h"
#include "pagedviewport.h"

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(PagedViewport)

// DuiSceneWindow stubs
int showWindowCount = 0;
int pageWidth = 0;

void DuiSceneWindow::appear(DuiSceneWindow::DeletionPolicy)
{
    showWindowCount++;
}

int hideWindowCount = 0;
void DuiSceneWindow::disappear()
{
    hideWindowCount++;
}

PagedViewport::PagedViewport(QGraphicsItem *parent) : DuiPannableViewport(parent)
{   

}

PagedViewport::~PagedViewport() {

}


//PagedViewport stubs
void PagedViewport::panToPage(uint page)
{
    Q_UNUSED(page)
}

void PagedViewport::updatePageWidth(int width)
{
    pageWidth = width;
}

void PagedViewport::setPanDirection(const Qt::Orientations &panDirection)
{
  Q_UNUSED(panDirection);
}

void Ut_LauncherView::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_launcherview";
    app = new DuiApplication(argc, &app_name);
    appWindow = new DuiApplicationWindow;
}

void Ut_LauncherView::cleanupTestCase()
{
    delete appWindow;
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


void Ut_LauncherView::testSetPageWidthUpdateWhenGeometryChanges()
{
    QRectF rect(0, 0, 100, 100);
    view->setGeometry(rect);    
    QCOMPARE(pageWidth, 100);
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

void Ut_LauncherView::testAddAndRemovePages()
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

    pages.removeAt(1);
    controller->model()->setLauncherPages(pages);

    QCOMPARE(layout->count(), 1);
}

QTEST_APPLESS_MAIN(Ut_LauncherView)
