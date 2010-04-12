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

#include "statusindicatorlabelview.h"
#include "statusindicator.h"
#include <QGraphicsLinearLayout>
#include <MTheme>
#include <MLabel>
#include <MViewCreator>

StatusIndicatorLabelView::StatusIndicatorLabelView(StatusIndicator *controller) :
    MWidgetView(controller),
    controller(controller)
{
    label = new MLabel(controller);
    label->setObjectName("StatusIndicatorLabel");
}

StatusIndicatorLabelView::~StatusIndicatorLabelView()
{
}

void StatusIndicatorLabelView::setupModel()
{
    MWidgetView::setupModel();

    label->setText(model()->value().toString());
}

void StatusIndicatorLabelView::updateData(const QList<const char *>& modifications)
{
    MWidgetView::updateData(modifications);

    const char *member;
    foreach(member, modifications) {
        if (member == StatusIndicatorModel::Value) {
            label->setText(model()->value().toString());
        }
    }
}
M_REGISTER_VIEW_NEW(StatusIndicatorLabelView, StatusIndicator)
