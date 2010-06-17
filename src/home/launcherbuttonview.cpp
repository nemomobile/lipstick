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

#include <QApplication>
#include <QGraphicsAnchorLayout>
#include <MProgressIndicator>
#include "launcherbuttonview.h"
#include "launcherbutton.h"

static const int PROGRESS_INDICATOR_TIMEOUT = 10000;

LauncherButtonView::LauncherButtonView(LauncherButton *controller) :
    MButtonIconView(controller),
    progressIndicatorLayout(new QGraphicsAnchorLayout(controller)),
    progressIndicator(new MProgressIndicator(NULL, MProgressIndicator::spinnerType))
{
    controller->setLayout(progressIndicatorLayout);
    progressIndicatorLayout->addCornerAnchors(progressIndicator, Qt::TopLeftCorner, progressIndicatorLayout, Qt::TopLeftCorner);
    progressIndicator->hide();
    progressIndicator->setProperty("launcherButtonText", controller->text());

    progressIndicatorTimer.setSingleShot(true);
    progressIndicatorTimer.setInterval(PROGRESS_INDICATOR_TIMEOUT);

    connect(controller, SIGNAL(clicked()), this, SLOT(showProgressIndicator()));
    connect(&progressIndicatorTimer, SIGNAL(timeout()), this, SLOT(hideProgressIndicator()));
}

LauncherButtonView::~LauncherButtonView()
{
}

void LauncherButtonView::showProgressIndicator()
{
    if (!progressIndicator->isVisible()) {
        connect(qApp, SIGNAL(windowStackingOrderChanged(const QList<WindowInfo> &)), this, SLOT(hideProgressIndicatorIfObscured(const QList<WindowInfo> &)));

        progressIndicator->show();
        progressIndicator->setUnknownDuration(true);
        progressIndicatorTimer.start();
    }
}

void LauncherButtonView::hideProgressIndicator()
{
    progressIndicator->hide();
    progressIndicator->setUnknownDuration(false);
    if (progressIndicatorTimer.isActive()) {
        progressIndicatorTimer.stop();
    }

    disconnect(qApp, SIGNAL(windowStackingOrderChanged(const QList<WindowInfo> &)), this, SLOT(hideProgressIndicatorIfObscured(const QList<WindowInfo> &)));
}

void LauncherButtonView::hideProgressIndicatorIfObscured(const QList<WindowInfo> &windowList)
{
    if (!windowList.isEmpty()) {
        const QList<Atom>& windowTypes = windowList.last().types();
        if (!windowTypes.contains(WindowInfo::NotificationAtom) &&
            !windowTypes.contains(WindowInfo::DesktopAtom) &&
            !windowTypes.contains(WindowInfo::DialogAtom) &&
            !windowTypes.contains(WindowInfo::MenuAtom)) {
            hideProgressIndicator();
        }
    }
}

M_REGISTER_VIEW_NEW(LauncherButtonView, LauncherButton)
