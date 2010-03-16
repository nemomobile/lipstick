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
#include <QTextStream>
#include <QFile>
#include <QTimer>
#include <QTime>
#include <QFileSystemWatcher>
#include <QDir>

// These should really be private variables if one has more than one
// instance of Desktop
static bool benchmarking = false;
static QTime lastUpdate;
static int frameCount = 0;
static int fps = 0;
static QFile* fpsFile;
static QTextStream* fpsStream;

const int MillisecsInSec = 1000;
const int FpsRefreshInterval = 1000;

void DesktopView::writeFps()
{
    if (!benchmarking)
        return;

    QString fpsString = QString::number(fps);
    QDateTime now = QDateTime::currentDateTime();
    QString nowString = now.toString(Qt::ISODate);

    *fpsStream << fpsString << " " << nowString << endl;
    fpsStream->flush();
}

void DesktopView::startBenchmarking()
{
    QDir dir;
    if(!dir.exists("/tmp/duihome_benchmarks")) {
        dir.mkdir("/tmp/duihome_benchmarks");
    }

    fpsFile = new QFile("/tmp/duihome_benchmarks/benchmark_results.txt");
    fpsFile->open(QIODevice::WriteOnly | QIODevice::Append);
    fpsStream = new QTextStream(fpsFile);

    frameCount = 0;
    fps = 0;
    lastUpdate = QTime::currentTime();
    benchmarking = true;

    update();
}

void DesktopView::stopBenchmarking()
{
    benchmarking = false;

    delete fpsStream;
    delete fpsFile;
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

    // Add a placeholder for the status area TODO remove hardcoded values
    QGraphicsWidget *widget = new QGraphicsWidget;
    widget->setMinimumHeight(28);
    widget->setMaximumHeight(28);
    mainLayout->addItem(widget);

    // Create phone network status indicator and put it in a scene window
    phoneNetworkIndicator = new PhoneNetworkStatusIndicator(contextFrameworkContext);
    mainLayout->addItem(phoneNetworkIndicator);

    // Create switcher
    switcher->setObjectName("OverviewSwitcher");
    mainLayout->addItem(switcher);
    connect(desktop, SIGNAL(viewportSizePosChanged(const QSizeF &, const QRectF &, const QPointF &)),
            switcher, SLOT(viewportSizePosChanged(const QSizeF &, const QRectF &, const QPointF &)));

    // Add a placeholder for the quick launch bar TODO remove hardcoded values
    widget = new QGraphicsWidget;
    widget->setMinimumHeight(76);
    widget->setMaximumHeight(76);
    mainLayout->addItem(widget);

    // Create a quick launch bar and put it in a scene window
    quickLaunchBar = new QuickLaunchBar;
    connect(quickLaunchBar, SIGNAL(toggleLauncherButtonClicked()), this, SLOT(toggleLauncher()));
    connect(quickLaunchBar, SIGNAL(toggleAppletSpaceButtonClicked()), this, SLOT(toggleAppletSpace()));
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

    // Create a layout for the launcher scene window
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

    // Create a layout for the applet space scene window
    windowLayout = new QGraphicsLinearLayout();
    windowLayout->setContentsMargins(0, 0, 0, 0);
    windowLayout->addItem(appletSpaceViewport);
    appletSpaceWindow->setLayout(windowLayout);
    appletSpaceWindow->setObjectName("AppletSpaceWindow");
    MainWindow::instance()->sceneManager()->hideWindowNow(appletSpaceWindow);

#ifdef BENCHMARKS_ON
    connect(DuiApplication::instance(), SIGNAL(startBenchmarking()), this, SLOT(startBenchmarking()));
    connect(DuiApplication::instance(), SIGNAL(stopBenchmarking()), this, SLOT(stopBenchmarking()));
#endif
}

DesktopView::~DesktopView()
{
    delete launcherWindow;
    delete quickLaunchBarWindow;
    delete appletSpaceWindow;
}

#ifdef BENCHMARKS_ON
void DesktopView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    DuiWidgetView::paint(painter, option, widget);

    if (benchmarking) {
        QTime now = QTime::currentTime();
        ++frameCount;

        if (lastUpdate.msecsTo(now) > FpsRefreshInterval) {
            fps = (MillisecsInSec * frameCount) / (lastUpdate.msecsTo(now));
            frameCount = 0;
            lastUpdate = now;

            writeFps();
        }

        update();
    }
}
#endif

void DesktopView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *) const
{
    // Draw the background image
    const QPixmap *pixmap = style()->desktopBackgroundImage();
    if (pixmap != NULL) {
        painter->drawTiledPixmap(boundingRect(), *pixmap, QPointF());
    }
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

    // Set the viewports to the size of the desktop
    launcherViewport->setMinimumSize(rect.size());
    launcherViewport->setMaximumSize(rect.size());
    appletSpaceViewport->setMinimumSize(rect.size());
    appletSpaceViewport->setMaximumSize(rect.size());
}

DUI_REGISTER_VIEW_NEW(DesktopView, Desktop)
