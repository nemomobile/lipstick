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
#include "ut_switcher.h"
#include "windowmonitor.h"
#include "switcher.h"
#include "switcherbutton.h"
#include "switcherview.h"
#include "mscenemanager_stub.h"
#include "mwindow_stub.h"
#include "homewindowmonitor_stub.h"
#include <MApplication>
#include "x11wrapper.h"

// The numbers of different windows the stubs are testing
#define INVALID_WINDOWS 16
#define APPLICATION_WINDOWS 2
#define NON_APPLICATION_WINDOWS 5
#define VALID_WINDOWS (NON_APPLICATION_WINDOWS + APPLICATION_WINDOWS)
#define NUMBER_OF_WINDOWS (INVALID_WINDOWS + VALID_WINDOWS)

#define FIRST_INVALID_WINDOW 0
#define FIRST_APPLICATION_WINDOW (FIRST_INVALID_WINDOW + INVALID_WINDOWS)
#define FIRST_NON_APPLICATION_WINDOW (FIRST_APPLICATION_WINDOW + APPLICATION_WINDOWS)

// Values for X11 atoms
#define ATOM_TYPE 0x00010000
#define ATOM_TYPE_NORMAL 0x00010001
#define ATOM_TYPE_DESKTOP 0x00010002
#define ATOM_TYPE_NOTIFICATION 0x00010003
#define ATOM_TYPE_DIALOG 0x00010004
#define ATOM_TYPE_DOCK 0x00010005
#define ATOM_TYPE_MENU 0x00010006
#define ATOM_TYPE_INVALID 0x0001ffff
#define ATOM_CLIENT_LIST 0x00020000
#define ATOM_CLIENT_LIST_STACKING 0x00030000
#define ATOM_ACTIVE_WINDOW 0x00040000
#define ATOM_CLOSE_WINDOW 0x00050000
#define ATOM_NET_WM_STATE 0x00060000
#define ATOM_STATE_SKIP_TASKBAR 0x00060001
#define ATOM_NET_WM_NAME 0x00070000
#define ATOM_WM_NAME 0x00080000

WId QWidget::winId() const
{
    return 0xbadf00d;
}

static QMap<Window, QVector<Atom> > g_windowTypeMap;
static QMap<Window, QVector<Atom> > g_windowStateMap;
static QVector<Window> g_windows;

// QCoreApplication stubs to avoid crashing in processEvents()
QStringList QCoreApplication::arguments()
{
    return QStringList();
}

// X11 Stubs
static bool gGetWindowTitleSucceeds;
static const unsigned long WINDOW_ID_FOR_WINDOW_TO_FRONT = 666;
static const unsigned long WINDOW_ID_FOR_CLOSE_WINDOW = 777;
static const unsigned long WINDOW_ID_FOR_WINDOW_VISIBLE = 888;
Atom X11Wrapper::XInternAtom(Display *, const char *atom_name, Bool)
{
    if (strcmp(atom_name, "_NET_WM_WINDOW_TYPE") == 0) {
        return ATOM_TYPE;
    } else if (strcmp(atom_name, "_NET_CLIENT_LIST_STACKING") == 0) {
        return ATOM_CLIENT_LIST_STACKING;
    } else if (strcmp(atom_name, "_NET_ACTIVE_WINDOW") == 0) {
        return ATOM_ACTIVE_WINDOW;
    } else if (strcmp(atom_name, "_NET_CLOSE_WINDOW") == 0) {
        return ATOM_CLOSE_WINDOW;
    } else if (strcmp(atom_name, "_NET_WM_STATE") == 0) {
        return ATOM_NET_WM_STATE;
    } else if (strcmp(atom_name, "_NET_WM_NAME") == 0) {
        return ATOM_NET_WM_NAME;
    } else if (strcmp(atom_name, "WM_NAME") == 0) {
        return ATOM_WM_NAME;
    } else if (strcmp(atom_name, "_NET_WM_STATE_SKIP_TASKBAR") == 0) {
        return ATOM_STATE_SKIP_TASKBAR;
    } else if (strcmp(atom_name, "_NET_WM_WINDOW_TYPE_DESKTOP") == 0) {
        return ATOM_TYPE_DESKTOP;
    } else if (strcmp(atom_name, "_NET_WM_WINDOW_TYPE_NOTIFICATION") == 0) {
        return ATOM_TYPE_NOTIFICATION;
    } else if (strcmp(atom_name, "_NET_WM_WINDOW_TYPE_DIALOG") == 0) {
        return ATOM_TYPE_DIALOG;
    } else if (strcmp(atom_name, "_NET_WM_WINDOW_TYPE_MENU") == 0) {
        return  ATOM_TYPE_MENU;
    } else if (strcmp(atom_name, "_NET_WM_WINDOW_TYPE_DOCK") == 0) {
        return ATOM_TYPE_DOCK;
    }
    return ATOM_TYPE_INVALID;
}

int X11Wrapper::XSelectInput(Display *, Window window, long mask)
{
    if ((mask & VisibilityChangeMask) != 0) {
        Ut_Switcher::visibilityNotifyWindows.append(window);
    }
    return 0;
}

Status X11Wrapper::XGetWindowAttributes(Display *, Window w, XWindowAttributes *window_attributes_return)
{
    memset(window_attributes_return, 0, sizeof(XWindowAttributes));
    window_attributes_return->map_state = IsUnmapped;

    switch (w) {
    case 0:
        return 0;
    case 1:
        break;
    case 2:
        window_attributes_return->width = 1;
        break;
    case 3:
        window_attributes_return->width = 1;
        window_attributes_return->height = 1;
        break;
    case 4:
        window_attributes_return->height = 1;
        break;
    case 5:
        window_attributes_return->height = 1;
        window_attributes_return->c_class = InputOutput;
        break;
    case 6:
        window_attributes_return->c_class = InputOutput;
        break;
    case 7:
        window_attributes_return->width = 1;
        window_attributes_return->c_class = InputOutput;
        break;
    case 8:
        window_attributes_return->width = 1;
        window_attributes_return->height = 1;
        window_attributes_return->c_class = InputOutput;
        break;
    case 9:
        window_attributes_return->map_state = IsViewable;
        break;
    case 10:
        window_attributes_return->width = 1;
        window_attributes_return->map_state = IsViewable;
        break;
    case 11:
        window_attributes_return->width = 1;
        window_attributes_return->height = 1;
        window_attributes_return->map_state = IsViewable;
        break;
    case 12:
        window_attributes_return->height = 1;
        window_attributes_return->map_state = IsViewable;
        break;
    case 13:
        window_attributes_return->height = 1;
        window_attributes_return->c_class = InputOutput;
        window_attributes_return->map_state = IsViewable;
        break;
    case 14:
        window_attributes_return->c_class = InputOutput;
        window_attributes_return->map_state = IsViewable;
        break;
    case 15:
        window_attributes_return->width = 1;
        window_attributes_return->c_class = InputOutput;
        window_attributes_return->map_state = IsViewable;
        break;
    default:
        window_attributes_return->width = 100 + w;
        window_attributes_return->height = 200 + w;
        window_attributes_return->c_class = InputOutput;
        window_attributes_return->map_state = IsViewable;
        break;
    }
    return 1;
}

int X11Wrapper::XGetWindowProperty(Display *dpy, Window w, Atom property, long long_offset, long long_length, Bool del, Atom req_type, Atom *actual_type_return, int *actual_format_return, unsigned long *nitems_return, unsigned long *bytes_after_return, unsigned char **prop_return)
{
    Q_UNUSED(long_offset);
    Q_UNUSED(long_length);
    Q_UNUSED(del);
    Q_UNUSED(req_type);
    Q_UNUSED(actual_type_return);
    Q_UNUSED(actual_format_return);
    Q_UNUSED(bytes_after_return);

    if (property == ATOM_CLIENT_LIST_STACKING) {
        if (w != DefaultRootWindow(dpy)) {
            return BadWindow;
        } else {
            *nitems_return = Ut_Switcher::clientListNumberOfWindows;
            *prop_return = new unsigned char[Ut_Switcher::clientListNumberOfWindows * sizeof(Window)];

            Window *windows = (Window *) * prop_return;
            for (int w = 0; w < g_windows.count(); w++) {
                windows[w] = g_windows[w];
            }
            //for (int i = 0; i < Ut_Switcher::clientListNumberOfWindows; i++)
            //windows[i] = i;
            return Success;
        }
    } else if (property == ATOM_TYPE) {
        QVector<Atom> types = g_windowTypeMap[w];
        *nitems_return = types.count();
        *prop_return = new unsigned char[*nitems_return * sizeof(Atom)];
        Atom *atom = (Atom*)*prop_return;
        for (int i = 0; i < types.count(); i++) {
            atom[i] = types[i];
        }
        return Success;
    } else if (property == ATOM_NET_WM_STATE) {
        QVector<Atom> states = g_windowStateMap[w];
        *nitems_return = states.count();
        *prop_return = new unsigned char[*nitems_return * sizeof(Atom)];
        Atom *atom = (Atom*)*prop_return;
        for (int i = 0; i < states.count(); i++) {
            atom[i] = states[i];
        }
        return Success;
    }
    return BadAtom;
}

int X11Wrapper::XFree(void *data)
{
    if (data != NULL) {
        delete [](unsigned char *)data;
    }
    return 0;
}

QMap<Window, QString> x11WrapperWMName;
Status X11Wrapper::XGetWMName(Display *, Window window, XTextProperty *textProperty)
{
    if (gGetWindowTitleSucceeds) {
        QString title = x11WrapperWMName.value(window);
        std::string::size_type strSize = title.toStdString().length();
        textProperty->value = new unsigned char[strSize + 1];
        strncpy((char *)textProperty->value, title.toStdString().c_str(), strSize + 1);
        return Success;
    } else {
        return 0;
    }
}

Status X11Wrapper::XGetTextProperty(Display *, Window window, XTextProperty *text_prop_return, Atom)
{
    if (gGetWindowTitleSucceeds) {
        QString title = x11WrapperWMName.value(window);
        std::string::size_type strSize = title.toStdString().length();
        text_prop_return->value = new unsigned char[strSize + 1];
        strncpy((char *)text_prop_return->value, title.toStdString().c_str(), strSize + 1);
        return 1;
    } else {
        return 0;
    }
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

class SendEventCall {
public:
    SendEventCall(Window w, Bool p, long m, XEvent* e) :
        window(w), propagate(p), event_mask(m), event(*e) {}

    Window      window;
    Bool        propagate;
    long        event_mask;
    XEvent      event;
};

static QList<SendEventCall> x11WrapperSendEventCalls;
Status X11Wrapper::XSendEvent(Display *, Window window, Bool propagate, long mask, XEvent *event)
{
    x11WrapperSendEventCalls.append(SendEventCall(window, propagate, mask, event));
    return 0;
}

QMap<Window, Window> x11WrapperTransientForHint;
Status X11Wrapper::XGetTransientForHint(Display *, Window window, Window *prop_return)
{
    *prop_return = x11WrapperTransientForHint.value(window);
    return 1;
}

QMap<SwitcherButton *, Window> g_windowButtonMap;

// SwitcherButton stubs (used by Switcher)
SwitcherButton::SwitcherButton(const QString &title, MWidget *parent, Window window) :
    MButton(title, parent, new SwitcherButtonModel)
{
    g_windowButtonMap[this] = window;
}

SwitcherButton::~SwitcherButton()
{
    g_windowButtonMap.remove(this);
}

void SwitcherButton::switchToWindow()
{
}

void SwitcherButton::resetState()
{
}

void SwitcherButton::close()
{
}

void SwitcherButton::enterDisplayEvent()
{
}

void SwitcherButton::exitDisplayEvent()
{
}

void SwitcherButton::prepareGeometryChange()
{
    QGraphicsItem::prepareGeometryChange();
}

void SwitcherButton::setXWindow(Window window)
{
    g_windowButtonMap[this] = window;
}

Window SwitcherButton::xWindow()
{
    return g_windowButtonMap[this];
}

class MockWindowMonitor : public WindowMonitor {
public:
    virtual bool isOwnWindow(WId wid) const {
        return ownWindows.contains(wid);
    }

    QList<WId> ownWindows;
};

// QTimer stubs
bool qTimerImmediateTimeout;
void QTimer::start(int)
{
    start();
}

void QTimer::start()
{
    if (qTimerImmediateTimeout) {
        emit timeout();
    }

    id = 0;
}

void QTimer::stop()
{
    id = -1;
}

QList<Window> Ut_Switcher::visibilityNotifyWindows;
int Ut_Switcher::clientListNumberOfWindows;
void Ut_Switcher::init()
{
    // Creating a switcher also creates the switcher view
    mockWindowMonitor = new MockWindowMonitor;
    switcher = new Switcher(mockWindowMonitor);

    visibilityNotifyWindows.clear();

    gGetWindowTitleSucceeds = true;
    g_windowTypeMap.clear();

    clientListNumberOfWindows = NUMBER_OF_WINDOWS;
    g_windows = QVector<Window>(clientListNumberOfWindows);
    for (int w = 0; w < clientListNumberOfWindows; w++) {
        QVector<Atom> types(1);
        types[0] = ATOM_TYPE_NORMAL;
        g_windowTypeMap[w] = types;
        g_windows[w] = w;
    }

    // Add non-application windows
    g_windowTypeMap[FIRST_NON_APPLICATION_WINDOW + 0][0] = ATOM_TYPE_DESKTOP;
    g_windowTypeMap[FIRST_NON_APPLICATION_WINDOW + 1][0] = ATOM_TYPE_NOTIFICATION;
    g_windowTypeMap[FIRST_NON_APPLICATION_WINDOW + 2][0] = ATOM_TYPE_DIALOG;
    g_windowTypeMap[FIRST_NON_APPLICATION_WINDOW + 3][0] = ATOM_TYPE_DOCK;
    g_windowTypeMap[FIRST_NON_APPLICATION_WINDOW + 4][0] = ATOM_TYPE_MENU;
    g_windowStateMap.clear();

    // Configure stubs
    qTimerImmediateTimeout = true;
    x11WrapperWMName.clear();
    x11WrapperSendEventCalls.clear();
    x11WrapperTransientForHint.clear();
}

void Ut_Switcher::cleanup()
{
    // Destroy the switcher (and the associated view)
    delete switcher;
    switcher = NULL;
    mockWindowMonitor = NULL;
}

void Ut_Switcher::initTestCase()
{
    WindowInfo::initializeAtoms();

    // MApplications must be created manually these days due to theme system changes
    static int argc = 1;
    static char *app_name = (char *)"./ut_switcher";
    app = new MApplication(argc, &app_name);

    mSceneManager = new MSceneManager(NULL, NULL);
    gMWindowStub->stubSetReturnValue("sceneManager", mSceneManager);
}

void Ut_Switcher::cleanupTestCase()
{
    delete mSceneManager;
    // Destroy the MApplication
    delete app;
}

void Ut_Switcher::updateWindowList()
{
    XEvent event;
    event.xproperty.atom = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_CLIENT_LIST_STACKING", False);
    event.xproperty.window = DefaultRootWindow(QX11Info::display());
    event.type = PropertyNotify;
    QVERIFY(switcher->handleXEvent(event));
}

void Ut_Switcher::testWindowToFront()
{
    switcher->windowToFront(WINDOW_ID_FOR_WINDOW_TO_FRONT);
    SendEventCall &sec = x11WrapperSendEventCalls.first();
    QCOMPARE(sec.window, DefaultRootWindow(QX11Info::display()));
    QCOMPARE(sec.propagate, False);
    QCOMPARE(sec.event_mask, StructureNotifyMask);
    QCOMPARE(sec.event.xclient.type, ClientMessage);
    QCOMPARE(sec.event.xclient.window, (Window)WINDOW_ID_FOR_WINDOW_TO_FRONT);
    QCOMPARE(sec.event.xclient.message_type, (Atom)ATOM_ACTIVE_WINDOW);
    QCOMPARE(sec.event.xclient.format, 32);
    QCOMPARE(sec.event.xclient.data.l[0], 1L);
    QCOMPARE(sec.event.xclient.data.l[1], CurrentTime);
    QCOMPARE(sec.event.xclient.data.l[2], 0L);
}

void Ut_Switcher::testCloseWindow()
{
    switcher->closeWindow(WINDOW_ID_FOR_CLOSE_WINDOW);
    SendEventCall &sec = x11WrapperSendEventCalls.first();
    QCOMPARE(sec.window, DefaultRootWindow(QX11Info::display()));
    QCOMPARE(sec.propagate, False);
    QCOMPARE(sec.event_mask, SubstructureRedirectMask);
    QCOMPARE(sec.event.xclient.type, ClientMessage);
    QCOMPARE(sec.event.xclient.window, (Window)WINDOW_ID_FOR_CLOSE_WINDOW);
    QCOMPARE(sec.event.xclient.message_type, (Atom)ATOM_CLOSE_WINDOW);
    QCOMPARE(sec.event.xclient.format, 32);
    QCOMPARE(sec.event.xclient.data.l[0], CurrentTime);
    QCOMPARE(sec.event.xclient.data.l[1], (long)QX11Info::appRootWindow(QX11Info::appScreen()));
    QCOMPARE(sec.event.xclient.data.l[2], 0L);
}

void Ut_Switcher::verifyModel(const QList<WindowInfo> &windowList)
{
    // There given windows should be in the switcher model
    QCOMPARE(switcher->model()->buttons().count(), windowList.count());

    for (int i = 0; i < windowList.count(); i++) {
        // The buttons should match the windows
        SwitcherButton *b = switcher->model()->buttons().at(i).data();
        QCOMPARE(b->xWindow(), windowList.at(i).window());
        QCOMPARE(b->text(), windowList.at(i).title());
    }
}

void Ut_Switcher::testX11EventFilterWithPropertyNotify()
{
    WindowListReceiver r;
    connect(switcher, SIGNAL(windowListUpdated(const QList<WindowInfo> &)), &r, SLOT(windowListUpdated(const QList<WindowInfo> &)));

    // Verify that X11EventFilter only reacts to events it's supposed to react on (type, window and atom are all set correctly)
    XEvent event;
    event.type = 0;
    event.xproperty.window = 0;
    event.xproperty.atom = 0;
    QVERIFY(!switcher->handleXEvent(event));
    event.type = PropertyNotify;
    QVERIFY(!switcher->handleXEvent(event));
    event.xproperty.window = DefaultRootWindow(QX11Info::display());
    QVERIFY(!switcher->handleXEvent(event));
    event.type = 0;
    QVERIFY(!switcher->handleXEvent(event));

    event.xproperty.atom = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_CLIENT_LIST_STACKING", False);
    QVERIFY(!switcher->handleXEvent(event));

    event.xproperty.window = 0;
    QVERIFY(!switcher->handleXEvent(event));
    event.type = PropertyNotify;
    QVERIFY(!switcher->handleXEvent(event));

    event.xproperty.window = DefaultRootWindow(QX11Info::display());
    QVERIFY(switcher->handleXEvent(event));

    // Make sure the window list change signal was emitted
    QCOMPARE(r.count, 1);
    QCOMPARE(r.windowList.count(), APPLICATION_WINDOWS);
    verifyModel(r.windowList);

    // Update the state of a window that currently is an application window
    QVector<Atom> states(1);
    states[0] = ATOM_STATE_SKIP_TASKBAR;
    g_windowStateMap[FIRST_APPLICATION_WINDOW] = states;
    event.xproperty.window = INVALID_WINDOWS;
    event.xproperty.atom = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_WM_STATE", False);
    QVERIFY(switcher->handleXEvent(event));
    QCOMPARE(r.count, 2);
    QCOMPARE(r.windowList.count(), APPLICATION_WINDOWS - 1);
    verifyModel(r.windowList);

    // Update the type of a window that currently is an application window
    g_windowTypeMap[FIRST_APPLICATION_WINDOW + 1][0] = ATOM_TYPE_MENU;
    event.xproperty.window = FIRST_APPLICATION_WINDOW + 1;
    event.xproperty.atom = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_WM_WINDOW_TYPE", False);
    QVERIFY(switcher->handleXEvent(event));
    QCOMPARE(r.count, 3);
    QCOMPARE(r.windowList.count(), 0);
    verifyModel(r.windowList);
}

void Ut_Switcher::testX11EventFilterWithVisibilityNotify()
{
    WindowVisibilityReceiver r;
    connect(switcher, SIGNAL(windowVisibilityChanged(Window)), &r, SLOT(windowVisibilityChanged(Window)));

    // Verify that X11EventFilter only reacts to events it's supposed to react on
    XEvent event;
    event.type = VisibilityNotify;
    event.xvisibility.window = 303;
    event.xvisibility.state = VisibilityUnobscured;
    event.xvisibility.send_event = TRUE;

    // Make sure the window visibility change signal is not emitted if state is VisibilityUnobscured
    QVERIFY(!switcher->handleXEvent(event));
    QCOMPARE(r.windowList.count(), 0);

    // Make sure the window visibility change signal was emitted with the given window if state is VisibilityFullyObscured
    event.xvisibility.state = VisibilityFullyObscured;
    QVERIFY(switcher->handleXEvent(event));
    QCOMPARE(r.windowList.count(), 1);
    QCOMPARE(r.windowList.at(0), (Window)303);

    // Make sure the window visibility change signal is not emitted if send_event is FALSE but the event is processed
    event.xvisibility.send_event = FALSE;
    QVERIFY(switcher->handleXEvent(event));
    QCOMPARE(r.windowList.count(), 1);

    // Make sure the window visibility change signal is not emitted if the window is the homescreen itself
    WId HOME_WINDOW_ID = 1001;
    mockWindowMonitor->ownWindows.append(HOME_WINDOW_ID);
    event.xvisibility.window = HOME_WINDOW_ID;
    QVERIFY(!switcher->handleXEvent(event));
    QCOMPARE(r.windowList.count(), 1);
}

void Ut_Switcher::testX11EventFilterWithClientMessage()
{
    WindowListReceiver r;
    connect(switcher, SIGNAL(windowListUpdated(const QList<WindowInfo> &)), &r, SLOT(windowListUpdated(const QList<WindowInfo> &)));

    // Verify that X11EventFilter only reacts to events it's supposed to react on (type and message_type are both set correctly)
    XEvent clientEvent;
    clientEvent.type = 0;
    clientEvent.xclient.message_type = 0;
    QVERIFY(!switcher->handleXEvent(clientEvent));
    clientEvent.type = ClientMessage;
    QVERIFY(!switcher->handleXEvent(clientEvent));
    clientEvent.type = 0;
    clientEvent.xclient.message_type = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_CLOSE_WINDOW", False);
    QVERIFY(!switcher->handleXEvent(clientEvent));
    clientEvent.type = ClientMessage;
    clientEvent.xclient.window = FIRST_APPLICATION_WINDOW;
    QVERIFY(switcher->handleXEvent(clientEvent));

    // When the stacking list is updated the closed window should be excluded
    updateWindowList();

    // Make sure the window list change signal was not emitted
    QCOMPARE(r.count, 1);

    // There should be 2 windows in the window list
    QCOMPARE(r.windowList.count(), APPLICATION_WINDOWS - 1);
    verifyModel(r.windowList);
}

void Ut_Switcher::testWindowStackingOrder()
{
    WindowListReceiver r;
    connect(switcher, SIGNAL(windowStackingOrderChanged(const QList<WindowInfo> &)), &r, SLOT(stackingWindowListUpdated(const QList<WindowInfo> &)));

    updateWindowList();

    QCOMPARE(r.stackingCount, 1);
    QCOMPARE(r.stackingWindowList.count(), VALID_WINDOWS);

    WindowInfo first = r.stackingWindowList.first();
    WindowInfo last = r.stackingWindowList.last();

    QCOMPARE(first.window(), (unsigned long)(FIRST_APPLICATION_WINDOW));
    QCOMPARE(last.window(), (unsigned long)g_windows.count() - 1);

    Window tmp = g_windows[FIRST_APPLICATION_WINDOW];
    g_windows[FIRST_APPLICATION_WINDOW] = g_windows.last();
    g_windows[g_windows.count() - 1] = tmp;

    updateWindowList();
    QCOMPARE(r.stackingCount, 2);
    QCOMPARE(r.stackingWindowList.count(), VALID_WINDOWS);

    QCOMPARE(first.window(), r.stackingWindowList.last().window());
    QCOMPARE(last.window(), r.stackingWindowList.first().window());
}

void Ut_Switcher::testX11EventWindowNameChange_data()
{
    QTest::addColumn<bool>("success");
    QTest::addColumn<Window>("window");
    QTest::addColumn<QString>("property");
    QTest::addColumn<QString>("oldTitle");
    QTest::addColumn<QString>("newTitle");

    QTest::newRow("_NET_WM_NAME fails") << false << (Window) FIRST_APPLICATION_WINDOW << "_NET_WM_NAME" << "Old" << "New";
    QTest::newRow("_NET_WM_NAME succeeds") << true << (Window) FIRST_APPLICATION_WINDOW << "_NET_WM_NAME" << "Old" << "New";
}

QString Ut_Switcher::titleForWindow(Window window, const QList<WindowInfo> &windowList)
{
    foreach (const WindowInfo &info, windowList) {
        if (info.window() == window) {
            return info.title();
        }
    }
    return QString();
}

void Ut_Switcher::testX11EventWindowNameChange()
{
    QFETCH(bool, success);
    QFETCH(Window, window);
    QFETCH(QString, property);
    QFETCH(QString, oldTitle);
    QFETCH(QString, newTitle);

    WindowListReceiver r;
    connect(switcher, SIGNAL(windowListUpdated(const QList<WindowInfo> &)), &r, SLOT(windowListUpdated(const QList<WindowInfo> &)));
    x11WrapperWMName[window] = oldTitle;

    // Change the client list so that a window exists
    updateWindowList();
    verifyModel(r.windowList);
    QCOMPARE(titleForWindow(window, r.windowList), oldTitle);

    // Update the title of a window
    XEvent event;
    event.xproperty.atom = X11Wrapper::XInternAtom(QX11Info::display(), property.toUtf8(), False);
    event.type = PropertyNotify;
    event.xproperty.window = window;
    gGetWindowTitleSucceeds = success;
    x11WrapperWMName[window] = newTitle;
    QVERIFY(switcher->handleXEvent(event));
    verifyModel(r.windowList);

    // The title should only change if getting it succeeds
    QCOMPARE(titleForWindow(window, r.windowList), success ? newTitle : oldTitle);
}

void Ut_Switcher::testUpdateDelay()
{
    WindowListReceiver r;
    connect(switcher, SIGNAL(windowListUpdated(const QList<WindowInfo> &)), &r, SLOT(windowListUpdated(const QList<WindowInfo> &)));
    qTimerImmediateTimeout = false;

    updateWindowList();

    // Make sure the window list change signal was not emitted
    QCOMPARE(r.count, 0);

    // There should be no items in the switcher model yet
    QCOMPARE(switcher->model()->buttons().count(), 0);

    // But the update should have been scheduled
    QVERIFY(switcher->updateButtonsTimer.isActive());
}

void Ut_Switcher::testTransientWindowOpening()
{
    WindowListReceiver r;
    connect(switcher, SIGNAL(windowListUpdated(const QList<WindowInfo> &)), &r, SLOT(windowListUpdated(const QList<WindowInfo> &)));

    // Configure the windows
    x11WrapperWMName[FIRST_APPLICATION_WINDOW] = "First";
    x11WrapperWMName[FIRST_APPLICATION_WINDOW + 1] = "Second";
    x11WrapperTransientForHint[FIRST_APPLICATION_WINDOW + 1] = FIRST_APPLICATION_WINDOW;
    updateWindowList();

    // The window list should contain only one window which should represent the second window
    QCOMPARE(r.count, 1);
    QCOMPARE(r.windowList.count(), APPLICATION_WINDOWS - 1);
    QCOMPARE(switcher->model()->buttons().count(), APPLICATION_WINDOWS - 1);
    SwitcherButton *b = switcher->model()->buttons().at(0).data();
    QCOMPARE(b->xWindow(), (Window)(FIRST_APPLICATION_WINDOW + 1));
    QCOMPARE(b->text(), x11WrapperWMName[FIRST_APPLICATION_WINDOW + 1]);
}

void Ut_Switcher::testTransiencyChanges()
{
    WindowListReceiver r;
    connect(switcher, SIGNAL(windowListUpdated(const QList<WindowInfo> &)), &r, SLOT(windowListUpdated(const QList<WindowInfo> &)));

    // Configure the windows
    x11WrapperWMName[FIRST_APPLICATION_WINDOW] = "First";
    x11WrapperWMName[FIRST_APPLICATION_WINDOW + 1] = "Second";
    updateWindowList();

    // Update the state of a window that currently is an application window to transient
    x11WrapperTransientForHint[FIRST_APPLICATION_WINDOW + 1] = FIRST_APPLICATION_WINDOW;
    XEvent event;
    event.type = PropertyNotify;
    event.xproperty.window = INVALID_WINDOWS + 1;
    event.xproperty.atom = XA_WM_TRANSIENT_FOR;
    QVERIFY(switcher->handleXEvent(event));

    // The window list should contain only one window which should represent the second window
    QCOMPARE(r.count, 2);
    QCOMPARE(r.windowList.count(), APPLICATION_WINDOWS - 1);
    QCOMPARE(switcher->model()->buttons().count(), APPLICATION_WINDOWS - 1);
    SwitcherButton *b = switcher->model()->buttons().at(0).data();
    QCOMPARE(b->xWindow(), (Window)(FIRST_APPLICATION_WINDOW + 1));
    QCOMPARE(b->text(), x11WrapperWMName[FIRST_APPLICATION_WINDOW + 1]);

    // Update the state of a window that currently is a transient window back to normal
    x11WrapperTransientForHint.clear();
    QVERIFY(switcher->handleXEvent(event));

    // The window list should contain only both windows again
    QCOMPARE(r.count, 3);
    verifyModel(r.windowList);
}

void Ut_Switcher::testTransientWindowClosing()
{
    // Configure the windows
    x11WrapperWMName[FIRST_APPLICATION_WINDOW] = "First";
    x11WrapperWMName[FIRST_APPLICATION_WINDOW + 1] = "Second";
    x11WrapperTransientForHint[FIRST_APPLICATION_WINDOW + 1] = FIRST_APPLICATION_WINDOW;
    updateWindowList();

    switcher->closeWindow(FIRST_APPLICATION_WINDOW + 1);
    QCOMPARE(x11WrapperSendEventCalls.count(), 2);
    for (int i = 0; i < 2; i++) {
        const SendEventCall &sec = x11WrapperSendEventCalls.at(i);
        QCOMPARE(sec.window, DefaultRootWindow(QX11Info::display()));
        QCOMPARE(sec.propagate, False);
        QCOMPARE(sec.event_mask, SubstructureRedirectMask);
        QCOMPARE(sec.event.xclient.type, ClientMessage);
        QCOMPARE(sec.event.xclient.window, (Window)(FIRST_APPLICATION_WINDOW + 1 - i));
        QCOMPARE(sec.event.xclient.message_type, (Atom)ATOM_CLOSE_WINDOW);
        QCOMPARE(sec.event.xclient.format, 32);
        QCOMPARE(sec.event.xclient.data.l[0], CurrentTime);
        QCOMPARE(sec.event.xclient.data.l[1], (long)QX11Info::appRootWindow(QX11Info::appScreen()));
        QCOMPARE(sec.event.xclient.data.l[2], 0L);
    }
}

QTEST_APPLESS_MAIN(Ut_Switcher)
