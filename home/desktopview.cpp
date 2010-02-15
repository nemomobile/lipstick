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

#include "desktopview.h"
#include "desktop.h"
#include "notificationarea.h"
#include "mainwindow.h"
#include "switcher.h"
#include "statusindicator.h"
#include "contextframeworkcontext.h"
#include "appletspace.h"

#include <DuiViewCreator>
#include <DuiDeviceProfile>
#include <DuiSceneManager>
#include <DuiModalSceneWindow>
#include <DuiPannableViewport>
#include <DuiApplication>
#include <QGraphicsLinearLayout>

DesktopView::DesktopView(Desktop *desktop) :
    DuiWidgetView(desktop),
    switcher(new Switcher),
    appletSpace(new AppletSpace),
    appletSpaceWindow(new DuiModalSceneWindow),
    appletSpaceViewport(new DuiPannableViewport(appletSpaceWindow))
{
    // Create the main layout that contains the switcher etc.
    QGraphicsLinearLayout *mainLayout = new QGraphicsLinearLayout(Qt::Vertical);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    desktop->setLayout(mainLayout);

    // Create phone network indicator menu
    phoneNetworkIndicator = new PhoneNetworkStatusIndicator(contextFrameworkContext, desktop);
    mainLayout->addItem(phoneNetworkIndicator);

    // Create switcher
    mainLayout->addItem(switcher);
    connect(desktop, SIGNAL(viewportSizePosChanged(const QSizeF &, const QRectF &, const QPointF &)),
            switcher, SLOT(viewportSizePosChanged(const QSizeF &, const QRectF &, const QPointF &)));

    // Stretch desktop before the quick launch bar
    mainLayout->addStretch();

    // Create a quick launch bar
    DuiButton *quickLaunchBar = new DuiButton("Applet Space");
    connect(quickLaunchBar, SIGNAL(clicked()), this, SLOT(toggleAppletSpace()));
    mainLayout->addItem(quickLaunchBar);

    // Put the applet space inside a pannable viewport
    connect(appletSpace, SIGNAL(closed()), this, SLOT(toggleAppletSpace()));
    appletSpaceViewport->setWidget(appletSpace);
    appletSpaceViewport->setMinimumSize(DuiApplication::activeWindow()->visibleSceneSize());
    appletSpaceViewport->setMaximumSize(DuiApplication::activeWindow()->visibleSceneSize());

    // Create a layout for the mashup canvas window
    QGraphicsLinearLayout *dialogLayout = new QGraphicsLinearLayout();
    dialogLayout->setContentsMargins(0, 0, 0, 0);
    dialogLayout->addItem(appletSpaceViewport);
    appletSpaceWindow->setLayout(dialogLayout);
    appletSpaceWindow->setObjectName("AppletSpaceWindow");
    MainWindow::instance()->sceneManager()->hideWindowNow(appletSpaceWindow);
}

DesktopView::~DesktopView()
{
}

void DesktopView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *) const
{
    // Draw the background image
    const QPixmap *pixmap = style()->desktopBackgroundImage();
    if (pixmap != NULL) {
        // Always draw the background texture without rotation
        const QTransform w = painter->worldTransform();
        painter->setWorldTransform(QTransform());

        // Use the actual screen size for painting the background because it does not rotate
        QPointF p = w.map(QPointF(0, 0));
        QPointF offset(-p.x() + w.dx(), -p.y() + w.dy());
        painter->drawTiledPixmap(QRectF(0, 0, DuiDeviceProfile::instance()->resolution().width(), DuiDeviceProfile::instance()->resolution().height()), *pixmap, offset);

        // Reset the transform
        painter->setWorldTransform(w);
    }

    if (!model()->notificationAreaOpen()) {
        // Draw the top image
        pixmap = style()->desktopBackgroundTop();
        if (pixmap != NULL) {
            painter->drawTiledPixmap(QRectF(0, -pixmap->height(), geometry().width(), pixmap->height()), *pixmap);
        }
    }

    // Draw the bottom image
    pixmap = style()->desktopBackgroundBottom();
    if (pixmap != NULL) {
        painter->drawTiledPixmap(QRectF(0, geometry().height(), geometry().width(), pixmap->height()), *pixmap);
    }
}

QRectF DesktopView::boundingRect() const
{
    // The area to be drawn includes the top and bottom images in addition to the actual content
    int top = 0;
    int bottom = 0;
    const QPixmap *topPixmap = style()->desktopBackgroundTop();
    const QPixmap *bottomPixmap = style()->desktopBackgroundBottom();

    if (topPixmap != NULL) {
        top = topPixmap->height();
    }
    if (bottomPixmap != NULL) {
        bottom = bottomPixmap->height();
    }

    QRectF rect(0, -top, geometry().width(), geometry().height() + top + bottom);
    return rect.united(QRectF(0, 0, DuiDeviceProfile::instance()->resolution().width(), DuiDeviceProfile::instance()->resolution().height()));
}

void DesktopView::toggleAppletSpace()
{
    if (appletSpaceWindow->isVisible()) {
        appletSpaceWindow->disappear();
        appletSpace->setEnabled(false);
    } else {
        appletSpaceWindow->appear();
        appletSpace->setEnabled(true);
    }
}

DUI_REGISTER_VIEW_NEW(DesktopView, Desktop)
