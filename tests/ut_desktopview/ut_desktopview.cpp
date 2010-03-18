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

#include <DuiApplication>
#include <DuiApplicationWindow>
#include <QPaintEngine>
#include <QPaintDevice>
#include <DuiButton>
#include <DuiSceneManager>
#include <DuiScene>
#include <DuiDeviceProfile>
#include <DuiPannableViewport>
#include <DuiModalSceneWindow>

#include "ut_desktopview.h"
#include "desktopview.h"
#include "desktop.h"
#include "switcher_stub.h"
#include "quicklaunchbar_stub.h"
#include "launcher_stub.h"
#include "windowinfo_stub.h"
#include "homeapplication_stub.h"
#include "mainwindow_stub.h"
#include "applicationcontext.h"
#include <QDBusConnection>
#include "contextframeworkcontext_stub.h"
#include "phonenetworkstatusindicator_stub.h"
#include "statusindicator_stub.h"
#include "appletspace_stub.h"

// Mock Paint Engine
class MockPaintEngine : public QPaintEngine
{
    bool begin(QPaintDevice *pdev) {
        Q_UNUSED(pdev);
        return true;
    }
    bool end() {
        return true;
    }
    void updateState(const QPaintEngineState &state) {
        Q_UNUSED(state);
    }
    void drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr);
    Type type() const {
        return QPaintEngine::User;
    }
};

void MockPaintEngine::drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr)
{
    Q_UNUSED(pm);
    Q_UNUSED(sr);
    Ut_DesktopView::paintArea = Ut_DesktopView::paintArea.united(r);
}

// Mock Paint Device
class MockPaintDevice : public QPaintDevice
{
public:
    MockPaintDevice() : engine(new MockPaintEngine) {}
    ~MockPaintDevice();

    QPaintEngine *paintEngine() const {
        return engine;
    }

    int metric(PaintDeviceMetric metric) const;
private:
    QPaintEngine *engine;
};

MockPaintDevice::~MockPaintDevice()
{
    delete engine;
}

int MockPaintDevice::metric(PaintDeviceMetric metric) const
{
    switch (metric) {
    case QPaintDevice::PdmWidth:
        return 1000;
    case QPaintDevice::PdmHeight:
        return 1000;
    case QPaintDevice::PdmDpiY:
    case QPaintDevice::PdmDpiX:
        return 300;
    default:
        return 0;
    }
    return 0;
}

// DuiSceneManager stubs
void DuiSceneManager::showWindow(DuiSceneWindow *window, DuiSceneWindow::DeletionPolicy)
{
    window->show();
}

void DuiSceneManager::hideWindow(DuiSceneWindow *window)
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

// DuiTheme stubs
void DuiTheme::releasePixmap(const QPixmap *)
{
}

// Desktop stubs
Desktop::Desktop(DuiWidget *parent) : DuiWidgetController(new DesktopModel, parent)
{
}

Desktop::~Desktop()
{
}

// Test desktop view implementation
TestDesktopView::TestDesktopView(Desktop *desktop) : DesktopView(desktop)
{
}

// Tests
QRectF Ut_DesktopView::paintArea;
MainWindow *Ut_DesktopView::mainWindow;

void Ut_DesktopView::initTestCase()
{
    // DuiApplications must be created manually these days due to theme system changes
    static int argc = 1;
    static char *app_name = (char *)"./ut_desktopview";
    app = new HomeApplication(argc, &app_name);
    mainWindow = MainWindow::instance(true);
    gHomeApplicationStub->stubSetReturnValue("mainWindow", mainWindow);

    paintDevice = new MockPaintDevice;
    painter = new QPainter(paintDevice);

    backgroundImage = new QPixmap(50, 50);
    backgroundTopImage = new QPixmap(25, 25);
    backgroundBottomImage = new QPixmap(35, 35);
}

void Ut_DesktopView::cleanupTestCase()
{
    delete painter;
    delete paintDevice;
    delete mainWindow;

    // Destroy the DuiApplication
    delete app;
}

void Ut_DesktopView::init()
{
    desktop = new Desktop;
    desktopView = new TestDesktopView(desktop);
    desktop->setView(desktopView);
    desktopView->modifiableStyle()->setDesktopBackgroundImage(backgroundImage);
    paintArea = QRectF();
    connect(this, SIGNAL(launcherButtonClicked()), desktopView, SLOT(toggleLauncher()));
}

void Ut_DesktopView::cleanup()
{
    delete desktop;
}

void Ut_DesktopView::testBoundingRectAndDrawBackground()
{
    QRectF br = desktopView->boundingRect();

    // Check that the view doesn't draw outside the bounding rectangle
    desktopView->drawBackground(painter, NULL);
    QVERIFY(br == paintArea || br.contains(paintArea) || paintArea.isEmpty());
}

void Ut_DesktopView::testShowingHidingLauncher()
{
    QCOMPARE(desktopView->launcherWindow->isVisible(), false);

    // Show launcher
    emit launcherButtonClicked();
    QCOMPARE(desktopView->launcherWindow->isVisible(), true);
    QCOMPARE(gLauncherStub->stubLastCallTo("setEnabled").parameter<bool>(0), true);

    // Hide launcher
    emit launcherButtonClicked();
    QCOMPARE(desktopView->launcherWindow->isVisible(), false);
    QCOMPARE(gLauncherStub->stubLastCallTo("setEnabled").parameter<bool>(0), false);
}

QTEST_APPLESS_MAIN(Ut_DesktopView)
