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

#include "launcherpageview.h"
#include "launcherpage.h"
#include "launcherpagemodel.h"
#include "launcherbutton.h"
#include <MLayout>
#include <MFlowLayoutPolicy>

LauncherPageView::LauncherPageView(LauncherPage *controller) :
    MWidgetView(controller),
    layout(new MLayout),
    policy(new MFlowLayoutPolicy(layout))
{
    policy->setObjectName("LauncherPageFlowLayoutPolicy");
    controller->setLayout(layout);
}

LauncherPageView::~LauncherPageView()
{
}

void LauncherPageView::updateData(const QList<const char *>& modifications)
{
    MWidgetView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == LauncherPageModel::LauncherButtons) {
            updateLayoutFromButtonList();
        }
    }
}

void LauncherPageView::updateLayoutFromButtonList()
{
    QList<QSharedPointer<LauncherButton> > buttons = model()->launcherButtons();

    // Add new buttons (buttons that are found from button list but not from layout)
    foreach(QSharedPointer<LauncherButton> listButton, buttons) {
        bool contains = false;
        for (int i = 0; i < layout->count(); i++) {
            LauncherButton *layoutButton = dynamic_cast<LauncherButton *> (layout->itemAt(i));
            if (listButton.data() == layoutButton) {
                contains = true;
                break;
            }
        }

        if (!contains) {
            policy->addItem(listButton.data());
        }
    }

    // Remove non-existent pages (pages that are in layout but not in page list)
    for (int i = 0; i < layout->count(); i++) {
        LauncherButton *layoutButton = dynamic_cast<LauncherButton *> (layout->itemAt(i));
        bool contains = false;
        foreach(QSharedPointer<LauncherButton> listButton, buttons) {
            if (listButton.data() == layoutButton) {
                contains = true;
                break;
            }
        }

        if (!contains) {
            layout->removeItem(layoutButton);
        }
    }
}

void LauncherPageView::setupModel()
{
    MWidgetView::setupModel();

    QList<const char *> modifications;
    modifications << LauncherPageModel::LauncherButtons;
    updateData(modifications);
}

M_REGISTER_VIEW_NEW(LauncherPageView, LauncherPage)
