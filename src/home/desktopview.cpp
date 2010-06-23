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
#endif

#include <QDir>

#include "launcher.h"
#include "launcherdatastore.h"
#include "desktopview.h"
#include "desktop.h"
#include "mainwindow.h"
#include "switcher.h"
#include "appletspace.h"
#include "quicklaunchbar.h"
#include "mdesktopbackgroundextensioninterface.h"
#include "mfiledatastore.h"

#include <MViewCreator>
#include <MDeviceProfile>
#include <MSceneManager>
#include <MModalSceneWindow>
#include <MPannableViewport>
#include <MApplication>
#include <MOverlay>
#include <MApplicationExtensionArea>
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
    launcherDataStore(NULL),
    launcher(NULL),
    launcherWindow(new MModalSceneWindow),
    quickLaunchBar(NULL),
    quickLaunchBarWindow(new MOverlay),
    appletSpace(new AppletSpace),
    appletSpaceWindow(new MModalSceneWindow),
    appletSpaceViewport(new MPannableViewport(appletSpaceWindow)),
    backgroundExtensionArea(new MApplicationExtensionArea("com.meego.core.MDesktopBackgroundExtensionInterface/1.0"))
{
    // Create the launcher data store
    launcherDataStore = createLauncherDataStore();

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
    quickLaunchBar = new QuickLaunchBar(launcherDataStore);
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
    launcher = new Launcher(launcherDataStore);

    connect(qApp, SIGNAL(focusToLauncherAppRequested(const QString &)), this, SLOT(showLauncherAndPanToPage(const QString &)));
    connect(qApp, SIGNAL(windowStackingOrderChanged(const QList<WindowInfo> &)), this, SLOT(updateLauncherVisiblity(const QList<WindowInfo> &)));
    launcherWindow->setLayout(windowLayout);
    launcherWindow->setObjectName("LauncherWindow");
    windowLayout->addItem(launcher);

    // Register the launcher window into the scene manager to make sure launcher page styling works in both orientations
    MainWindow::instance()->sceneManager()->appearSceneWindowNow(launcherWindow);
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

#ifdef BENCHMARKS_ON
    connect(MApplication::instance(), SIGNAL(startBenchmarking()), this, SLOT(startBenchmarking()));
    connect(MApplication::instance(), SIGNAL(stopBenchmarking()), this, SLOT(stopBenchmarking()));
#endif

    // Connect the desktop background extension signals
    connect(backgroundExtensionArea, SIGNAL(extensionInstantiated(MApplicationExtensionInterface*)),
            this, SLOT(addExtension(MApplicationExtensionInterface*)));
    connect(backgroundExtensionArea, SIGNAL(extensionRemoved(MApplicationExtensionInterface*)),
            this, SLOT(removeExtension(MApplicationExtensionInterface*)));
    backgroundExtensionArea->init();
}

DesktopView::~DesktopView()
{
    delete launcherWindow;
    delete quickLaunchBarWindow;
    delete appletSpaceWindow;
    delete backgroundExtensionArea;
    delete launcherDataStore;
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
    foreach (MDesktopBackgroundExtensionInterface *backgroundExtension, backgroundExtensions) {
        backgroundExtension->drawBackground(painter, boundingRect());
    }
}

void DesktopView::showLauncherAndPanToPage(const QString &desktopFileEntry)
{
    if(launcher->panToPage(desktopFileEntry) >= 0 || desktopFileEntry.isEmpty()) {
        showLauncher();
        MainWindow::instance()->activateWindow();
        MainWindow::instance()->raise();
    }
}

void DesktopView::updateLauncherVisiblity(const QList<WindowInfo> &windowList)
{
    if (launcher->isEnabled() && !windowList.isEmpty()) {
        const QList<Atom>& windowTypes = windowList.last().types();
        if (!windowTypes.contains(WindowInfo::NotificationAtom) &&
            !windowTypes.contains(WindowInfo::DesktopAtom) &&
            !windowTypes.contains(WindowInfo::DialogAtom) &&
            !windowTypes.contains(WindowInfo::MenuAtom)) {
            hideLauncher();
        }
    }
}

void DesktopView::toggleLauncher()
{
    if (launcherWindow->isVisible()) {
        hideLauncher();
    } else {
        launcher->setFirstPage();
        showLauncher();
    }
}

void DesktopView::showLauncher()
{
    MainWindow::instance()->sceneManager()->appearSceneWindow(launcherWindow);

    // Set the launcher window below other modal scene windows
    // @todo TODO get rid of the hardcoded value when MSceneManager enables dynamic allocation of Z values
    launcherWindow->parentItem()->setZValue(300);

    launcher->setEnabled(true);

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

void DesktopView::setGeometry(const QRectF &rect)
{
    MWidgetView::setGeometry(rect);
    // Set the viewports to the size of the desktop
    appletSpaceViewport->setMinimumSize(rect.size());
    appletSpaceViewport->setMaximumSize(rect.size());
}

void DesktopView::update()
{
    MWidgetView::update();
}

M::OrientationAngle DesktopView::orientationAngle()
{
    return MainWindow::instance()->sceneManager()->orientationAngle();
}

void DesktopView::addExtension(MApplicationExtensionInterface *extension)
{
    MDesktopBackgroundExtensionInterface *backgroundExtension = static_cast<MDesktopBackgroundExtensionInterface *>(extension);
    backgroundExtension->setDesktopInterface(*this);
    backgroundExtensions.append(backgroundExtension);
}

void DesktopView::removeExtension(MApplicationExtensionInterface *extension)
{
    MDesktopBackgroundExtensionInterface *backgroundExtension = static_cast<MDesktopBackgroundExtensionInterface *>(extension);
    backgroundExtensions.removeOne(backgroundExtension);
}

LauncherDataStore *DesktopView::createLauncherDataStore()
{
    if (!QDir::root().exists(QDir::homePath() + "/.config/duihome")) {
        QDir::root().mkpath(QDir::homePath() + "/.config/duihome");
    }

    QString dataStoreFileName = QDir::homePath() + "/.config/duihome/launcherbuttons.data";

    if (!QFile::exists(dataStoreFileName)) {
        QString defaultDataStoreFileName = M_XDG_DIR "/duihome/launcherbuttons.data";
        // Copy the default datastore only if it exists
        if (QFile::exists(defaultDataStoreFileName)) {
            QFile::copy(defaultDataStoreFileName, dataStoreFileName);
        }
    }

    return new LauncherDataStore(new MFileDataStore(dataStoreFileName));
}

M_REGISTER_VIEW_NEW(DesktopView, Desktop)
