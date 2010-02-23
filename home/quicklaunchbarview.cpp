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

#include "quicklaunchbarview.h"
#include "quicklaunchbar.h"
#include "launcher.h"
#include "mainwindow.h"

#include <QGraphicsLinearLayout>
#include <DuiButton>
#include <DuiViewCreator>

QuickLaunchBarView::QuickLaunchBarView(QuickLaunchBar *controller) : DuiWidgetView(controller)
{
    QGraphicsLinearLayout *l = new QGraphicsLinearLayout(Qt::Horizontal);
    l->setContentsMargins(0, 0, 0, 0);
    controller->setLayout(l);

    // A layout for the launcher buttons
    quickLaunchButtonLayout = new QGraphicsLinearLayout(Qt::Horizontal);
    quickLaunchButtonLayout->setContentsMargins(0, 0, 0, 0);

    // Create launcher button
    DuiButton *launcherButton = new DuiButton;
    launcherButton->setViewType("icon");
    launcherButton->setObjectName("ToggleLauncherButton");
    launcherButton->setIconID("icon-m-launcher");
    connect(launcherButton, SIGNAL(clicked()), controller, SIGNAL(launcherButtonClicked()));

    // Put the stuff into a layout
    l->addStretch();
    l->addItem(launcherButton);
    l->addItem(quickLaunchButtonLayout);
    l->addStretch();
    DuiButton *appletSpaceButton = new DuiButton("Applet Space");
    connect(appletSpaceButton, SIGNAL(clicked()), controller, SIGNAL(appletSpaceButtonClicked()));
    l->addItem(appletSpaceButton);
}

QuickLaunchBarView::~QuickLaunchBarView()
{
}

void QuickLaunchBarView::setupModel()
{
    QList<const char *> modifications;
    modifications << QuickLaunchBarModel::Widgets;
    updateData(modifications);
}

void QuickLaunchBarView::updateData(const QList<const char *>& modifications)
{
    DuiWidgetView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == QuickLaunchBarModel::Widgets) {
            // Remove everything from the launcher button layout
            while (quickLaunchButtonLayout->count() > 0) {
                quickLaunchButtonLayout->removeAt(0);
            }

            // Add all launcher buttons to the launcher button layout
            foreach (DuiWidget *widget, model()->widgets()) {
                quickLaunchButtonLayout->addItem(widget);
            }
        }
    }
}

DUI_REGISTER_VIEW_NEW(QuickLaunchBarView, QuickLaunchBar)
