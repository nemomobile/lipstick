/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouchhome.
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

#include <QGraphicsLinearLayout>
#include <MLabel>
#include <MButton>
#include <MApplication>
#include "switcherbutton.h"
#include "switcherbuttonwithtitlebarview.h"

SwitcherButtonWithTitleBarView::SwitcherButtonWithTitleBarView(SwitcherButton *button) :
    SwitcherButtonView(button)
{
    titleBarLayout = new QGraphicsLinearLayout(Qt::Horizontal, controller);
    titleBarLayout->setContentsMargins(0,0,0,0);

    titleLabel = new MLabel(controller);
    titleLabel->setContentsMargins(0,0,0,0);
    titleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    titleBarLayout->addItem(titleLabel);

    closeButton = new MButton();
    closeButton->setViewType("extended");
    connect(closeButton, SIGNAL(clicked()), controller, SLOT(close()));
    titleBarLayout->addItem(closeButton);
    titleBarLayout->setAlignment(closeButton, Qt::AlignVCenter);

    controller->setLayout(titleBarLayout);
}

SwitcherButtonWithTitleBarView::~SwitcherButtonWithTitleBarView()
{
}

void SwitcherButtonWithTitleBarView::applyStyle()
{
    SwitcherButtonView::applyStyle();

    if (style()->croppingTop() > 0) {
        titleLabel->show();

        // Apply style to close button
        if (controller->objectName() == "DetailviewButton") {
            closeButton->setObjectName("CloseButtonDetailview");
            if (model()->viewMode() == SwitcherButtonModel::Large) {
                closeButton->show();
            } else {
                closeButton->hide();
            }
            titleLabel->setObjectName("SwitcherButtonTitleLabelDetailview");

        } else {
            if(model()->viewMode() == SwitcherButtonModel::Medium) {
                closeButton->setObjectName("CloseButtonOverviewMedium");
                titleLabel->setObjectName("SwitcherButtonTitleLabelOverviewMedium");
            } else {
                closeButton->setObjectName("CloseButtonOverviewLarge");
                titleLabel->setObjectName("SwitcherButtonTitleLabelOverviewLarge");
            }

            closeButton->show();
        }
        closeButton->setIconID(style()->closeIcon());
    } else {
        titleLabel->hide();
        closeButton->hide();
    }

    update();
}


void SwitcherButtonWithTitleBarView::setupModel()
{
    SwitcherButtonView::setupModel();

    titleLabel->setText(model()->text());
}

void SwitcherButtonWithTitleBarView::updateData(const QList<const char *>& modifications)
{
    SwitcherButtonView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if(member == SwitcherButtonModel::Text) {
            titleLabel->setText(model()->text());
        }
    }
}

QPoint SwitcherButtonWithTitleBarView::thumbnailPosition() const
{
    return QPoint(0, titleLabel->size().height());
}

M_REGISTER_VIEW_NEW(SwitcherButtonWithTitleBarView, SwitcherButton)
