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

QuickLaunchBarView::QuickLaunchBarView(QuickLaunchBar *controller) : MWidgetView(controller),
    launcherButtonLayout(new QGraphicsLinearLayout(Qt::Horizontal)),
    toggleLauncherButton(new MButton)
{
    QGraphicsLinearLayout *l = new QGraphicsLinearLayout(Qt::Horizontal);
    l->setContentsMargins(0, 0, 0, 0);
    l->setSpacing(0);
    controller->setLayout(l);

    // A layout for the launcher buttons
    launcherButtonLayout->setContentsMargins(0, 0, 0, 0);
    launcherButtonLayout->setSpacing(0);

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
    // remove buttons from layout to avoid multi deletion (buttons are in model as QSharedPointer's)
    foreach (QSharedPointer<LauncherButton> button, model()->buttons().values()) {
        launcherButtonLayout->removeItem(button.data());
        button->setParentItem(0);
    }
}

void QuickLaunchBarView::applyStyle()
{
    MWidgetView::applyStyle();
    setupModel();
}

void QuickLaunchBarView::setupModel()
{
    MWidgetView::setupModel();
    QList<const char *> modifications;
    modifications << QuickLaunchBarModel::Buttons;
    updateData(modifications);
}

void QuickLaunchBarView::updateData(const QList<const char *>& modifications)
{
    MWidgetView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == QuickLaunchBarModel::Buttons) {
            // Remove everything from the launcher button layout
            while (launcherButtonLayout->count() > 0) {
                QGraphicsObject* obj = launcherButtonLayout->itemAt(0)->graphicsItem()->toGraphicsObject();
                launcherButtonLayout->removeAt(0);
                // If this was a placeholder widget, delete it
                if (obj && obj->property("placeholder").isValid()) {
                    delete obj;
                }
            }

            // Add launcher buttons to the layout and empty placeholder items into positions that have no buttons
            for (int buttonIndex = 0, layoutIndex = 0; buttonIndex < style()->buttonCount(); layoutIndex++) {
                if (layoutIndex == style()->toggleLauncherButtonIndex()) {
                    launcherButtonLayout->addItem(toggleLauncherButton);
                } else if (model()->buttons().contains(buttonIndex)) {
                    launcherButtonLayout->addItem(model()->buttons().value(buttonIndex).data());
                    buttonIndex++;
                } else {
                    MWidget *widget = new MWidget();
                    launcherButtonLayout->addItem(widget);
                    buttonIndex++;
                    // Mark this as a placeholder quick launchbar button
                    widget->setProperty("placeholder", QVariant(true));
                    widget->setObjectName(QString("QuickLaunchBarButton"));
                }
            }
        }
    }
}

M_REGISTER_VIEW_NEW(QuickLaunchBarView, QuickLaunchBar)
