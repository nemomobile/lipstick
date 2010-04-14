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

#ifdef BENCHMARKS_ON
#include <QTextStream>
#include <QFile>
#include <QTimer>
#include <QTime>
#include <QFileSystemWatcher>
#include <QDir>
#endif
#include <QDebug>

#include "launcher.h"
#include "desktopview.h"
#include "desktop.h"
#include "mainwindow.h"
#include "switcher.h"
#include "statusindicator.h"
#include "contextframeworkcontext.h"
#include "appletspace.h"
#include "quicklaunchbar.h"
#include "desktopbackground.h"

#include <MViewCreator>
#include <MDeviceProfile>
#include <MSceneManager>
#include <MModalSceneWindow>
#include <MPannableViewport>
#include <MApplication>
#include <MOverlay>
#include <QGraphicsLinearLayout>

#ifdef BENCHMARKS_ON

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
    MWidgetView(desktop),
    switcher(new Switcher),
    quickLaunchBar(new QuickLaunchBar),
    quickLaunchBarWindow(new MOverlay),
    launcher(new Launcher),
    launcherWindow(new MModalSceneWindow),
    appletSpace(new AppletSpace),
    appletSpaceWindow(new MModalSceneWindow),
    appletSpaceViewport(new MPannableViewport(appletSpaceWindow))
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
    connect(quickLaunchBar, SIGNAL(toggleLauncherButtonClicked()), this, SLOT(toggleLauncher()));
    connect(quickLaunchBar, SIGNAL(toggleAppletSpaceButtonClicked()), this, SLOT(toggleAppletSpace()));
    QGraphicsLinearLayout *windowLayout = new QGraphicsLinearLayout();
    windowLayout->setContentsMargins(0, 0, 0, 0);
    windowLayout->addItem(quickLaunchBar);
    quickLaunchBarWindow->setLayout(windowLayout);
    quickLaunchBarWindow->setObjectName("QuickLaunchBarOverlay");
    MainWindow::instance()->sceneManager()->appearSceneWindowNow(quickLaunchBarWindow);

    windowLayout = new QGraphicsLinearLayout();
    windowLayout->setContentsMargins(0, 0, 0, 0);

    // The launcher is added into a modal scene window
    connect(launcher, SIGNAL(launcherButtonClicked()), this, SLOT(toggleLauncher()));
    launcherWindow->setLayout(windowLayout);
    launcherWindow->setObjectName("LauncherWindow");

    windowLayout->addItem(launcher);
    MainWindow::instance()->sceneManager()->disappearSceneWindowNow(launcherWindow);

    // Put the applet space inside a pannable viewport
    connect(appletSpace, SIGNAL(closed()), this, SLOT(toggleAppletSpace()));
    appletSpaceViewport->setWidget(appletSpace);
    appletSpaceViewport->setMinimumSize(MApplication::activeWindow()->visibleSceneSize());
    appletSpaceViewport->setMaximumSize(MApplication::activeWindow()->visibleSceneSize());

    // Create a layout for the applet space scene window
    windowLayout = new QGraphicsLinearLayout();
    windowLayout->setContentsMargins(0, 0, 0, 0);
    windowLayout->addItem(appletSpaceViewport);
    appletSpaceWindow->setLayout(windowLayout);
    appletSpaceWindow->setObjectName("AppletSpaceWindow");
    MainWindow::instance()->sceneManager()->disappearSceneWindowNow(appletSpaceWindow);
    desktopBackground = new DesktopBackground(this);
    connect(desktopBackground, SIGNAL(backgroundImageChanged()),
            this, SLOT(updateBackground()));

#ifdef BENCHMARKS_ON
    connect(MApplication::instance(), SIGNAL(startBenchmarking()), this, SLOT(startBenchmarking()));
    connect(MApplication::instance(), SIGNAL(stopBenchmarking()), this, SLOT(stopBenchmarking()));
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
    MWidgetView::paint(painter, option, widget);

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
    QPixmap pixmap = desktopBackground->backgroundImage(style());
    if (!pixmap.isNull()) {
        painter->drawTiledPixmap(boundingRect(), pixmap, QPointF());
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
    MainWindow::instance()->sceneManager()->appearSceneWindow(launcherWindow);

    // Set the launcher window below other modal scene windows
    // @todo TODO get rid of the hardcoded value when MSceneManager enables dynamic allocation of Z values
    launcherWindow->parentItem()->setZValue(300);

    // Needed to update the panned page width in case the orientation has changed
    launcher->setEnabled(true);
    launcher->resize(geometry().size());

    // TODO : does this have to be animated??
    switcher->setVisible(false);
}

void DesktopView::hideLauncher()
{
    // Disable the launcher so that during the disappear animation of
    // the dialog it's not possible to launch another application
    launcher->setEnabled(false);

    // Scroll the launcher above the screen
    MainWindow::instance()->sceneManager()->disappearSceneWindow(launcherWindow);
    
    // TODO : does this have to be animated??
    switcher->setVisible(true);
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

void DesktopView::updateBackground()
{
    update();
}

void DesktopView::setGeometry(const QRectF &rect)
{
    MWidgetView::setGeometry(rect);
    // Set the viewports to the size of the desktop
    appletSpaceViewport->setMinimumSize(rect.size());
    appletSpaceViewport->setMaximumSize(rect.size());
}

M_REGISTER_VIEW_NEW(DesktopView, Desktop)
