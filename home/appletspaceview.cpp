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

#include <QGraphicsLinearLayout>
#include <MModalSceneWindow>
#include <MPannableViewport>
#include <MMashupCanvas>
#include <MOverlay>
#include <MButton>
#include "appletspaceview.h"
#include "appletspace.h"
#include <QDebug>

AppletSpaceView::AppletSpaceView(AppletSpace *controller) :
    MExtendingBackgroundView(controller),
    controller(controller),
    closeButtonOverlay(new MOverlay),
    closeButton(new MButton(closeButtonOverlay))
{
    // Put the mashup canvas into the bottom of the layout
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->setContentsMargins(0, 0, 0, 0);
    controller->setLayout(layout);

    mashupCanvas = new MMashupCanvas("appletcanvas");
    layout->addStretch();
    layout->addItem(mashupCanvas);

    // Set close button properties
    closeButton->setViewType("icon");
    closeButton->setObjectName("AppletSpaceCloseButton");
    closeButton->setIconID("icon-m-framework-close");
    QObject::connect(closeButton, SIGNAL(clicked()), controller, SIGNAL(closed()));

    closeButtonOverlay->setObjectName("AppletSpaceCloseButtonOverlay");
}

AppletSpaceView::~AppletSpaceView()
{
    delete closeButtonOverlay;
}

void AppletSpaceView::updateData(const QList<const char *>& modifications)
{
    MExtendingBackgroundView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == AppletSpaceModel::CloseButtonVisible) {
            // Set the visibility of the button
            if (model()->closeButtonVisible()) {
                closeButtonOverlay->appear();
            } else {
                closeButtonOverlay->disappear();
            }
        }
    }
}

M_REGISTER_VIEW_NEW(AppletSpaceView, AppletSpace)
