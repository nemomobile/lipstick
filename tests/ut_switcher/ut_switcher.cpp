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
#include <MApplicationPage>
#include "ut_switcher.h"
#include "switcher.h"
#include "switcherbutton.h"
#include "switcherview.h"
#include "x11wrapper.h"
#include "mscenemanager_stub.h"
#include "mwindow_stub.h"
#include "mapplication_stub.h"
#include "x11wrapper.h"

// The numbers of different windows the stubs are testing
#define INVALID_WINDOWS 16
#define APPLICATION_WINDOWS 2
#define NON_APPLICATION_WINDOWS 5
#define VALID_WINDOWS (NON_APPLICATION_WINDOWS + APPLICATION_WINDOWS)
#define NUMBER_OF_WINDOWS (INVALID_WINDOWS + VALID_WINDOWS)

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
static QString gWindowTitle;
static int gTypeOfXSendEvent;
static Atom gMessageTypeOfXSendEvent;
static int gFormatOfXSendEvent;
static Window gWindowOfXSendEvent;
static Display* gDisplayOfXSendEvent;
static long gMaskOfXSendEvent;
static Bool gPropagateFlagOfXSendEvent;
static long gLongParametersOfXSendEvent[5];
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

Status X11Wrapper::XGetWMName(Display *, Window, XTextProperty *textProperty)
{
    if (gGetWindowTitleSucceeds) {
        std::string::size_type strSize = gWindowTitle.toStdString().length();
        textProperty->value = new unsigned char[strSize + 1];
        strncpy((char *)textProperty->value, gWindowTitle.toStdString().c_str(), strSize + 1);
        return Success;
    } else {
        return 0;
    }
}

Status X11Wrapper::XGetTextProperty(Display *, Window, XTextProperty *text_prop_return, Atom)
{
    if (gGetWindowTitleSucceeds) {
        std::string::size_type strSize = gWindowTitle.toStdString().length();
        text_prop_return->value = new unsigned char[strSize + 1];
        strncpy((char *)text_prop_return->value, gWindowTitle.toStdString().c_str(), strSize + 1);
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

Status X11Wrapper::XSendEvent(Display *display, Window,
                              Bool propagate, long mask, XEvent *event)
{
    gTypeOfXSendEvent = event->type;
    gDisplayOfXSendEvent = display;
    gMaskOfXSendEvent = mask;
    gPropagateFlagOfXSendEvent = propagate;
    if (event->type == ClientMessage) {
        gFormatOfXSendEvent = event->xclient.format;
        gMessageTypeOfXSendEvent = event->xclient.message_type;
        gWindowOfXSendEvent = event->xclient.window;
        gLongParametersOfXSendEvent[0] = event->xclient.data.l[0];
        gLongParametersOfXSendEvent[1] = event->xclient.data.l[1];
        if (gMessageTypeOfXSendEvent == ATOM_ACTIVE_WINDOW) {
            gLongParametersOfXSendEvent[2] = event->xclient.data.l[2];
        }
    }
    return 0;
}

QMap<SwitcherButton *, Window> g_windowButtonMap;
QString g_lastSingleShot;
QString g_singleShotTarget;

// Home stubs
class Home : public MApplicationPage
{
public:
    Home(QGraphicsItem *parent = 0);
};

Home::Home(QGraphicsItem *parent) : MApplicationPage(parent)
{
}


MainWindow *g_testMainWindow;

MainWindow::MainWindow()
{
}

MainWindow::~MainWindow()
{
}

MainWindow *MainWindow::instance(bool)
{
    return g_testMainWindow;
}

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

Window SwitcherButton::xWindow()
{
    return g_windowButtonMap[this];
}

void QTimer::singleShot(int msec, QObject *receiver, const char *member)
{
    Q_UNUSED(msec);

    if( !g_singleShotTarget.isEmpty() ) {
        QMetaObject::invokeMethod(receiver, g_singleShotTarget.toAscii().data(), Qt::DirectConnection);
    }

    g_lastSingleShot = QString(member);
}

QList<Window> Ut_Switcher::visibilityNotifyWindows;
int Ut_Switcher::clientListNumberOfWindows;
void Ut_Switcher::init()
{
    gWindowTitle.clear();
    gTypeOfXSendEvent = 0;
    gMessageTypeOfXSendEvent = 0;
    gFormatOfXSendEvent = 0;
    gWindowOfXSendEvent = 0;
    gDisplayOfXSendEvent = NULL;
    gMaskOfXSendEvent = 0;
    gPropagateFlagOfXSendEvent = TRUE;
    for (int i = 0; i < sizeof(gLongParametersOfXSendEvent) / sizeof(long); i++) {
        gLongParametersOfXSendEvent[i] = 0;
    }

    // Creating a switcher also creates the switcher view
    switcher = Switcher::instance();

    g_lastSingleShot = QString();
    g_singleShotTarget = "updateButtons";

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
    g_windowTypeMap[INVALID_WINDOWS + APPLICATION_WINDOWS + 0][0] = ATOM_TYPE_DESKTOP;
    g_windowTypeMap[INVALID_WINDOWS + APPLICATION_WINDOWS + 1][0] = ATOM_TYPE_NOTIFICATION;
    g_windowTypeMap[INVALID_WINDOWS + APPLICATION_WINDOWS + 2][0] = ATOM_TYPE_DIALOG;
    g_windowTypeMap[INVALID_WINDOWS + APPLICATION_WINDOWS + 3][0] = ATOM_TYPE_DOCK;
    g_windowTypeMap[INVALID_WINDOWS + APPLICATION_WINDOWS + 4][0] = ATOM_TYPE_MENU;

    g_windowStateMap.clear();
}

void Ut_Switcher::cleanup()
{
    // Destroy the switcher (and the associated view)
    delete switcher;
}

void Ut_Switcher::initTestCase()
{
    WindowInfo::initializeAtoms();

    // MApplications must be created manually these days due to theme system changes
    static int argc = 1;
    static char *app_name = (char *)"./ut_switcher";
    app = new MApplication(argc, &app_name);

    g_testMainWindow = new MainWindow();
    mSceneManager = new MSceneManager(NULL, NULL);
    gMWindowStub->stubSetReturnValue("sceneManager", mSceneManager);
}

void Ut_Switcher::cleanupTestCase()
{
    delete mSceneManager;
    // Destroy the MApplication
    delete app;
}

static void testXSendEventCommonValues()
{
    QCOMPARE(gTypeOfXSendEvent, ClientMessage);
    QCOMPARE(gDisplayOfXSendEvent, QX11Info::display());
    QVERIFY(!gPropagateFlagOfXSendEvent);
    QCOMPARE(gFormatOfXSendEvent, 32);
}

void Ut_Switcher::testWindowToFront()
{
    switcher->windowToFront(WINDOW_ID_FOR_WINDOW_TO_FRONT);
    testXSendEventCommonValues();
    QCOMPARE(gMessageTypeOfXSendEvent, (Atom)ATOM_ACTIVE_WINDOW);
    QCOMPARE(gWindowOfXSendEvent, WINDOW_ID_FOR_WINDOW_TO_FRONT);
    QCOMPARE(gMaskOfXSendEvent, StructureNotifyMask);
    QCOMPARE(gLongParametersOfXSendEvent[0], 1L);
    QCOMPARE(gLongParametersOfXSendEvent[1], CurrentTime);
    QCOMPARE(gLongParametersOfXSendEvent[2], 0L);
}

void Ut_Switcher::testCloseWindow()
{
    switcher->closeWindow(WINDOW_ID_FOR_CLOSE_WINDOW);
    testXSendEventCommonValues();
    QCOMPARE(gMessageTypeOfXSendEvent, (Atom)ATOM_CLOSE_WINDOW);
    QCOMPARE(gWindowOfXSendEvent, WINDOW_ID_FOR_CLOSE_WINDOW);
    QCOMPARE(gMaskOfXSendEvent, SubstructureRedirectMask);
    QCOMPARE(gLongParametersOfXSendEvent[0], CurrentTime);
    QCOMPARE(gLongParametersOfXSendEvent[1], (long)QX11Info::appRootWindow(QX11Info::appScreen()));
}

void Ut_Switcher::verifyModel(const QList<WindowInfo> &windowList)
{
    // There given windows should be in the switcher model
    QCOMPARE(switcher->model()->buttons().count(), windowList.count());

    for (int i = 0; i < windowList.count(); i++) {
        // The button titles should match the window names
        SwitcherButton *b = switcher->model()->buttons().at(i).data();
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
    QVERIFY(!switcher->handleX11Event(&event));
    event.type = PropertyNotify;
    QVERIFY(!switcher->handleX11Event(&event));
    event.xproperty.window = DefaultRootWindow(QX11Info::display());
    QVERIFY(!switcher->handleX11Event(&event));
    event.type = 0;
    QVERIFY(!switcher->handleX11Event(&event));

    event.xproperty.atom = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_CLIENT_LIST_STACKING", False);
    QVERIFY(!switcher->handleX11Event(&event));

    event.xproperty.window = 0;
    QVERIFY(!switcher->handleX11Event(&event));
    event.type = PropertyNotify;
    QVERIFY(!switcher->handleX11Event(&event));

    event.xproperty.window = DefaultRootWindow(QX11Info::display());
    QVERIFY(switcher->handleX11Event(&event));

    // Make sure the window list change signal was emitted
    QCOMPARE(r.count, 1);
    QCOMPARE(r.windowList.count(), APPLICATION_WINDOWS);
    verifyModel(r.windowList);

    // Update the state of a window that currently is an application window
    QVector<Atom> states(1);
    states[0] = ATOM_STATE_SKIP_TASKBAR;
    g_windowStateMap[INVALID_WINDOWS] = states;
    event.xproperty.window = INVALID_WINDOWS;
    event.xproperty.atom = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_WM_STATE", False);
    QVERIFY(switcher->handleX11Event(&event));
    QCOMPARE(r.count, 2);
    QCOMPARE(r.windowList.count(), APPLICATION_WINDOWS - 1);
    verifyModel(r.windowList);

    // Update the type of a window that currently is an application window
    g_windowTypeMap[INVALID_WINDOWS + 1][0] = ATOM_TYPE_MENU;
    event.xproperty.window = INVALID_WINDOWS + 1;
    event.xproperty.atom = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_WM_WINDOW_TYPE", False);
    QVERIFY(switcher->handleX11Event(&event));
    QCOMPARE(r.count, 3);
    QCOMPARE(r.windowList.count(), 0);
    verifyModel(r.windowList);
}

void Ut_Switcher::testX11EventFilterWithVisibilityNotify()
{
    MainWindow *w = MainWindow::instance(true);
    QList<MWindow *> windowList;
    windowList.append(w);
    gMApplicationStub->stubSetReturnValue<QList<MWindow *> >("windows", windowList);

    int x11EventFilterCallCount;
    WindowVisibilityReceiver r;
    connect(switcher, SIGNAL(windowVisibilityChanged(Window)), &r, SLOT(windowVisibilityChanged(Window)));

    // Verify that X11EventFilter only reacts to events it's supposed to react on
    XEvent event;
    event.type = VisibilityNotify;
    event.xvisibility.window = 303;
    event.xvisibility.state = VisibilityUnobscured;
    event.xvisibility.send_event = TRUE;

    // Make sure the window visibility change signal is not emitted if state is VisibilityUnobscured
    x11EventFilterCallCount = gMApplicationStub->stubCallCount("x11EventFilter");
    QVERIFY(!switcher->handleX11Event(&event));
    QCOMPARE(r.windowList.count(), 0);

    // Make sure the window visibility change signal was emitted with the given window if state is VisibilityFullyObscured
    x11EventFilterCallCount = gMApplicationStub->stubCallCount("x11EventFilter");
    event.xvisibility.state = VisibilityFullyObscured;
    QVERIFY(switcher->handleX11Event(&event));
    QCOMPARE(r.windowList.count(), 1);
    QCOMPARE(r.windowList.at(0), (Window)303);

    // Make sure the window visibility change signal is not emitted if send_event is FALSE but the event is processed
    event.xvisibility.send_event = FALSE;
    QVERIFY(switcher->handleX11Event(&event));
    QCOMPARE(r.windowList.count(), 1);

    // Make sure the window visibility change signal is not emitted if the window is the homescreen itself
    event.xvisibility.window = w->winId();
    QVERIFY(!switcher->handleX11Event(&event));
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
    QVERIFY(!switcher->handleX11Event(&clientEvent));
    clientEvent.type = ClientMessage;
    QVERIFY(!switcher->handleX11Event(&clientEvent));
    clientEvent.type = 0;
    clientEvent.xclient.message_type = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_CLOSE_WINDOW", False);
    QVERIFY(!switcher->handleX11Event(&clientEvent));
    clientEvent.type = ClientMessage;
    clientEvent.xclient.window = INVALID_WINDOWS + 1; // This is the first application window id
    QVERIFY(switcher->handleX11Event(&clientEvent));

    // Make sure the window list change signal was emitted
    QCOMPARE(r.count, 1);
    // We have closed one window
    QCOMPARE(r.windowList.count(), APPLICATION_WINDOWS - 1);
    verifyModel(r.windowList);

    // Change the client list so that the window being closed was actually closed
    XEvent propertyEvent;
    propertyEvent.type = PropertyNotify;
    propertyEvent.xproperty.window = DefaultRootWindow(QX11Info::display());
    propertyEvent.xproperty.atom = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_CLIENT_LIST_STACKING", False);
    QVERIFY(switcher->handleX11Event(&propertyEvent));

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

    XEvent event;
    event.xproperty.atom = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_CLIENT_LIST_STACKING", False);
    event.xproperty.window = DefaultRootWindow(QX11Info::display());
    event.type = PropertyNotify;
    QVERIFY(switcher->handleX11Event(&event));

    QCOMPARE(r.stackingCount, 1);
    QCOMPARE(r.stackingWindowList.count(), VALID_WINDOWS);

    WindowInfo first = r.stackingWindowList.first();
    WindowInfo last = r.stackingWindowList.last();

    QCOMPARE(first.window(), (unsigned long)(INVALID_WINDOWS));
    QCOMPARE(last.window(), (unsigned long)g_windows.count() - 1);

    Window tmp = g_windows[INVALID_WINDOWS];
    g_windows[INVALID_WINDOWS] = g_windows.last();
    g_windows[g_windows.count() - 1] = tmp;

    QVERIFY(switcher->handleX11Event(&event));
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

    QTest::newRow("_NET_WM_NAME fails") << false << (Window) INVALID_WINDOWS << "_NET_WM_NAME" << "Old" << "New";
    QTest::newRow("_NET_WM_NAME succeeds") << true << (Window) INVALID_WINDOWS << "_NET_WM_NAME" << "Old" << "New";
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
    gWindowTitle = oldTitle;

    // Change the client list so that a window exists
    XEvent event;
    event.xproperty.atom = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_CLIENT_LIST_STACKING", False);
    event.type = PropertyNotify;
    event.xproperty.window = DefaultRootWindow(QX11Info::display());
    QVERIFY(switcher->handleX11Event(&event));
    verifyModel(r.windowList);
    QCOMPARE(titleForWindow(window, r.windowList), oldTitle);

    // Update the title of a window
    event.xproperty.atom = X11Wrapper::XInternAtom(QX11Info::display(), property.toUtf8(), False);
    event.type = PropertyNotify;
    event.xproperty.window = window;
    gGetWindowTitleSucceeds = success;
    gWindowTitle = newTitle;
    QVERIFY(switcher->handleX11Event(&event));
    verifyModel(r.windowList);

    // The title should only change if getting it succeeds
    QCOMPARE(titleForWindow(window, r.windowList), success ? newTitle : oldTitle);
}

void Ut_Switcher::testUpdateDelay()
{
    // Prevent immediate update
    g_singleShotTarget.clear();

    WindowListReceiver r;
    connect(switcher, SIGNAL(windowListUpdated(const QList<WindowInfo> &)), &r, SLOT(windowListUpdated(const QList<WindowInfo> &)));

    XEvent event;
    event.xproperty.atom = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_CLIENT_LIST_STACKING", False);
    event.type = PropertyNotify;
    event.xproperty.window = DefaultRootWindow(QX11Info::display());

    QVERIFY(switcher->handleX11Event(&event));

    // Make sure the window list change signal was not emitted
    QCOMPARE(r.count, 0);

    // There should be no items in the switcher model yet
    QCOMPARE(switcher->model()->buttons().count(), 0);

    // But the update should have been scheduled
    QVERIFY(g_lastSingleShot.contains("updateButtons"));
}

QTEST_APPLESS_MAIN(Ut_Switcher)
