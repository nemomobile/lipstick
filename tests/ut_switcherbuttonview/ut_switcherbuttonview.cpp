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
#include "ut_switcherbuttonview.h"

#include <MScalableImage>
#include <MCancelEvent>
#include "mscenemanager_stub.h"
#include <MScene>
#include <MLabel>
#include "mainwindow_stub.h"
#include "homeapplication_stub.h"
#include "switcher_stub.h"
#include "windowinfo_stub.h"

bool Ut_SwitcherButtonView::timerStarted = false;
MainWindow *Ut_SwitcherButtonView::mainWindow = NULL;

#define TEST_ANY_OTHER_ATOM 1
#define TEST_NET_WM_ICON_GEOMETRY_ATOM 303

// QCoreApplication stubs to avoid crashing in processEvents()
QStringList QCoreApplication::arguments()
{
    return QStringList();
}

// X11Wrapper stubs (used by SwitcherButtonView)
Atom X11Wrapper::XInternAtom(Display *, const char *atom_name, Bool)
{
    if (strcmp(atom_name, "_NET_WM_ICON_GEOMETRY") == 0) {
        return TEST_NET_WM_ICON_GEOMETRY_ATOM;
    } else {
        return TEST_ANY_OTHER_ATOM;
    }
}

int X11Wrapper::XChangeProperty(Display *display, Window w, Atom property, Atom type, int format, int mode, unsigned char *data, int nelements)
{
    Ut_SwitcherButtonView::xChangePropertyDisplay = display;
    Ut_SwitcherButtonView::xChangePropertyWindow = w;
    Ut_SwitcherButtonView::xChangePropertyProperty = property;
    Ut_SwitcherButtonView::xChangePropertyType = type;
    Ut_SwitcherButtonView::xChangePropertyFormat = format;
    Ut_SwitcherButtonView::xChangePropertyMode = mode;
    Ut_SwitcherButtonView::xChangePropertyNElements = nelements;

    if (data != NULL) {
        memcpy(Ut_SwitcherButtonView::xChangePropertyData, data, nelements * format / 8);
    }
    return 0;
}

// MTheme stubs
void MTheme::releasePixmap(const QPixmap *)
{
}

// X stubs (used by SwitcherButton)
QList<Pixmap> Ut_SwitcherButtonView::allocatedPixmaps;
Pixmap Ut_SwitcherButtonView::lastPixmap;
bool Ut_SwitcherButtonView::xCompositeNameWindowPixmapCausesBadMatch = false;
XErrorHandler Ut_SwitcherButtonView::xErrorHandler = NULL;
Display *Ut_SwitcherButtonView::xChangePropertyDisplay;
Window Ut_SwitcherButtonView::xChangePropertyWindow;
Atom Ut_SwitcherButtonView::xChangePropertyProperty;
Atom Ut_SwitcherButtonView::xChangePropertyType;
int Ut_SwitcherButtonView::xChangePropertyFormat;
int Ut_SwitcherButtonView::xChangePropertyMode;
unsigned char Ut_SwitcherButtonView::xChangePropertyData[16];
int Ut_SwitcherButtonView::xChangePropertyNElements;
bool Ut_SwitcherButtonView::damageCreated = false;
bool Ut_SwitcherButtonView::damageDestroyed = false;
unsigned long Ut_SwitcherButtonView::damageHandle = 0;
Display *Ut_SwitcherButtonView::damageDisplay = NULL;
bool Ut_SwitcherButtonView::damageSubtracted = false;
unsigned long Ut_SwitcherButtonView::damageSubtractHandle = 0;
Display *Ut_SwitcherButtonView::damageSubtractDisplay = NULL;

XErrorHandler X11Wrapper::XSetErrorHandler(XErrorHandler handler)
{
    XErrorHandler oldHandler = Ut_SwitcherButtonView::xErrorHandler;
    Ut_SwitcherButtonView::xErrorHandler = handler;
    return oldHandler;
}

Pixmap X11Wrapper::XCompositeNameWindowPixmap(Display *display, Window)
{
    if (Ut_SwitcherButtonView::xCompositeNameWindowPixmapCausesBadMatch && Ut_SwitcherButtonView::xErrorHandler != NULL) {
        XErrorEvent error;
        error.error_code = BadMatch;
        Ut_SwitcherButtonView::xErrorHandler(display, &error);
        return 123456;
    } else {
        Ut_SwitcherButtonView::allocatedPixmaps.append(++Ut_SwitcherButtonView::lastPixmap);
        return Ut_SwitcherButtonView::lastPixmap;
    }
}

int X11Wrapper::XFreePixmap(Display *, Pixmap pixmap)
{
    return Ut_SwitcherButtonView::allocatedPixmaps.removeOne(pixmap) ? 0 : BadPixmap;
}

Damage X11Wrapper::XDamageCreate(Display *dpy, Drawable drawable, int)
{
    Ut_SwitcherButtonView::damageCreated = true;
    Ut_SwitcherButtonView::damageHandle = drawable;
    Ut_SwitcherButtonView::damageDisplay = dpy;
    return Ut_SwitcherButtonView::damageHandle;
}

void X11Wrapper::XDamageDestroy(Display *, Damage damage)
{
    if (damage == Ut_SwitcherButtonView::damageHandle) {
        Ut_SwitcherButtonView::damageDestroyed = true;
        Ut_SwitcherButtonView::damageHandle = 0;
    }
}

int X11Wrapper::XSync(Display *, Bool)
{
    return 0;
}

void X11Wrapper::XDamageSubtract(Display *dpy, Damage damage, XserverRegion, XserverRegion)
{
    Ut_SwitcherButtonView::damageSubtracted = true;
    Ut_SwitcherButtonView::damageSubtractHandle = damage;
    Ut_SwitcherButtonView::damageSubtractDisplay = dpy;
}

// MScalableImage stubs
const MScalableImage *Ut_SwitcherButtonView::drawnScalableImage;

void MScalableImage::draw(const QRect &rect, QPainter *painter) const
{
    Q_UNUSED(rect);
    Q_UNUSED(painter);

    Ut_SwitcherButtonView::drawnScalableImage = this;
}

// QPainter stubs (used by SwitcherButtonView)
QRectF Ut_SwitcherButtonView::drawPixmapRect;
QRectF Ut_SwitcherButtonView::drawPixmapSourceRect;
bool gQPainter_drawPixmap_throwsBadAlloc = false;
void QPainter::drawPixmap(const QRectF &targetRect, const QPixmap &, const QRectF &sourceRect)
{
    if (gQPainter_drawPixmap_throwsBadAlloc) {
        QT_THROW(std::bad_alloc());
    }
    Ut_SwitcherButtonView::drawPixmapRect = targetRect;
    Ut_SwitcherButtonView::drawPixmapSourceRect = sourceRect;
}

bool gQPainter_restore_called = false;
void QPainter::restore()
{
    gQPainter_restore_called = true;
}

int Ut_SwitcherButtonView::returnedPixmapWidth = 180;
int QPixmap::width() const
{
    return Ut_SwitcherButtonView::returnedPixmapWidth;
}

int Ut_SwitcherButtonView::returnedPixmapHeight = 120;
int QPixmap::height() const
{
    return Ut_SwitcherButtonView::returnedPixmapHeight;
}

bool Ut_SwitcherButtonView::viewUpdateCalled;
void MWidgetView::update(const QRectF &)
{
    Ut_SwitcherButtonView::viewUpdateCalled = true;
}

// Test switcher button implementation
TestSwitcherButton::TestSwitcherButton(QGraphicsItem *parent) :
    SwitcherButton(parent)
{
    view = new TestSwitcherButtonView(*this);
    setView(view);
}

TestSwitcherButtonView *TestSwitcherButton::getView()
{
    return dynamic_cast<TestSwitcherButtonView *>(view);
}

void TestSwitcherButton::emitDisplayEntered()
{
    emit displayEntered();
}

void TestSwitcherButton::emitDisplayExited()
{
    emit displayExited();
}

// Test switcher button view implementation
TestSwitcherButtonView::TestSwitcherButtonView(SwitcherButton &button) :
    SwitcherButtonView(&button)
{
}

// Test home application
class TestHomeApplication : public HomeApplication
{
public:
    TestHomeApplication(int &argc, char **argv);

    void emitDamageEvent(Qt::HANDLE damage, short x, short y, unsigned short width, unsigned short height);
};

TestHomeApplication::TestHomeApplication(int &argc, char **argv) : HomeApplication(argc, argv)
{
}

void TestHomeApplication::emitDamageEvent(Qt::HANDLE damage, short x, short y, unsigned short width, unsigned short height)
{
    emit damageEvent(damage, x, y, width, height);
}

// QTimer stubs (used by Ut_SwitcherButtonView)
void QTimer::start(int)
{
    start();
}

void QTimer::start()
{
    id = 0;
    Ut_SwitcherButtonView::timerStarted = true;
}

void QTimer::stop()
{
    id = -1;
    Ut_SwitcherButtonView::timerStarted = false;
}

Qt::HANDLE gQPixmapX11Handle;

// QPixmap stubs (used by SwitcherButtonView)
QPixmap QPixmap::fromX11Pixmap(Qt::HANDLE handle, ShareMode)
{
    gQPixmapX11Handle = handle;
    return QPixmap();
}

void Ut_SwitcherButtonView::init()
{
    button = new TestSwitcherButton;
    button->setText("Test");
    button->setPos(10, 10);
    m_subject = button->getView();

    timerStarted = false;
    allocatedPixmaps.clear();
    lastPixmap = 0;
    xCompositeNameWindowPixmapCausesBadMatch = false;
    xErrorHandler = NULL;
    damageCreated = false;
    viewUpdateCalled = false;
    damageDestroyed = false;
    damageDisplay = NULL;
    damageSubtracted = false;
    damageSubtractHandle = 0;
    damageSubtractDisplay = NULL;
    gQPainter_drawPixmap_throwsBadAlloc = false;
    gQPainter_restore_called = false;
    gQPixmapX11Handle = 0;
}

void Ut_SwitcherButtonView::cleanup()
{
    // Destroy the controller which will also destroy the view
    delete button;

    // Check that whatever Pixmap is allocated is also freed
    QCOMPARE(allocatedPixmaps.count(), 0);

    gHomeApplicationStub->stubReset();
}

void Ut_SwitcherButtonView::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_switcherbuttonview";
    app = new TestHomeApplication(argc, &app_name);
    mainWindow = MainWindow::instance(true);
    gHomeApplicationStub->stubSetReturnValue("mainWindow", mainWindow);
}

void Ut_SwitcherButtonView::cleanupTestCase()
{
    delete mainWindow;
    delete app;
}

void Ut_SwitcherButtonView::testXWindow()
{
    // Setting an X window ID when the switcher button is not displayed should allocate a new Pixmap but not create Damage
    button->emitDisplayExited();
    button->model()->setXWindow(1);
    QCOMPARE(allocatedPixmaps.count(), 1);

    Pixmap oldPixmap = allocatedPixmaps.at(0);
    QCOMPARE(oldPixmap, (Pixmap)1);
    QCOMPARE(damageCreated, false);

    // Setting another X window ID should free the previously allocated Pixmap and allocate a new one and create Damage
    button->emitDisplayEntered();
    button->model()->setXWindow(2);
    QCOMPARE(allocatedPixmaps.count(), 1);
    QVERIFY(allocatedPixmaps.at(0) != oldPixmap);
    QCOMPARE(damageCreated, true);
}

void Ut_SwitcherButtonView::testViewModeChange()
{
    QCOMPARE(button->model()->viewMode(), SwitcherButtonModel::Large);
    for (int i = 0; i < 2; ++i) { // test setting a couple of times
        button->model()->setViewMode(SwitcherButtonModel::Small);
        QCOMPARE(m_subject->styleContainer().currentMode(), QString("small"));
        button->model()->setViewMode(SwitcherButtonModel::Medium);
        QCOMPARE(m_subject->styleContainer().currentMode(), QString("medium"));
        button->model()->setViewMode(SwitcherButtonModel::Large);
        QCOMPARE(m_subject->styleContainer().currentMode(), QString("large"));
    }
}

void Ut_SwitcherButtonView::testDamageEventForKnownDamage()
{
    // Create a known damage handle
    button->model()->setXWindow(1);
    button->emitDisplayEntered();

    // Known damage events while being displayed should cause a view update
    viewUpdateCalled = false;
    app->emitDamageEvent(damageHandle, 0, 0, 0, 0);
    QVERIFY(viewUpdateCalled);
}

void Ut_SwitcherButtonView::testDamageEventForUnknownDamage()
{
    // Create a known damage handle
    button->model()->setXWindow(1);

    // Unknown damage events should not cause a view update
    viewUpdateCalled = false;
    app->emitDamageEvent(damageHandle + 1, 0, 0, 0, 0);
    QVERIFY(!viewUpdateCalled);
}

void Ut_SwitcherButtonView::testEnterExitDisplay()
{
    // The damage is not created unless there is a window
    button->model()->setXWindow(1);

    // Entering the display should cause a view update and damage creation
    viewUpdateCalled = false;
    button->emitDisplayEntered();
    QVERIFY(viewUpdateCalled);

    // Check that the damage is tracked with the underlying window
    QVERIFY(damageCreated);
    QCOMPARE(damageHandle, button->model()->xWindow());

    // Exiting the display should cause the destruction of the damage
    button->emitDisplayExited();
    QVERIFY(damageDestroyed);
    QCOMPARE(damageHandle, (unsigned long)0);

    QVERIFY(!disconnect(qApp, SIGNAL(damageEvent(Qt::HANDLE &, short &, short &, unsigned short &, unsigned short &)), m_subject, SLOT(damageEvent(Qt::HANDLE &, short &, short &, unsigned short &, unsigned short &))));
}

void Ut_SwitcherButtonView::testXDamageSubtractWhenDisplayEntered()
{
    // The damage is not created unless there is a window
    button->model()->setXWindow(1);

    button->emitDisplayEntered();

    app->emitDamageEvent(damageHandle, 0, 0, 0, 0);

    QVERIFY(damageSubtracted);
    QCOMPARE(damageDisplay, damageSubtractDisplay);
    QCOMPARE(damageHandle, damageSubtractHandle);

    QVERIFY(disconnect(qApp, SIGNAL(damageEvent(Qt::HANDLE &, short &, short &, unsigned short &, unsigned short &)), m_subject, SLOT(damageEvent(Qt::HANDLE &, short &, short &, unsigned short &, unsigned short &))));
}

void Ut_SwitcherButtonView::testDamageCreationUponConstruction()
{
    // The damage is not created unless there is a window
    button->model()->setXWindow(1);
    QVERIFY(!damageCreated);
    QCOMPARE(damageHandle, (unsigned long)0);
}

void Ut_SwitcherButtonView::testSignalConnections()
{
    QVERIFY(disconnect(button, SIGNAL(displayEntered()), m_subject, SLOT(setOnDisplay())));
    QVERIFY(disconnect(button, SIGNAL(displayExited()), m_subject, SLOT(unsetOnDisplay())));
    QVERIFY(disconnect(&m_subject->updateXWindowIconGeometryTimer,
                       SIGNAL(timeout()),
                       m_subject,
                       SLOT(updateXWindowIconGeometry())));
    QVERIFY(disconnect(&m_subject->updateXWindowPixmapRetryTimer,
                       SIGNAL(timeout()),
                       m_subject,
                       SLOT(updateXWindowPixmap())));
}

const qreal thumbnailStyleWidth = 180.0;
const qreal thumbnailStyleHeight = 120.0;
void Ut_SwitcherButtonView::testDrawBackground_data()
{
    QTest::addColumn<M::Orientation>("orientation");
    QTest::addColumn<M::OrientationAngle>("orientationAngle");
    QTest::addColumn<QRectF>("targetRect");
    QTest::addColumn<QRectF>("sourceRect");

    QTest::newRow("landscape0") << M::Landscape << M::Angle0
            << QRectF(0, 0, thumbnailStyleWidth, thumbnailStyleHeight)
            << QRectF(0, 0, Ut_SwitcherButtonView::returnedPixmapWidth, Ut_SwitcherButtonView::returnedPixmapHeight);

    QTest::newRow("landscape90") << M::Landscape << M::Angle90
            << QRectF(-thumbnailStyleWidth, 0, thumbnailStyleWidth, thumbnailStyleHeight)
            << QRectF(0, 0, Ut_SwitcherButtonView::returnedPixmapWidth, Ut_SwitcherButtonView::returnedPixmapHeight);

    // FIXME: add tests for portrait and other angles
}

void Ut_SwitcherButtonView::testDrawBackground()
{
    QPainter painter;
    QFETCH(M::Orientation, orientation);
    QFETCH(M::OrientationAngle, orientationAngle);
    QFETCH(QRectF, targetRect);
    QFETCH(QRectF, sourceRect);

    m_subject->modifiableStyle()->setIconSize(QSize(thumbnailStyleWidth, thumbnailStyleHeight));

    gMSceneManagerStub->stubSetReturnValue("orientation", orientation);
    gMSceneManagerStub->stubSetReturnValue("orientationAngle", orientationAngle);
    m_subject->drawBackground(&painter, NULL);

    QCOMPARE(drawPixmapRect, targetRect);
    QCOMPARE(drawPixmapSourceRect, sourceRect);

    // When the background is drawn the icon geometry should be updated if necessary
    QVERIFY(m_subject->updateXWindowIconGeometryTimer.isActive());
}

void Ut_SwitcherButtonView::testWhenDrawingPixmapThrowsBadAllocThenDrawBackgroundSkipsDrawing()
{
    QPainter painter;
    gQPainter_drawPixmap_throwsBadAlloc = true;
    m_subject->drawBackground(&painter, NULL);
    QCOMPARE(gQPainter_restore_called, true);
}

void Ut_SwitcherButtonView::testUpdateXWindowIconGeometryIfNecessary()
{
    // When no timer is running and the geometry differs from what has already been set the timer should be started
    m_subject->updateXWindowIconGeometryIfNecessary();
    QVERIFY(timerStarted);

    // Timer is restarted if another call comes and the geometry wasn't updated yet
    timerStarted = false;
    m_subject->updateXWindowIconGeometryIfNecessary();
    QVERIFY(timerStarted);

    // When the geometry is the same as what has already been set the timer should not be started
    m_subject->updateXWindowIconGeometry();
    m_subject->updateXWindowIconGeometryTimer.stop();
    m_subject->updateXWindowIconGeometryIfNecessary();
    QVERIFY(!m_subject->updateXWindowIconGeometryTimer.isActive());

    // Test signal connections
    QVERIFY(disconnect(&m_subject->updateXWindowIconGeometryTimer, SIGNAL(timeout()), m_subject, SLOT(updateXWindowIconGeometry())));
}

void Ut_SwitcherButtonView::testUpdateXWindowIconGeometry()
{
    m_subject->modifiableStyle()->setIconSize(QSize(thumbnailStyleWidth, thumbnailStyleHeight));
    m_subject->updateXWindowIconGeometry();

    // XChangeProperty should be called for the window of the button and _NET_WM_ICON_GEOMETRY property should be filled with 4 32-bit values which should contain the icon geometry
    QRectF iconSceneGeometry(QPointF(button->x(), button->y()), QSizeF(thumbnailStyleWidth, thumbnailStyleHeight));
    QCOMPARE(xChangePropertyWindow, button->xWindow());
    QCOMPARE(xChangePropertyProperty, (Atom)TEST_NET_WM_ICON_GEOMETRY_ATOM);
    QCOMPARE(xChangePropertyFormat, 32);
    QCOMPARE(xChangePropertyNElements, 4);
    unsigned int *iconGeometry = (unsigned int *)xChangePropertyData;
    QCOMPARE(iconGeometry[0], (unsigned int)iconSceneGeometry.x());
    QCOMPARE(iconGeometry[1], (unsigned int)iconSceneGeometry.y());
    QCOMPARE(iconGeometry[2], (unsigned int)iconSceneGeometry.width());
    QCOMPARE(iconGeometry[3], (unsigned int)iconSceneGeometry.height());
}

static const int ICON_PIXMAP_RETRY_MAX_COUNT = 2;
void Ut_SwitcherButtonView::testUpdateXWindowPixmap()
{
    // damage handling is tested elsewhere, this test
    // tests the error handling and retrying logic

    // first update the pixmap successfully
    xCompositeNameWindowPixmapCausesBadMatch = false;
    button->model()->setXWindow(1);
    Pixmap oldPixmap = m_subject->xWindowPixmap;
    QCOMPARE(allocatedPixmaps.count(), 1);
    QCOMPARE(gQPixmapX11Handle, oldPixmap);

    // test that the timer is started each time the pixmap
    // fetch causes a badmatch, up to the max count and that
    // nothing else is done
    xCompositeNameWindowPixmapCausesBadMatch = true;
    for (int i = 0; i < ICON_PIXMAP_RETRY_MAX_COUNT; ++i) {
        timerStarted = false;
        m_subject->updateXWindowPixmap();
        QVERIFY(timerStarted);
        QCOMPARE(m_subject->updateXWindowPixmapRetryCount, i + 1);
        QCOMPARE(m_subject->xWindowPixmap, oldPixmap);
        QCOMPARE(allocatedPixmaps.count(), 1);
        QCOMPARE(gQPixmapX11Handle, oldPixmap);
    }
    // the next attempt after max count doesn't even start the
    // timer anymore
    timerStarted = false;
    m_subject->updateXWindowPixmap();
    QVERIFY(!timerStarted);
    QCOMPARE(m_subject->xWindowPixmap, oldPixmap);
    QCOMPARE(allocatedPixmaps.count(), 1);
    QCOMPARE(gQPixmapX11Handle, oldPixmap);

    // when badmatch doesn't occur the timer is not started,
    // but the pixmap does get updated
    xCompositeNameWindowPixmapCausesBadMatch = false;
    timerStarted = false;
    m_subject->updateXWindowPixmap();
    QVERIFY(!timerStarted);
    QCOMPARE(m_subject->updateXWindowPixmapRetryCount, 0);
    QVERIFY(m_subject->xWindowPixmap != oldPixmap);
    QCOMPARE(gQPixmapX11Handle, m_subject->xWindowPixmap);
    QCOMPARE(allocatedPixmaps.count(), 1);
}

const Window CORRECT_WINDOW_ID = 1001;
const Window INCORRECT_WINDOW_ID = 2002;
XEvent Ut_SwitcherButtonView::setupVisibilityNotifyTest()
{
    button->setXWindow(CORRECT_WINDOW_ID);
    viewUpdateCalled = false;

    XEvent xevent;
    xevent.type = VisibilityNotify;
    xevent.xvisibility.state = VisibilityFullyObscured;
    xevent.xvisibility.window = CORRECT_WINDOW_ID;
    xevent.xvisibility.send_event = True;
    return xevent;
}

void Ut_SwitcherButtonView::testSwitcherButtonsVisibilityEventCausesUpdateToBeCalled()
{
    XEvent xevent = setupVisibilityNotifyTest();

    QCOMPARE(m_subject->xEventListener->handleXEvent(xevent), true);
    QCOMPARE(viewUpdateCalled, true);
}

void Ut_SwitcherButtonView::testSwitcherButtonsVisibilityEventWithIncorrectTypeDoesNotCauseUpdateToBeCalled()
{
    XEvent xevent = setupVisibilityNotifyTest();
    xevent.type = PropertyNotify;

    QCOMPARE(m_subject->xEventListener->handleXEvent(xevent), false);
    QCOMPARE(viewUpdateCalled, false);
}

void Ut_SwitcherButtonView::testSwitcherButtonsVisibilityEventWithIncorrectVisibilityStateDoesNotCauseUpdateToBeCalled()
{
    XEvent xevent = setupVisibilityNotifyTest();
    xevent.xvisibility.state = VisibilityPartiallyObscured;

    QCOMPARE(m_subject->xEventListener->handleXEvent(xevent), false);
    QCOMPARE(viewUpdateCalled, false);
}

void Ut_SwitcherButtonView::testSwitcherButtonsVisibilityEventWithIncorrectVisibilityWindowDoesNotCauseUpdateToBeCalled()
{
    XEvent xevent = setupVisibilityNotifyTest();
    xevent.xvisibility.window = INCORRECT_WINDOW_ID;

    QCOMPARE(m_subject->xEventListener->handleXEvent(xevent), false);
    QCOMPARE(viewUpdateCalled, false);
}

void Ut_SwitcherButtonView::testSwitcherButtonsVisibilityEventWithIncorrectVisibilitySendEventDoesNotCauseUpdateToBeCalled()
{
    XEvent xevent = setupVisibilityNotifyTest();
    xevent.xvisibility.send_event = False;

    QCOMPARE(m_subject->xEventListener->handleXEvent(xevent), false);
    QCOMPARE(viewUpdateCalled, false);
}

QTEST_APPLESS_MAIN(Ut_SwitcherButtonView)
