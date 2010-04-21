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
#include <MSceneManager>
#include <MScene>
#include "mainwindow_stub.h"
#include "homeapplication_stub.h"

bool Ut_SwitcherButtonView::timerImmediateTimeout = false;
MainWindow *Ut_SwitcherButtonView::mainWindow = NULL;

// QCoreApplication stubs to avoid crashing in processEvents()
QStringList QCoreApplication::arguments()
{
    return QStringList();
}

// X11Wrapper stubs (used by SwitcherButton)
Atom X11Wrapper::XInternAtom(Display *, const char *, Bool)
{
    return 1;
}

int X11Wrapper::XChangeProperty(Display *, Window, Atom, Atom, int, int, unsigned char *, int)
{
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
bool Ut_SwitcherButtonView::damageCreated = false;
unsigned long Ut_SwitcherButtonView::damageHandle = 0;

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

Damage X11Wrapper::XDamageCreate(Display *, Drawable drawable, int)
{
    Ut_SwitcherButtonView::damageCreated = true;
    Ut_SwitcherButtonView::damageHandle = drawable;
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

void TestSwitcherButtonView::emulateCloseButtonClick()
{
    controller->close();
 }

void TestSwitcherButtonView::emulateButtonClick()
{
    QGraphicsSceneMouseEvent event;
    QPointF p = iconRect().center();
    event.setPos(p);
    event.setScenePos(p);
    mousePressEvent(&event);
    mouseReleaseEvent(&event);
}

void TestSwitcherButtonView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *item) const
{
    SwitcherButtonView::drawContents(painter, item);
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
void QTimer::start(int msec)
{
    Q_UNUSED(msec);

    if (Ut_SwitcherButtonView::timerImmediateTimeout) {
        emit timeout();
    }

    id = 0;
}

void QTimer::stop()
{
    id = -1;
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
    allocatedPixmaps.clear();
    lastPixmap = 0;
    xCompositeNameWindowPixmapCausesBadMatch = false;
    xErrorHandler = NULL;
    damageCreated = false;
    painterOpacity = 0;
    painterText.clear();
    painterTextOpacity = 0;
    viewUpdateCalled = false;
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

void Ut_SwitcherButtonView::testMousePressRelease()
{
    // The MButton signals and the windowToFront signal are interesting
    QSignalSpy windowToFrontSpy(button, SIGNAL(windowToFront(Window)));

    // Click the button
    m_subject->emulateButtonClick();

    // Check that windowToFront were emitted
    QCOMPARE(windowToFrontSpy.count(), 1);
}

void Ut_SwitcherButtonView::testClosingWithTimeout()
{
    QSignalSpy closeSpy(button, SIGNAL(closeWindow(Window)));

    MWidget *parent = new MWidget();
    MWidget *switcher = new MWidget(parent);
    button->setParentItem(switcher);

    timerImmediateTimeout = true;

    // "Click" the close button: check that a closeWindow signal is fired
    m_subject->emulateCloseButtonClick();

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
    m_subject->emulateCloseButtonClick();
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

void Ut_SwitcherButtonView::testXWindowWithXError()
{
    // When a composite error occurs setting an X window ID should cause the error handler to be called and no Damage created
    xCompositeNameWindowPixmapCausesBadMatch = true;
    button->model()->setXWindow(1);
    QCOMPARE(allocatedPixmaps.count(), 0);
    QCOMPARE(damageCreated, false);
}

void Ut_SwitcherButtonView::testTextOpacity()
{
    QPainter painter;
    button->model()->setText("test");
    button->model()->setTextVisible(true);
    m_subject->modifiableStyle()->setTextOpacity(0.5);
    m_subject->drawContents(&painter, NULL);
    QCOMPARE(painterText, QString("test"));
    QCOMPARE(painterTextOpacity, m_subject->modifiableStyle()->textOpacity());
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

QTEST_APPLESS_MAIN(Ut_SwitcherButtonView)
