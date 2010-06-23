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
#include <QSet>

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
    // remove buttons from layout to avoid multi deletion (buttons are in model as QSharedPointer's)
    foreach (QSharedPointer<LauncherButton> button, model()->launcherButtons()) {
        removeButtonFromLayout(button.data());
    }
}

void LauncherPageView::updateData(const QList<const char *>& modifications)
{
    MWidgetView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == LauncherPageModel::LauncherButtons) {
            updateLayoutFromModel();
        }
    }
}

void LauncherPageView::updateLayoutFromModel()
{
    // Set of buttons in layout
    QSet<LauncherButton *> layoutButtons;
    for (int i = 0; i < layout->count(); i++) {
        layoutButtons.insert(static_cast<LauncherButton *>(layout->itemAt(i)));
    }

    // List of buttons in model (list needed to assure the order of buttons)
    QList<LauncherButton *> modelButtonsList;
    foreach(QSharedPointer<LauncherButton> modelButton, model()->launcherButtons()) {
        modelButtonsList.append(modelButton.data());
    }

    // Set of buttons in model
    QSet<LauncherButton *> modelButtonsSet(modelButtonsList.toSet());

    // Remove from layout the buttons that doesn't exists any more in model
    QSet<LauncherButton *> deletedButtons = layoutButtons - modelButtonsSet;
    foreach (LauncherButton *deletedButton, deletedButtons) {
        removeButtonFromLayout(deletedButton);
    }

    // Get buttons that are new in model to a map (with model list index as a key) for sorting
    QSet<LauncherButton *> newButtons = modelButtonsSet - layoutButtons;
    QMap<int, LauncherButton *> newButtonsSorted;
    foreach (LauncherButton *addedButton, newButtons) {
        newButtonsSorted.insert(modelButtonsList.indexOf(addedButton), addedButton);
    }

    // Add new buttons to layout in sorted order
    QMapIterator<int, LauncherButton *> iter(newButtonsSorted);
    while (iter.hasNext()) {
        iter.next();
        policy->insertItem(iter.key(), iter.value());
    }
}

void LauncherPageView::setupModel()
{
    MWidgetView::setupModel();

    QList<const char *> modifications;
    modifications << LauncherPageModel::LauncherButtons;
    updateData(modifications);
}

void LauncherPageView::removeButtonFromLayout(LauncherButton *button)
{
    layout->removeItem(button);
    // set parent to NULL to avoid double deletion as items are as QSharedPointers in model
    button->setParentItem(0);
}

M_REGISTER_VIEW_NEW(LauncherPageView, LauncherPage)
