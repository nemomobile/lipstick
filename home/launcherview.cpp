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

#include "launcherview.h"
#include "launcher.h"
#include <QGraphicsLinearLayout>
#include <DuiFlowLayoutPolicy>
#include <DuiOverlay>
#include <DuiButton>

LauncherView::LauncherView(Launcher *container) :
    DuiExtendingBackgroundView(container),
    controller(container),
    mainLayout(new QGraphicsLinearLayout(Qt::Vertical)),
    layout(new DuiLayout),
    policy(new DuiFlowLayoutPolicy(layout)),
    backButtonOverlay(new DuiOverlay),
    backButton(new DuiButton(backButtonOverlay))
{
    controller->setLayout(mainLayout);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Create layout for the launcher buttons
    layout->setContentsMargins(0, 0, 0, 0);
    policy->setObjectName("LauncherFlowLayoutPolicy");
    mainLayout->addItem(layout);

    // Set close button properties
    backButton->setViewType("icon");
    backButton->setObjectName("LauncherBackButton");
    backButton->setIconID("Icon-back");
    connect(backButton, SIGNAL(clicked()), controller, SLOT(openRootCategory()));
    backButtonOverlay->setObjectName("LauncherBackButtonOverlay");
}

LauncherView::~LauncherView()
{
    delete backButtonOverlay;
}

void LauncherView::updateData(const QList<const char *>& modifications)
{
    DuiWidgetView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == LauncherModel::Widgets) {
            // Remove all widgets from the layout (do not destroy them)
            while (layout->count() > 0) {
                layout->removeAt(0);
            }

            // Add widgets from the model to the layout
            foreach(DuiWidget * widget, model()->widgets()) {
                policy->addItem(widget);
            }
        } else if (member == LauncherModel::LayoutObjectName) {
            // Set the name of the layout
            policy->setObjectName(model()->layoutObjectName());
        } else if (member == LauncherModel::Category) {
            // Set the visibility of the back button
            if (model()->category() == LauncherModel::RootCategory) {
                backButtonOverlay->disappear();
            } else {
                backButtonOverlay->appear();
            }
        }
    }
}

DUI_REGISTER_VIEW_NEW(LauncherView, Launcher)
