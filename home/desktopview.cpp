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

#ifdef BENCHMARKS_ON
#include <QTextStream>
#include <QFile>
#include <QTimer>
#include <QTime>
#include <QFileSystemWatcher>
#include <QDir>
#endif

#include "launcher.h"
#include "desktopview.h"
#include "desktop.h"
#include "notificationarea.h"
#include "mainwindow.h"
#include "switcher.h"
#include "statusindicator.h"
#include "contextframeworkcontext.h"
#include "appletspace.h"
#include "quicklaunchbar.h"

#include <DuiViewCreator>
#include <DuiDeviceProfile>
#include <DuiSceneManager>
#include <DuiModalSceneWindow>
#include <DuiPannableViewport>
#include <DuiApplication>
#include <DuiOverlay>
#include <QGraphicsLinearLayout>


#ifdef BENCHMARKS_ON
static QTime lastUpdate;
static int frameCount = 0;
static int fps = 0;
const int MillisecsInSec = 1000;
const int FpsRefreshInterval = 1000;

uint DesktopView::getFps()
{
    return fps;
}

void DesktopView::writeFps(QString str)
{
    Q_UNUSED(str);
    QFile file("/tmp/duihome_benchmarks/benchmarkresult.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    if(getFps() == 0) {
        return;
    }
    QString fpsString = QString::number(getFps());
    QTextStream ts(&file);
    QDateTime now = QDateTime::currentDateTime();
    QString nowString = now.toString("dd.MM.yy.hh:mm:ss");
    ts << fpsString << "  "<< nowString << endl;
    file.close();
}

#endif

DesktopView::DesktopView(Desktop *desktop) :
    DuiWidgetView(desktop),
    switcher(new Switcher),
    quickLaunchBar(new QuickLaunchBar),
    quickLaunchBarWindow(new DuiOverlay),
    launcher(new Launcher),
    launcherWindow(new DuiModalSceneWindow),
    launcherViewport(new DuiPannableViewport(launcherWindow)),
    appletSpace(new AppletSpace),
    appletSpaceWindow(new DuiModalSceneWindow),
    appletSpaceViewport(new DuiPannableViewport(appletSpaceWindow))
{
    // Create the main layout that contains the switcher etc.
    QGraphicsLinearLayout *mainLayout = new QGraphicsLinearLayout(Qt::Vertical);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    desktop->setLayout(mainLayout);

    // Create phone network status indicator
    phoneNetworkIndicator = new PhoneNetworkStatusIndicator(contextFrameworkContext, desktop);

    // Create switcher
    mainLayout->addItem(switcher);
    connect(desktop, SIGNAL(viewportSizePosChanged(const QSizeF &, const QRectF &, const QPointF &)),
            switcher, SLOT(viewportSizePosChanged(const QSizeF &, const QRectF &, const QPointF &)));

    // Fill the rest with empty space
    mainLayout->addStretch();

    // Create a quick launch bar
    quickLaunchBar = new QuickLaunchBar;
    connect(quickLaunchBar, SIGNAL(toggleLauncherButtonClicked()), this, SLOT(toggleLauncher()));
    connect(quickLaunchBar, SIGNAL(toggleAppletSpaceButtonClicked()), this, SLOT(toggleAppletSpace()));

    // Create a layout for the quick launch bar window
    QGraphicsLinearLayout *windowLayout = new QGraphicsLinearLayout();
    windowLayout->setContentsMargins(0, 0, 0, 0);
    windowLayout->addItem(quickLaunchBar);
    quickLaunchBarWindow->setLayout(windowLayout);
    quickLaunchBarWindow->setObjectName("QuickLaunchBarOverlay");
    MainWindow::instance()->sceneManager()->showWindowNow(quickLaunchBarWindow);

    // Put the launcher inside a pannable viewport
    launcherViewport->setWidget(launcher);
    launcherViewport->setMinimumSize(DuiApplication::activeWindow()->visibleSceneSize());
    launcherViewport->setMaximumSize(DuiApplication::activeWindow()->visibleSceneSize());

    // Create a layout for the launcher window
    windowLayout = new QGraphicsLinearLayout();
    windowLayout->setContentsMargins(0, 0, 0, 0);
    windowLayout->addItem(launcherViewport);
    launcherWindow->setLayout(windowLayout);
    launcherWindow->setObjectName("LauncherWindow");
    MainWindow::instance()->sceneManager()->hideWindowNow(launcherWindow);

    // Put the applet space inside a pannable viewport
    connect(appletSpace, SIGNAL(closed()), this, SLOT(toggleAppletSpace()));
    appletSpaceViewport->setWidget(appletSpace);
    appletSpaceViewport->setMinimumSize(DuiApplication::activeWindow()->visibleSceneSize());
    appletSpaceViewport->setMaximumSize(DuiApplication::activeWindow()->visibleSceneSize());

    // Create a layout for the applet space window
    windowLayout = new QGraphicsLinearLayout();
    windowLayout->setContentsMargins(0, 0, 0, 0);
    windowLayout->addItem(appletSpaceViewport);
    appletSpaceWindow->setLayout(windowLayout);
    appletSpaceWindow->setObjectName("AppletSpaceWindow");
    MainWindow::instance()->sceneManager()->hideWindowNow(appletSpaceWindow);

#ifdef BENCHMARKS_ON
    watcher = new QFileSystemWatcher;
    QDir dir;
    if(!dir.exists("/tmp/duihome_benchmarks")) {
        dir.mkdir("/tmp/duihome_benchmarks");
    }
    watcher->addPath("/tmp/duihome_benchmarks");
    connect(watcher,SIGNAL(directoryChanged(QString)),this,SLOT(writeFps(QString)));
#endif
}

DesktopView::~DesktopView()
{
    delete launcherWindow;
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

#ifdef BENCHMARKS_ON
    QTime now = QTime::currentTime();
    ++frameCount;

    if (lastUpdate.msecsTo(now) > FpsRefreshInterval) {
        fps = (MillisecsInSec * frameCount) / (lastUpdate.msecsTo(now));
        frameCount = 0;
        lastUpdate = now;
    }
#endif

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


void DesktopView::toggleLauncher()
{
    if (launcherWindow->isVisible()) {
        hideLauncher();
    } else {
        showLauncher();
    }
}

void DesktopView::showLauncher()
{
    launcher->setEnabled(true);
    launcher->openRootCategory();
    MainWindow::instance()->sceneManager()->showWindow(launcherWindow);

    // Set the launcher window below other modal scene windows
    // @todo TODO get rid of the hardcoded value when DuiSceneManager enables dynamic allocation of Z values
    launcherWindow->parentItem()->setZValue(300);
}

void DesktopView::hideLauncher()
{
    // Disable the launcher so that during the disappear animation of
    // the dialog it's not possible to launch another application
    launcher->setEnabled(false);

    // Scroll the launcher above the screen
    MainWindow::instance()->sceneManager()->hideWindow(launcherWindow);
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

void DesktopView::setGeometry(const QRectF &rect)
{
    DuiWidgetView::setGeometry(rect);

    // Set the launcher viewport to the size of the desktop
    launcherViewport->setMinimumSize(rect.size());
    launcherViewport->setMaximumSize(rect.size());
}

DUI_REGISTER_VIEW_NEW(DesktopView, Desktop)
