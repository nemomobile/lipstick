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

#include "launcherpageview.h"
#include "launcherpage.h"
#include "launcherpagemodel.h"
#include "launcherbutton.h"
#include <DuiLayout>
#include <DuiFlowLayoutPolicy>

LauncherPageView::LauncherPageView(LauncherPage *controller) :
    DuiWidgetView(controller),
    layout(new DuiLayout),
    policy(new DuiFlowLayoutPolicy(layout))
{
    policy->setObjectName("LauncherPageFlowLayoutPolicy");
    controller->setLayout(layout);
}

LauncherPageView::~LauncherPageView()
{
}

void LauncherPageView::updateData(const QList<const char *>& modifications)
{
    DuiWidgetView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == LauncherPageModel::LauncherButtons) {
            // Remove all widgets from the layout (do not destroy them)
            while (layout->count() > 0) {
                layout->removeAt(0);
            }

            // Add widgets from the model to the layout
            foreach(QSharedPointer< LauncherButton > button, model()->launcherButtons()) {
                policy->addItem(button.data());
            }
        }
    }
}

void LauncherPageView::setupModel()
{
    DuiWidgetView::setupModel();

    QList<const char *> modifications;
    modifications << LauncherPageModel::LauncherButtons;
    updateData(modifications);
}

DUI_REGISTER_VIEW_NEW(LauncherPageView, LauncherPage)
