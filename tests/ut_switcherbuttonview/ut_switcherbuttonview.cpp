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
#include "ut_switcherbuttonview.h"

#include <QGLContext>
#include <DuiScalableImage>
#include <DuiCancelEvent>
#include <DuiSceneManager>
#include <DuiScene>
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

// DuiTheme stubs
void DuiTheme::releasePixmap(const QPixmap *)
{
}

// DUIGLRenderer stubs (used by SwitcherButton)
#ifdef DUI_USE_OPENGL
DuiGLRenderer::DuiGLRenderer() : d_ptr(0)
{
}

DuiGLRenderer::~DuiGLRenderer()
{
}

DuiGLRenderer *DuiGLRenderer::instance()
{
    static DuiGLRenderer duiGLRenderer;
    return &duiGLRenderer;
}

quint32 DuiGLRenderer::bindX11Pixmap(Pixmap pixmap)
{
    Q_UNUSED(pixmap);
    return 1;
}

void DuiGLRenderer::unbindX11Pixmap(Pixmap pixmap)
{
    Q_UNUSED(pixmap);
}

void DuiGLRenderer::drawTexture(const QTransform &transform, quint32 texId, const QSizeF &size, qreal opacity,
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

Damage X11Wrapper::XDamageCreate(Display *, Drawable, int)
{
    Ut_SwitcherButtonView::damageCreated = true;
    return 0;
}

void X11Wrapper::XDamageDestroy(Display *, Damage)
{
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

// DuiScalableImage stubs
const DuiScalableImage *Ut_SwitcherButtonView::drawnScalableImage;

void DuiScalableImage::draw(const QRect &rect, QPainter *painter) const
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

// Test switcher button implementation
TestSwitcherButton::TestSwitcherButton(const QString &title, DuiWidget *parent, Window window) :
    SwitcherButton(title, parent, window)
{
    view = new TestSwitcherButtonView(*this);
    setView(view);
}

TestSwitcherButtonView *TestSwitcherButton::getView()
{
    return dynamic_cast<TestSwitcherButtonView *>(view);
}

// Test switcher button view implementation
TestSwitcherButtonView::TestSwitcherButtonView(SwitcherButton &button) :
    SwitcherButtonView(&button)
{
}

void TestSwitcherButtonView::mousePressRelease(const QPointF &p)
{
    mousePress(p);
    mouseRelease(p);
}

void TestSwitcherButtonView::mousePress(const QPointF &p)
{
    QGraphicsSceneMouseEvent event;
    event.setPos(p);
    event.setScenePos(p);

    mousePressEvent(&event);
}

void TestSwitcherButtonView::mouseMove(const QPointF &p)
{
    QGraphicsSceneMouseEvent event;
    event.setPos(p);
    event.setScenePos(p);

    mouseMoveEvent(&event);
}

void TestSwitcherButtonView::mouseRelease(const QPointF &p)
{
    QGraphicsSceneMouseEvent event;
    event.setPos(p);
    event.setScenePos(p);

    mouseReleaseEvent(&event);
}

void TestSwitcherButtonView::mouseCancel()
{
    DuiCancelEvent event;
    cancelEvent(&event);
}

void TestSwitcherButtonView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *item) const
{
    SwitcherButtonView::drawContents(painter, item);
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
    view = button->getView();
    connect(this, SIGNAL(windowVisibilityChanged(Window)), view, SLOT(windowVisibilityChanged(Window)));

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
    app = new HomeApplication(argc, &app_name);
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
    // The DuiButton signals and the windowToFront signal are interesting
    QSignalSpy pressedSpy(button, SIGNAL(pressed()));
    QSignalSpy clickedSpy(button, SIGNAL(clicked()));
    QSignalSpy windowToFrontSpy(button, SIGNAL(windowToFront(Window)));

    button->setGeometry(QRectF(0, 0, 100, 100));

    // Click the button
    view->mousePressRelease(view->buttonRect().center());

    // Check that pressed, clicked and windowToFront were emitted
    QCOMPARE(pressedSpy.count(), 1);
    QCOMPARE(clickedSpy.count(), 1);
    QCOMPARE(windowToFrontSpy.count(), 1);
}

void Ut_SwitcherButtonView::testClosingWithTimeout()
{
    QSignalSpy closeSpy(button, SIGNAL(closeWindow(Window)));
    QSignalSpy clickedSpy(button, SIGNAL(clicked()));

    DuiWidget *parent = new DuiWidget();
    DuiWidget *switcher = new DuiWidget(parent);
    switcher->setGeometry(QRectF(0, 0, 100, 100));
    button->setParentItem(switcher);
    button->setGeometry(QRectF(0, 0, 100, 100));

    timerImmediateTimeout = true;

    // Click the close button: check that a closeWindow signal is fired and clicked is not
    QRectF closeRect = view->closeRect();
    view->mousePressRelease(closeRect.center());
    QCOMPARE(closeSpy.count(), 1);
    QCOMPARE(clickedSpy.count(), 0);

    // The window close timeout occurs immediately: check that after the timeout button is not closed and still visible
    QCOMPARE(button->isVisible(), true);
}

void Ut_SwitcherButtonView::testClosingWithoutTimeout()
{
    QSignalSpy closeSpy(button, SIGNAL(closeWindow(Window)));
    QSignalSpy clickedSpy(button, SIGNAL(clicked()));

    DuiWidget *parent = new DuiWidget();
    DuiWidget *switcher = new DuiWidget(parent);
    switcher->setGeometry(QRectF(0, 0, 100, 100));
    button->setParentItem(switcher);
    button->setGeometry(QRectF(0, 0, 100, 100));

    // Click the close button: check that a closeWindow signal is fired and clicked is not
    view->mousePressRelease(view->closeRect().center());
    QCOMPARE(closeSpy.count(), 1);
    QCOMPARE(clickedSpy.count(), 0);

    // The window close timeout does not occur: check that button is closed and invisible
    QCOMPARE(button->isVisible(), false);
}

void Ut_SwitcherButtonView::testDragOutsideDoesNothing()
{
    QSignalSpy clickedSpy(button, SIGNAL(clicked()));
    QSignalSpy windowToFrontSpy(button, SIGNAL(windowToFront(Window)));
    QSignalSpy closeSpy(button, SIGNAL(closeWindow(Window)));

    DuiWidget *parent = new DuiWidget();
    DuiWidget *switcher = new DuiWidget(parent);
    switcher->setGeometry(QRectF(0, 0, 200, 200));
    button->setParentItem(switcher);
    button->setGeometry(QRectF(0, 0, 100, 100));

    // Test that mouse press on the button, drag off and release triggers
    // no actions
    view->mousePress(view->buttonRect().center());
    view->mouseMove(QPointF(200, 200));
    view->mouseRelease(QPointF(200, 200));
    QCOMPARE(clickedSpy.count(), 0);
    QCOMPARE(windowToFrontSpy.count(), 0);
    QCOMPARE(closeSpy.count(), 0);

    // Test that mouse press on the close button, drag off and release
    // triggers no actions
    view->mousePress(view->closeRect().center());
    view->mouseMove(QPointF(200, 200));
    view->mouseRelease(QPointF(200, 200));
    QCOMPARE(clickedSpy.count(), 0);
    QCOMPARE(windowToFrontSpy.count(), 0);
    QCOMPARE(closeSpy.count(), 0);

    // Test that mouse press elsewhere on the switcher, drag to the button
    // and release triggers no actions
    view->mousePress(QPointF(200, 200));
    view->mouseMove(view->buttonRect().center());
    view->mouseRelease(view->buttonRect().center());
    QCOMPARE(clickedSpy.count(), 0);
    QCOMPARE(windowToFrontSpy.count(), 0);
    QCOMPARE(closeSpy.count(), 0);

    // Test that mouse press elsewhere on the switcher, drag to the close
    // button and release triggers no actions
    view->mousePress(QPointF(200, 200));
    view->mouseMove(view->closeRect().center());
    view->mouseRelease(view->closeRect().center());
    QCOMPARE(clickedSpy.count(), 0);
    QCOMPARE(windowToFrontSpy.count(), 0);
    QCOMPARE(closeSpy.count(), 0);
}

void Ut_SwitcherButtonView::testDragInsideDoesNothing()
{
    QSignalSpy clickedSpy(button, SIGNAL(clicked()));
    QSignalSpy windowToFrontSpy(button, SIGNAL(windowToFront(Window)));
    QSignalSpy closeSpy(button, SIGNAL(closeWindow(Window)));

    DuiWidget *parent = new DuiWidget();
    DuiWidget *switcher = new DuiWidget(parent);
    switcher->setGeometry(QRectF(0, 0, 200, 200));
    button->setParentItem(switcher);
    button->setGeometry(QRectF(0, 0, 100, 100));

    // Test that mouse press on the button, drag to the close button and
    // release triggers no actions
    view->mousePress(view->buttonRect().center());
    view->mouseMove(view->closeRect().center());
    view->mouseRelease(view->closeRect().center());
    QCOMPARE(clickedSpy.count(), 0);
    QCOMPARE(windowToFrontSpy.count(), 0);
    QCOMPARE(closeSpy.count(), 0);

    // Test that mouse press on the close button, drag to the switcher
    // button and release triggers no actions
    view->mousePress(view->closeRect().center());
    view->mouseMove(view->buttonRect().center());
    view->mouseRelease(view->buttonRect().center());
    QCOMPARE(clickedSpy.count(), 0);
    QCOMPARE(windowToFrontSpy.count(), 0);
    QCOMPARE(closeSpy.count(), 0);

}

void Ut_SwitcherButtonView::testXWindow()
{
    // Setting an X window ID should allocate a new Pixmap and create Damage
    button->model()->setXWindow(1);
    QCOMPARE(allocatedPixmaps.count(), 1);

    Pixmap oldPixmap = allocatedPixmaps.at(0);
    QCOMPARE(oldPixmap, (Pixmap)1);
    QCOMPARE(damageCreated, true);

    // Setting another X window ID should free the previously allocated Pixmap and allocate a new one
    button->model()->setXWindow(2);
    QCOMPARE(allocatedPixmaps.count(), 1);
    QVERIFY(allocatedPixmaps.at(0) != oldPixmap);
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
    view->modifiableStyle()->setTextOpacity(0.5);
    view->drawContents(&painter, NULL);
    QCOMPARE(painterText, QString("test"));
    QCOMPARE(painterTextOpacity, view->modifiableStyle()->textOpacity());
}

void Ut_SwitcherButtonView::testEmphasis()
{
    for (int i = 0; i < 2; ++i) {
	button->setEmphasis();
	QCOMPARE(QString("emphasized"), view->styleContainer().currentMode());
	button->unsetEmphasis();
	QCOMPARE(QString("default"), view->styleContainer().currentMode());
    }
 }

QTEST_APPLESS_MAIN(Ut_SwitcherButtonView)
