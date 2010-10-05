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

#include "launcherview.h"
#include "launcher.h"
#include "launchermodel.h"
#include "launcherbutton.h"
#include "pagedviewport.h"
#include "mainwindow.h"
#include "mpositionindicator.h"
#include "pagepositionindicatorview.h"
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MOverlay>
#include <MButton>
#include <MApplication>
#include <MSceneManager>

LauncherView::LauncherView(Launcher *controller) :
    MWidgetView(controller),
    layout(new MLayout),
    policy(new MLinearLayoutPolicy(layout, Qt::Horizontal)),
    pannedWidget(new MWidget)
{
    layout->setContentsMargins(0, 0, 0, 0);
    pannedWidget->setLayout(layout);

    pagedViewport = new PagedViewport(controller);

    pagedViewport->positionIndicator()->setObjectName("LauncherPageIndicator");

    pagedViewport->setWidget(pannedWidget);
    pagedViewport->setObjectName("LauncherPagedViewport");
    pagedViewport->setPanDirection(Qt::Horizontal);

    connect(controller, SIGNAL(panningRequested(uint)), this, SLOT(panToPage(uint)));
    connect(controller, SIGNAL(focusToPageRequested(uint)), this, SLOT(focusToPage(uint)));
    connect(controller, SIGNAL(focusToButtonRequested(const QString &)), this, SLOT(focusToButton(const QString&)));
}

LauncherView::~LauncherView()
{
    // remove pages from layout to avoid multi deletion (pages are in model as QSharedPointer's)
    foreach (QSharedPointer<LauncherPage> page, model()->launcherPages()) {
        removePageFromLayout(page.data());
    }
}

void LauncherView::updateData(const QList<const char *>& modifications)
{
    MWidgetView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == LauncherModel::LauncherPages) {
            updateLayoutFromModel();

            pagedViewport->updatePageCount(model()->launcherPages().count());
        }
    }
}

void LauncherView::updateLayoutFromModel()
{
    // Set of pages in layout
    QSet<LauncherPage *> layoutPages;
    for (int i = 0; i < layout->count(); i++) {
        layoutPages.insert(static_cast<LauncherPage *>(layout->itemAt(i)));
    }

    // List of pages in model (list needed for the order of pages)
    QList<LauncherPage *> modelPagesList;
    foreach(QSharedPointer<LauncherPage> modelPage, model()->launcherPages()) {
        modelPagesList.append(modelPage.data());
    }
    // Set of pages in model
    QSet<LauncherPage *> modelPagesSet(modelPagesList.toSet());

    // Remove from layout the pages that doesn't exists any more in model
    QSet<LauncherPage *> deletedPages = layoutPages - modelPagesSet;
    foreach (LauncherPage *deletedPage, deletedPages) {
        removePageFromLayout(deletedPage);
    }

    // Get pages that are new in model to a map (with model list index as a key) for sorting
    QSet<LauncherPage *> newPages = modelPagesSet - layoutPages;
    QMap<int, LauncherPage *> newPagesSorted;
    foreach (LauncherPage *addedPage, newPages) {
        newPagesSorted.insert(modelPagesList.indexOf(addedPage), addedPage);
    }

    // Add new pages to layout in sorted order
    QMapIterator<int, LauncherPage *> iter(newPagesSorted);
    while (iter.hasNext()) {
        iter.next();
        policy->insertItem(iter.key(), iter.value());
    }
}

void LauncherView::panToPage(uint page)
{
    pagedViewport->panToPage(page);
}

void LauncherView::focusToPage(uint page)
{
    pagedViewport->setPage(page);
}

void LauncherView::focusToButton(const QString &desktopFileEntry)
{
    int pageNum = -1;
    uint pageIndex = 0;
    QList<QSharedPointer<LauncherPage> > pages = model()->launcherPages();
    foreach (QSharedPointer<LauncherPage> page, pages) {
        if (page->launcherButtonPosition(desktopFileEntry) >= 0) {
            pageNum = pageIndex;
            break;
        }
        pageIndex++;
    }
    if (pageNum >= 0) {
        pagedViewport->setPage(pageNum);
    }
}

void LauncherView::removePageFromLayout(LauncherPage *page)
{
    layout->removeItem(page);
    // set parent to NULL to avoid double deletion as items are as QSharedPointers in model
    page->setParentItem(0);
}

M_REGISTER_VIEW_NEW(LauncherView, Launcher)
