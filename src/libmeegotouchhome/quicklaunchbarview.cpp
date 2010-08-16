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

#include "quicklaunchbarview.h"
#include "quicklaunchbar.h"
#include "launcher.h"
#include "mainwindow.h"

#include <QGraphicsLinearLayout>
#include <MButton>
#include <MViewCreator>

const int QuickLaunchBarView::TOGGLE_LAUNCHER_BUTTON_INDEX = 2;

QuickLaunchBarView::QuickLaunchBarView(QuickLaunchBar *controller) : MWidgetView(controller),
    launcherButtonLayout(new QGraphicsLinearLayout(Qt::Horizontal)),
    toggleLauncherButton(new MButton)
{
    QGraphicsLinearLayout *l = new QGraphicsLinearLayout(Qt::Horizontal);
    l->setContentsMargins(0, 0, 0, 0);
    controller->setLayout(l);

    // A layout for the launcher buttons
    launcherButtonLayout->setContentsMargins(0, 0, 0, 0);

    // Create launcher button
    toggleLauncherButton->setViewType("icon");
    toggleLauncherButton->setObjectName("ToggleLauncherButton");
    toggleLauncherButton->setIconID("icon-m-quicklaunchbar-menu-open");
    connect(toggleLauncherButton, SIGNAL(clicked()), controller, SIGNAL(toggleLauncherButtonClicked()));

    // Put the stuff into a layout
    l->addStretch();
    l->addItem(launcherButtonLayout);
    l->addStretch();
}

QuickLaunchBarView::~QuickLaunchBarView()
{
}

void QuickLaunchBarView::setupModel()
{
    MWidgetView::setupModel();
    QList<const char *> modifications;
    modifications << QuickLaunchBarModel::Widgets;
    updateData(modifications);
}

void QuickLaunchBarView::updateData(const QList<const char *>& modifications)
{
    MWidgetView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == QuickLaunchBarModel::Widgets) {
            // Remove everything from the launcher button layout
            while (launcherButtonLayout->count() > 0) {
                launcherButtonLayout->removeAt(0);
            }

            // Add all launcher buttons to the launcher button layout
            foreach (MWidget *widget, model()->widgets()) {
                launcherButtonLayout->addItem(widget);

                if (launcherButtonLayout->count() == TOGGLE_LAUNCHER_BUTTON_INDEX) {
                    // Add a launcher button in the expected index
                    launcherButtonLayout->addItem(toggleLauncherButton);
                }
            }
        }
    }
}

M_REGISTER_VIEW_NEW(QuickLaunchBarView, QuickLaunchBar)
