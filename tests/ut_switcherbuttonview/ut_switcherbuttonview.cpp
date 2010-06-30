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

#include <QGLContext>
#include <MScalableImage>
#include <MCancelEvent>
#include "mscenemanager_stub.h"
#include <MScene>
#include <MLabel>
#include "mainwindow_stub.h"
#include "homeapplication_stub.h"
#include "windowinfo_stub.h"

bool Ut_SwitcherButtonView::timerImmediateTimeout = false;
bool Ut_SwitcherButtonView::timerStarted = false;
MainWindow *Ut_SwitcherButtonView::mainWindow = NULL;

const int NAVIGATION_BAR_HEIGHT = 100;

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

// MGLRenderer stubs (used by SwitcherButton)
#ifdef M_USE_OPENGL
MGLRenderer::MGLRenderer() : d_ptr(0)
{
}

MGLRenderer::~MGLRenderer()
{
}

MGLRenderer *MGLRenderer::instance()
{
    static MGLRenderer mGLRenderer;
    return &mGLRenderer;
}

quint32 MGLRenderer::bindX11Pixmap(Pixmap pixmap)
{
    Q_UNUSED(pixmap);
    return 1;
}

void MGLRenderer::unbindX11Pixmap(Pixmap pixmap)
{
    Q_UNUSED(pixmap);
}

void MGLRenderer::drawTexture(const QTransform &transform, quint32 texId, const QSizeF &size, qreal opacity,
                                bool inverted)
{
    Q_UNUSED(transform);
    Q_UNUSED(texId);
    Q_UNUSED(size);
    Q_UNUSED(opacity);
    Q_UNUSED(inverted);
}
#endif

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
unsigned long Ut_SwitcherButtonView::damageHandle = 0;
Display *Ut_SwitcherButtonView::damageDisplay = NULL;
bool Ut_SwitcherButtonView::damageSubtracted = false;
unsigned long Ut_SwitcherButtonView::damageSubtractHandle = NULL;
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
        Ut_SwitcherButtonView::damageCreated = false;
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

// QTimeLine stubs (used by SwitcherButton)
void QTimeLine::start()
{
    emit finished();
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
qreal Ut_SwitcherButtonView::painterOpacity;
void QPainter::setOpacity(qreal opacity)
{
    Ut_SwitcherButtonView::painterOpacity = opacity;
}

QString Ut_SwitcherButtonView::painterText;
qreal Ut_SwitcherButtonView::painterTextOpacity;
void QPainter::drawText(const QRectF &, int, const QString &text, QRectF *)
{
    Ut_SwitcherButtonView::painterText = text;
    Ut_SwitcherButtonView::painterTextOpacity = Ut_SwitcherButtonView::painterOpacity;
}

QRectF Ut_SwitcherButtonView::drawPixmapRect;
QRectF Ut_SwitcherButtonView::drawPixmapSourceRect;
void QPainter::drawPixmap(const QRectF &targetRect, const QPixmap &, const QRectF &sourceRect)
{
    Ut_SwitcherButtonView::drawPixmapRect = targetRect;
    Ut_SwitcherButtonView::drawPixmapSourceRect = sourceRect;
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
TestSwitcherButton::TestSwitcherButton(const QString &title, MWidget *parent, Window window) :
    SwitcherButton(title, parent, window)
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
    if (Ut_SwitcherButtonView::timerImmediateTimeout) {
        emit timeout();
    }

    id = 0;
    Ut_SwitcherButtonView::timerStarted = true;
}

void QTimer::stop()
{
    id = -1;
    Ut_SwitcherButtonView::timerStarted = false;
}

// QPixmap stubs (used by SwitcherButtonView)
QPixmap QPixmap::fromX11Pixmap(Qt::HANDLE, ShareMode)
{
    return QPixmap();
}

void Ut_SwitcherButtonView::init()
{
    button = new TestSwitcherButton("Test");
    m_subject = button->getView();
    connect(this, SIGNAL(windowVisibilityChanged(Window)), m_subject, SLOT(windowVisibilityChanged(Window)));

    // This really shouldn't be necessary, but apparently libdui needs something
    // as stupid as this since commit 9aa354a239. If libdui gets fixed
    // this can be removed.
    QCoreApplication::processEvents();
    // TODO this REALLY REALLY shouldn't be necessary, but apparently there is a
    // bug in Qt 4.6 as of 20090930 that requires this. If Qt gets fixed this
    // can be removed.
    QCoreApplication::processEvents();
    QCoreApplication::processEvents();
    QCoreApplication::processEvents();

    timerImmediateTimeout = false;
    timerStarted = false;
    allocatedPixmaps.clear();
    lastPixmap = 0;
    xCompositeNameWindowPixmapCausesBadMatch = false;
    xErrorHandler = NULL;
    damageCreated = false;
    painterOpacity = 0;
    painterText.clear();
    painterTextOpacity = 0;
    viewUpdateCalled = false;
    damageDisplay = NULL;
    damageSubtracted = false;
    damageSubtractHandle = NULL;
    damageSubtractDisplay = NULL;

}

void Ut_SwitcherButtonView::cleanup()
{
    // Destroy the controller which will also destroy the view
    delete button;

    // Check that whatever Pixmap is allocated is also freed
    QCOMPARE(allocatedPixmaps.count(), 0);
}

void Ut_SwitcherButtonView::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_switcherbutton";
    app = new TestHomeApplication(argc, &app_name);
    mainWindow = MainWindow::instance(true);
    gHomeApplicationStub->stubSetReturnValue("mainWindow", mainWindow);
}

void Ut_SwitcherButtonView::cleanupTestCase()
{
    delete mainWindow;
    delete app;
}

void Ut_SwitcherButtonView::testClosingWithTimeout()
{
    QSignalSpy closeSpy(button, SIGNAL(closeWindow(Window)));

    MWidget *parent = new MWidget();
    MWidget *switcher = new MWidget(parent);
    button->setParentItem(switcher);

    timerImmediateTimeout = true;

    // "Click" the close button: check that a closeWindow signal is fired
    button->getView()->closeButton->click();

    QCOMPARE(closeSpy.count(), 1);

    // The window close timeout occurs immediately: check that after the timeout button is not closed and still visible
    QCOMPARE(button->isVisible(), true);
}

void Ut_SwitcherButtonView::testClosingWithoutTimeout()
{
    QSignalSpy closeSpy(button, SIGNAL(closeWindow(Window)));

    MWidget *parent = new MWidget();
    MWidget *switcher = new MWidget(parent);
    switcher->setGeometry(QRectF(0, 0, 100, 100));
    button->setParentItem(switcher);
    button->setGeometry(QRectF(0, 0, 100, 100));

    // "Click" the close button: check that a closeWindow signal is fired
    button->getView()->closeButton->click();

    QCOMPARE(closeSpy.count(), 1);

    // The window close timeout does not occur: check that button is closed and invisible
    QCOMPARE(button->isVisible(), false);
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

void Ut_SwitcherButtonView::testApplyingStyle()
{
    button->setObjectName("OverviewButton");
    QCOMPARE(m_subject->closeButton->objectName(), QString("CloseButtonOverview"));
    QCOMPARE(m_subject->titleLabel->objectName(), QString("SwitcherButtonTitleLabelOverview"));

    button->setObjectName("DetailviewButton");
    button->model()->setViewMode(SwitcherButtonModel::Large);
    button->getView()->applyStyle();
    QVERIFY(m_subject->closeButton->isVisible());
    QCOMPARE(m_subject->closeButton->objectName(), QString("CloseButtonDetailview"));
    QCOMPARE(m_subject->titleLabel->objectName(), QString("SwitcherButtonTitleLabelDetailview"));

    button->model()->setViewMode(SwitcherButtonModel::Medium);
    button->getView()->applyStyle();
    QVERIFY(!m_subject->closeButton->isVisible());
}

void Ut_SwitcherButtonView::testDamageEventForKnownDamage()
{
    // Create a known damage handle
    button->model()->setXWindow(1);

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
    QVERIFY(!damageCreated);
    QCOMPARE(damageHandle, (unsigned long)0);
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
}

void Ut_SwitcherButtonView::testCloseButtonTranslate()
{
    const qreal translation = 20.0;
    QPointF initialPosition = m_subject->closeButton->scenePos();

    m_subject->modifiableStyle()->setCloseButtonHOffset(translation);
    m_subject->modifiableStyle()->setCloseButtonVOffset(translation);

    m_subject->translateCloseButton();

    QPointF translatedPosition(initialPosition + QPointF(translation, -translation));
    QCOMPARE(m_subject->closeButton->scenePos(), translatedPosition);
}

void Ut_SwitcherButtonView::testBoundingRect()
{
    const int titleHeight = m_subject->titleLabel->size().height();
    const QSize thumbnailSize(20, 20);
    m_subject->modifiableStyle()->setIconSize(thumbnailSize);

    QRectF expectedRect(0, 0, thumbnailSize.width(), titleHeight + thumbnailSize.height());
    QCOMPARE(m_subject->boundingRect(), expectedRect);

    // check that close button translation doesn't affect the bounding rect
    const qreal translation = 20.0;
    m_subject->modifiableStyle()->setCloseButtonHOffset(translation);
    m_subject->modifiableStyle()->setCloseButtonVOffset(translation);
    m_subject->translateCloseButton();

    QCOMPARE(m_subject->boundingRect(), expectedRect);
}

void Ut_SwitcherButtonView::testThumbnailPosition()
{
    const int titleHeight = m_subject->titleLabel->size().height();

    QPoint expectedPosition(0, titleHeight);
    QCOMPARE(m_subject->thumbnailPosition(), expectedPosition);
}

void Ut_SwitcherButtonView::testSignalConnections()
{
    // verify qApp connections
    QVERIFY(disconnect(qApp, SIGNAL(windowVisibilityChanged(Window)), m_subject, SLOT(windowVisibilityChanged(Window))));
    QVERIFY(disconnect(qApp, SIGNAL(damageEvent(Qt::HANDLE &, short &, short &, unsigned short &, unsigned short &)), m_subject, SLOT(damageEvent(Qt::HANDLE &, short &, short &, unsigned short &, unsigned short &))));

    QVERIFY(disconnect(button, SIGNAL(displayEntered()), m_subject, SLOT(setOnDisplay())));
    QVERIFY(disconnect(button, SIGNAL(displayExited()), m_subject, SLOT(unsetOnDisplay())));
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
            << QRectF(0, NAVIGATION_BAR_HEIGHT, Ut_SwitcherButtonView::returnedPixmapWidth, Ut_SwitcherButtonView::returnedPixmapHeight - NAVIGATION_BAR_HEIGHT);

    QTest::newRow("landscape90") << M::Landscape << M::Angle90
            << QRectF(0, 0, thumbnailStyleWidth, thumbnailStyleHeight)
            << QRectF(0, 0, Ut_SwitcherButtonView::returnedPixmapWidth - NAVIGATION_BAR_HEIGHT, Ut_SwitcherButtonView::returnedPixmapHeight);

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
    QPoint thumbnailPosition(0, m_subject->titleLabel->size().height());

    gMSceneManagerStub->stubSetReturnValue("orientation", orientation);
    gMSceneManagerStub->stubSetReturnValue("orientationAngle", orientationAngle);
    m_subject->drawBackground(&painter, NULL);

    // Adjust title height for thumbnail position
    if (orientationAngle == M::Angle0 && orientation == M::Landscape) {
        targetRect.adjust(0, m_subject->titleLabel->size().height(), 0, m_subject->titleLabel->size().height());
    } else if (orientationAngle == M::Angle90 && orientation == M::Landscape) {
        targetRect.adjust(-(m_subject->titleLabel->size().height() + thumbnailStyleWidth), 0, -(m_subject->titleLabel->size().height() + thumbnailStyleWidth), 0);
    }

    QCOMPARE(drawPixmapRect, targetRect);
    QCOMPARE(drawPixmapSourceRect, sourceRect);

    // When the background is drawn the icon geometry should be updated if necessary
    QVERIFY(m_subject->updateXWindowIconGeometryTimer.isActive());
}

void Ut_SwitcherButtonView::testUpdateXWindowIconGeometryIfNecessary()
{
    // When no timer is running and the geometry differs from what has already been set the timer should be started
    m_subject->updateXWindowIconGeometryIfNecessary();
    QVERIFY(timerStarted);

    // If a timer has already been started it should not be started again
    timerStarted = false;
    m_subject->updateXWindowIconGeometryIfNecessary();
    QVERIFY(!timerStarted);

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
    QRectF iconSceneGeometry(QPointF(0, m_subject->titleLabel->size().height()), QSizeF(thumbnailStyleWidth, thumbnailStyleHeight));
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

QTEST_APPLESS_MAIN(Ut_SwitcherButtonView)
