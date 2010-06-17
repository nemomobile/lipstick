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

#include <QtTest/QtTest>
#include <MApplication>
#include <MProgressIndicator>
#include "ut_launcherbuttonview.h"
#include "launcherbuttonview.h"
#include "launcherbutton_stub.h"
#include "launcheraction_stub.h"
#include "homeapplication_stub.h"
#include "x11wrapper.h"

#define ATOM_TYPE_NORMAL 1
#define ATOM_TYPE_NOTIFICATION 2
#define ATOM_TYPE_MENU 3
#define ATOM_TYPE_DIALOG 4
#define ATOM_TYPE_DESKTOP 5
#define ATOM_TYPE_DEFAULT 6

// X11Wrapper Stubs
int X11Wrapper::XSelectInput(Display *, Window , long)
{
    return 0;
}

Status X11Wrapper::XGetWindowAttributes(Display *, Window, XWindowAttributes *)
{
    return 0;
}

Atom X11Wrapper::XInternAtom(Display *, const char *atom_name, Bool)
{
    if (strcmp(atom_name, "_NET_WM_WINDOW_TYPE_NORMAL") == 0) {
        return ATOM_TYPE_NORMAL;
    } else if (strcmp(atom_name, "_NET_WM_WINDOW_TYPE_NOTIFICATION") == 0) {
        return ATOM_TYPE_NOTIFICATION;
    } else if (strcmp(atom_name, "_NET_WM_WINDOW_TYPE_MENU") == 0) {
        return ATOM_TYPE_MENU;
    } else if (strcmp(atom_name, "_NET_WM_WINDOW_TYPE_DIALOG") == 0) {
        return ATOM_TYPE_DIALOG;
    } else if (strcmp(atom_name, "_NET_WM_WINDOW_TYPE_DESKTOP") == 0) {
        return ATOM_TYPE_DESKTOP;
    } else {
        return ATOM_TYPE_DEFAULT;
    }
}

int X11Wrapper::XGetWindowProperty(Display *dpy, Window w, Atom property, long long_offset, long long_length, Bool del, Atom req_type, Atom *actual_type_return, int *actual_format_return, unsigned long *nitems_return, unsigned long *bytes_after_return, unsigned char **prop_return)
{
    Q_UNUSED(dpy);
    Q_UNUSED(property);
    Q_UNUSED(long_offset);
    Q_UNUSED(long_length);
    Q_UNUSED(del);
    Q_UNUSED(req_type);
    Q_UNUSED(actual_type_return);
    Q_UNUSED(actual_format_return);
    Q_UNUSED(bytes_after_return);

    if (w == ATOM_TYPE_NORMAL) {
        *nitems_return = 1;
        *prop_return = new unsigned char[1 * sizeof(Atom)];
        Atom* atom = (Atom *) * prop_return;
        atom[0] = ATOM_TYPE_NORMAL;
        return Success;
    } else if (w == ATOM_TYPE_NOTIFICATION) {
        *nitems_return = 1;
        *prop_return = new unsigned char[1 * sizeof(Atom)];
        Atom* atom = (Atom *) * prop_return;
        atom[0] = ATOM_TYPE_NOTIFICATION;
        return Success;
    } else if (w == ATOM_TYPE_MENU) {
        *nitems_return = 1;
        *prop_return = new unsigned char[1 * sizeof(Atom)];
        Atom* atom = (Atom *) * prop_return;
        atom[0] = ATOM_TYPE_MENU;
        return Success;
    } else if (w == ATOM_TYPE_DIALOG) {
        *nitems_return = 1;
        *prop_return = new unsigned char[1 * sizeof(Atom)];
        Atom* atom = (Atom *) * prop_return;
        atom[0] = ATOM_TYPE_DIALOG;
        return Success;
    } else if (w == ATOM_TYPE_DESKTOP) {
        *nitems_return = 1;
        *prop_return = new unsigned char[1 * sizeof(Atom)];
        Atom* atom = (Atom *) * prop_return;
        atom[0] = ATOM_TYPE_DESKTOP;
        return Success;
    } else if (w == ATOM_TYPE_DEFAULT) {
        *nitems_return = 1;
        *prop_return = new unsigned char[1 * sizeof(Atom)];
        Atom* atom = (Atom *) * prop_return;
        atom[0] = ATOM_TYPE_DEFAULT;
        return Success;
    } else {
        return BadAtom;
    }
}

int X11Wrapper::XFree(void *)
{
    return 0;
}

Status X11Wrapper::XGetWMName(Display *, Window, XTextProperty *)
{
    return -1;
}

Status X11Wrapper::XGetTextProperty(Display *, Window , XTextProperty *, Atom)
{
    return 0;
}

XWMHints *X11Wrapper::XGetWMHints(Display *, Window)
{
    return 0;
}

int X11Wrapper::XFreePixmap(Display *, Pixmap)
{
    return 0;
}

Pixmap X11Wrapper::XCompositeNameWindowPixmap(Display *, Window)
{
    return 0;
}

Damage X11Wrapper::XDamageCreate(Display *, Drawable, int)
{
    return 0;
}

void X11Wrapper::XDamageDestroy(Display *, Damage)
{

}

int X11Wrapper::XSync(Display *, Bool)
{
    return 0;
}

XErrorHandler X11Wrapper::XSetErrorHandler(XErrorHandler)
{
    return 0;
}

int X11Wrapper::XChangeProperty(Display *, Window, Atom, Atom, int, int, unsigned char *, int)
{
    return 0;
}

Status X11Wrapper::XSendEvent(Display *, Window, Bool, long, XEvent *)
{
    return 0;
}

bool qTimerStarted;
void QTimer::start()
{
    qTimerStarted = true;
}

QString mButtonText;
QString MButton::text() const
{
    return mButtonText;
}

void QTimer::stop()
{
    qTimerStarted = false;
}

void Ut_LauncherButtonView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_launcherbuttonview" };
    app = new HomeApplication(argc, app_name);
    mButtonText = "test";
}

void Ut_LauncherButtonView::cleanupTestCase()
{
    delete app;
}

void Ut_LauncherButtonView::init()
{
    qTimerStarted = false;
    controller = new LauncherButton;
    m_subject = new LauncherButtonView(controller);
    connect(this, SIGNAL(windowStackingOrderChanged(const QList<WindowInfo> &)), m_subject, SLOT(hideProgressIndicatorIfObscured(const QList<WindowInfo> &)));
}

void Ut_LauncherButtonView::cleanup()
{
    disconnect(this, SIGNAL(windowStackingOrderChanged(const QList<WindowInfo> &)), m_subject, SLOT(hideProgressIndicatorIfObscured(const QList<WindowInfo> &)));
    delete controller;
}

void Ut_LauncherButtonView::testInitialization()
{
    QVERIFY(disconnect(controller, SIGNAL(clicked()), m_subject, SLOT(showProgressIndicator())));
    QVERIFY(disconnect(&m_subject->progressIndicatorTimer, SIGNAL(timeout()), m_subject, SLOT(hideProgressIndicator())));

    QCOMPARE(m_subject->progressIndicator->property("launcherButtonText").toString(), controller->text());
    QVERIFY(!m_subject->progressIndicator->isVisible());
    QVERIFY(m_subject->progressIndicatorTimer.isSingleShot());
    QVERIFY(!qTimerStarted);
}

void Ut_LauncherButtonView::testShowProgressIndicator()
{
    m_subject->showProgressIndicator();
    QVERIFY(m_subject->progressIndicator->isVisible());
    QVERIFY(m_subject->progressIndicator->unknownDuration());
    QVERIFY(qTimerStarted);
    QVERIFY(disconnect(qApp, SIGNAL(windowStackingOrderChanged(const QList<WindowInfo> &)), m_subject, SLOT(hideProgressIndicatorIfObscured(const QList<WindowInfo> &))));

    qTimerStarted = false;
    m_subject->showProgressIndicator();
    QVERIFY(!qTimerStarted);
    QVERIFY(!disconnect(qApp, SIGNAL(windowStackingOrderChanged(const QList<WindowInfo> &)), m_subject, SLOT(hideProgressIndicatorIfObscured(const QList<WindowInfo> &))));
}

void Ut_LauncherButtonView::testHideProgressIndicator()
{
    m_subject->showProgressIndicator();
    m_subject->hideProgressIndicator();
    QVERIFY(!m_subject->progressIndicator->isVisible());
    QVERIFY(!m_subject->progressIndicator->unknownDuration());
    QVERIFY(!qTimerStarted);
    QVERIFY(!disconnect(qApp, SIGNAL(windowStackingOrderChanged(const QList<WindowInfo> &)), m_subject, SLOT(hideProgressIndicatorIfObscured(const QList<WindowInfo> &))));
}

void Ut_LauncherButtonView::testHideProgressIndicatorIfObscured_data()
{
    QTest::addColumn<int>("topMostWindowId");
    QTest::addColumn<bool>("shouldBeVisible");

    QTest::newRow("_NEW_WM_WINDOW_TYPE_NORMAL") << 1 << false;
    QTest::newRow("_NEW_WM_WINDOW_TYPE_NOTIFICATION") << 2 << true;
    QTest::newRow("_NEW_WM_WINDOW_TYPE_MENU") << 3 << true;
    QTest::newRow("_NEW_WM_WINDOW_TYPE_DIALOG") << 4 << true;
    QTest::newRow("_NEW_WM_WINDOW_TYPE_DESKTOP") << 5 << true;
    QTest::newRow("_NEW_WM_WINDOW_TYPE_DEFAULT") << 6 << false;
}

void Ut_LauncherButtonView::testHideProgressIndicatorIfObscured()
{
    QFETCH(int, topMostWindowId);
    QFETCH(bool, shouldBeVisible);

    m_subject->showProgressIndicator();

    QList<WindowInfo> windowList;
    windowList.append(WindowInfo(topMostWindowId));
    emit windowStackingOrderChanged(windowList);

    QCOMPARE(m_subject->progressIndicator->isVisible(), shouldBeVisible);
}

QTEST_APPLESS_MAIN(Ut_LauncherButtonView)
