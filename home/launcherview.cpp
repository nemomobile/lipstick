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
#include <QGraphicsLinearLayout>
#include <MFlowLayoutPolicy>
#include <MOverlay>
#include <MButton>
#include <MApplication>
#include <MSceneManager>

LauncherView::LauncherView(Launcher *controller) :
    MWidgetView(controller),
    layout(new QGraphicsLinearLayout(Qt::Horizontal)),
    pannedWidget(new MWidget)
{
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    pannedWidget->setLayout(layout);

    pagedViewport = new PagedViewport(controller);

    pagedViewport->positionIndicator()->setObjectName("LauncherPageIndicator");

    pagedViewport->setWidget(pannedWidget);
    pagedViewport->setObjectName("LauncherPagedViewport");
    pagedViewport->setPanDirection(Qt::Horizontal);
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
            // Remove all pages from the layout (do not destroy them)
            while (layout->count() > 0) {
                layout->removeAt(0);
            }

            // Add pages from the model to the layout
            foreach(QSharedPointer< LauncherPage > page, model()->launcherPages()) {
                layout->addItem(page.data());
            }

            pagedViewport->updatePageCount(model()->launcherPages().count());
        }
    }
}

M_REGISTER_VIEW_NEW(LauncherView, Launcher)
