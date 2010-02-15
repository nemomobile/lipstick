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

#include "switcherview.h"
#include "switcher.h"
#include "switcherbutton.h"

#include <DuiViewCreator>
#include <DuiLayout>
#include <DuiTheme>
#include <DuiLocale>
#include <DuiApplication>
#include <DuiWindow>
#include <DuiFlowLayoutPolicy>
#include <DuiSeparator>
#include <QTimeLine>
#include <QGraphicsLinearLayout>

SwitcherView::SwitcherView(Switcher *switcher) :
    DuiWidgetView(switcher),
    controller(switcher),
    mainLayout(new QGraphicsLinearLayout(Qt::Vertical)),
    layout(new DuiLayout),
    policy(new DuiFlowLayoutPolicy(layout))
{
    // Set layouts
    switcher->setLayout(mainLayout);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // The switcher buttons are between separators
    topSeparator = new DuiSeparator;
    topSeparator->setObjectName("SwitcherSeparator");
    bottomSeparator = new DuiSeparator;
    bottomSeparator->setObjectName("SwitcherSeparator");

    // Put the switcher buttons between separators
    mainLayout->addItem(topSeparator);
    layout->setContentsMargins(0, 0, 0, 0);
    policy->setObjectName("SwitcherFlowLayoutPolicy");
    mainLayout->addItem(layout);
    mainLayout->addItem(bottomSeparator);
}

SwitcherView::~SwitcherView()
{
}

void SwitcherView::updateData(const QList<const char *>& modifications)
{
    DuiWidgetView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == SwitcherModel::Buttons) {
            // Remove all widgets from the layout (do not destroy them)
            while (layout->count() > 0) {
                layout->removeAt(0);
            }

            // Add widgets from the model to the layout
            foreach(QSharedPointer<SwitcherButton> button, model()->buttons()) {
                policy->addItem(button.data());
            }
        }
    }
}

DUI_REGISTER_VIEW_NEW(SwitcherView, Switcher)
