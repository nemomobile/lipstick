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
}

LauncherView::~LauncherView()
{
}

void LauncherView::updateData(const QList<const char *>& modifications)
{
    MWidgetView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == LauncherModel::LauncherPages) {
            updateLayoutFromPageList();

            pagedViewport->updatePageCount(model()->launcherPages().count());
        }
    }
}

void LauncherView::updateLayoutFromPageList()
{
    QList<QSharedPointer<LauncherPage> > pages = model()->launcherPages();

    // Add new pages (pages that are found from page list but not from layout)
    foreach(QSharedPointer<LauncherPage> listPage, pages) {
        bool contains = false;
        for (int i = 0; i < layout->count(); i++) {
            LauncherPage *layoutPage = dynamic_cast<LauncherPage *> (layout->itemAt(i));
            if (listPage.data() == layoutPage) {
                contains = true;
                break;
            }
        }

        if (!contains) {
            policy->addItem(listPage.data());
        }
    }

    // Remove pages that doesn't exist anymore
    for (int i = 0; i < layout->count(); i++) {
        bool contains = false;
        LauncherPage *layoutPage = dynamic_cast<LauncherPage *> (layout->itemAt(i));
        foreach(QSharedPointer<LauncherPage> listPage, pages) {
            if (listPage.data() == layoutPage) {
                contains = true;
                break;
            }
        }

        if (!contains) {
            layout->removeItem(layoutPage);
        }
    }
}

void LauncherView::panToPage(uint page)
{
    pagedViewport->panToPage(page);
}

M_REGISTER_VIEW_NEW(LauncherView, Launcher)
