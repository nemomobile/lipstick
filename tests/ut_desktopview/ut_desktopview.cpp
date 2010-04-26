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

#include <MApplication>
#include <MApplicationWindow>
#include <QPaintEngine>
#include <QPaintDevice>
#include <MButton>
#include <MSceneManager>
#include <MScene>
#include <MDeviceProfile>
#include <MPannableViewport>
#include <MModalSceneWindow>

#include "ut_desktopview.h"
#include "desktopview.h"
#include "desktop.h"
#include "switcher_stub.h"
#include "quicklaunchbar_stub.h"
#include "launcher_stub.h"
#include "launcherdatastore_stub.h"
#include "windowinfo_stub.h"
#include "homeapplication_stub.h"
#include "mainwindow_stub.h"
#include "applicationcontext.h"
#include <QDBusConnection>
#include "contextframeworkcontext_stub.h"
#include "phonenetworkstatusindicator_stub.h"
#include "statusindicator_stub.h"
#include "appletspace_stub.h"

// MSceneManager stubs
void MSceneManager::appearSceneWindow(MSceneWindow *window, MSceneWindow::DeletionPolicy)
{
    window->show();
}

void MSceneManager::disappearSceneWindow(MSceneWindow *window)
{
    window->hide();
}

// QGraphicsItem stubs
void QGraphicsItem::setZValue(qreal)
{
}

// QDBusConnection stubs
bool QDBusConnection::registerObject(QString const &, QObject *, QFlags<QDBusConnection::RegisterOption>)
{
    return true;
}

// MTheme stubs
void MTheme::releasePixmap(const QPixmap *)
{
}

// Desktop stubs
Desktop::Desktop(MWidget *parent) : MWidgetController(new DesktopModel, parent)
{
}

Desktop::~Desktop()
{
}

TestDesktopBackgroundExtension::TestDesktopBackgroundExtension()
{
}

TestDesktopBackgroundExtension::~TestDesktopBackgroundExtension()
{
}

bool TestDesktopBackgroundExtension::initialize(const QString &)
{
    return true;
}

void TestDesktopBackgroundExtension::setDesktopInterface(MDesktopInterface &)
{
}

void TestDesktopBackgroundExtension::drawBackground(QPainter *, const QRectF &boundingRect) const
{
    this->boundingRect = boundingRect;
}

// Test desktop view implementation
TestDesktopView::TestDesktopView(Desktop *desktop) : DesktopView(desktop)
{
}

// Tests
MainWindow *Ut_DesktopView::mainWindow;

void Ut_DesktopView::initTestCase()
{
    // MApplications must be created manually these days due to theme system changes
    static int argc = 1;
    static char *app_name = (char *)"./ut_desktopview";
    app = new HomeApplication(argc, &app_name);
    mainWindow = MainWindow::instance(true);
    gHomeApplicationStub->stubSetReturnValue("mainWindow", mainWindow);

    backgroundImage = new QPixmap(50, 50);
    backgroundTopImage = new QPixmap(25, 25);
    backgroundBottomImage = new QPixmap(35, 35);
}

void Ut_DesktopView::cleanupTestCase()
{
    delete mainWindow;

    // Destroy the MApplication
    delete app;
}

void Ut_DesktopView::init()
{
    desktop = new Desktop;
    desktopView = new TestDesktopView(desktop);
    desktop->setView(desktopView);
    desktopView->modifiableStyle()->setDesktopBackgroundImage(backgroundImage);
    connect(this, SIGNAL(launcherButtonClicked()), desktopView, SLOT(toggleLauncher()));
}

void Ut_DesktopView::cleanup()
{
    delete desktop;
}

void Ut_DesktopView::testBoundingRectAndDrawBackground()
{
    // Add two extensions to the registered list of extensions
    TestDesktopBackgroundExtension extension1;
    TestDesktopBackgroundExtension extension2;
    desktopView->addExtension(&extension1);
    desktopView->addExtension(&extension2);

    // Check that the drawBackground function of all extensions is called
    QPainter painter;
    QRectF br = desktopView->boundingRect();
    desktopView->drawBackground(&painter, NULL);
    QCOMPARE(extension1.boundingRect, br);
    QCOMPARE(extension2.boundingRect, br);
}

void Ut_DesktopView::testShowingHidingLauncher()
{
    QCOMPARE(desktopView->launcherWindow->isVisible(), false);

    // Show launcher
    emit launcherButtonClicked();
    QCOMPARE(desktopView->launcherWindow->isVisible(), true);
    QCOMPARE(gLauncherStub->stubLastCallTo("setEnabled").parameter<bool>(0), true);

    // Check that the launcher's size was set correctly
    QCOMPARE(desktopView->launcher->geometry().size(), desktopView->geometry().size());

    // Hide launcher
    emit launcherButtonClicked();
    QCOMPARE(desktopView->launcherWindow->isVisible(), false);
    QCOMPARE(gLauncherStub->stubLastCallTo("setEnabled").parameter<bool>(0), false);
}

QTEST_APPLESS_MAIN(Ut_DesktopView)
