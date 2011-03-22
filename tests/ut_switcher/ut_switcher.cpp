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
#include "switcherview_stub.h"
#include "mscenemanager_stub.h"
#include "mwindow_stub.h"
#include "homewindowmonitor_stub.h"
#include "xeventlistener_stub.h"
#include <MApplication>
#include "x11wrapper.h"

// The numbers of different windows the stubs are testing
#define INVALID_WINDOWS 16
#define APPLICATION_WINDOWS 2
#define NON_APPLICATION_WINDOWS 7
#define VALID_WINDOWS (NON_APPLICATION_WINDOWS + APPLICATION_WINDOWS)
#define NUMBER_OF_WINDOWS (INVALID_WINDOWS + VALID_WINDOWS)

#define FIRST_INVALID_WINDOW 0
#define FIRST_APPLICATION_WINDOW (FIRST_INVALID_WINDOW + INVALID_WINDOWS)
#define FIRST_NON_APPLICATION_WINDOW (FIRST_APPLICATION_WINDOW + APPLICATION_WINDOWS)
#define SKIP_TASKBAR_WINDOW (FIRST_NON_APPLICATION_WINDOW + NON_APPLICATION_WINDOWS - 1)

// Values for X11 atoms
#define ATOM_TYPE 0x00010000
#define ATOM_TYPE_NORMAL 0x00010001
#define ATOM_TYPE_DESKTOP 0x00010002
#define ATOM_TYPE_NOTIFICATION 0x00010003
#define ATOM_TYPE_DIALOG 0x00010004
#define ATOM_TYPE_DOCK 0x00010005
#define ATOM_TYPE_MENU 0x00010006
#define ATOM_TYPE_INPUT_WINDOW 0x00010007
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
    } else if (strcmp(atom_name, "_NET_WM_WINDOW_TYPE_INPUT") == 0) {
        return ATOM_TYPE_INPUT_WINDOW;
    }
    return ATOM_TYPE_INVALID;
}

int X11Wrapper::XSelectInput(Display *, Window window, long mask)
{
    if ((mask & VisibilityChangeMask) != 0) {
        Ut_Switcher::visibilityNotifyWindows.append(window);
    }
    if ((mask & PropertyChangeMask) != 0) {
        Ut_Switcher::propertyNotifyWindows.append(window);
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
            *prop_return = new unsigned char[g_windows.count() * sizeof(Window)];

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
QMap<SwitcherButton*, bool> gSwitcherButtonVisibleInSwitcherProperty;

// SwitcherButton stubs (used by Switcher)
SwitcherButton::SwitcherButton(QGraphicsItem *parent, SwitcherButtonModel *model) :
    MButton(parent, model)
{
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
    gSwitcherButtonVisibleInSwitcherProperty[this] = true;
}

void SwitcherButton::exitDisplayEvent()
{
    gSwitcherButtonVisibleInSwitcherProperty[this] = false;
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

QMap<SwitcherButton*, bool> gSwitcherButtonVisibilityEnabled;
void SwitcherButton::setVisibilityPropertyEnabled(bool enable)
{
    gSwitcherButtonVisibilityEnabled[this] = enable;
}

void SwitcherButton::setVisibleInSwitcherProperty(bool set)
{
    gSwitcherButtonVisibleInSwitcherProperty[this] = set;
}

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
        // Set the id to less than zero to deactivate
        id = -1;
        return;
    }

    id = 0;
}

void QTimer::stop()
{
    id = -1;
}

QList<QPair<QGraphicsItem*, QGraphicsItem*> > gQGraphicsItem_removeSceneEventFilter;
void QGraphicsItem::removeSceneEventFilter(QGraphicsItem *filterItem)
{
    gQGraphicsItem_removeSceneEventFilter.append(qMakePair(this, filterItem));
}

QList<Window> Ut_Switcher::visibilityNotifyWindows;
QList<Window> Ut_Switcher::propertyNotifyWindows;
int Ut_Switcher::clientListNumberOfWindows;
void Ut_Switcher::init()
{
    homeWindowMonitor = new HomeWindowMonitor();
    gHomeWindowMonitorStub->stubSetReturnValue("instance", homeWindowMonitor);
    // Creating a switcher also creates the switcher view
    switcher = new Switcher();

    visibilityNotifyWindows.clear();
    propertyNotifyWindows.clear();

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
    g_windowTypeMap[FIRST_NON_APPLICATION_WINDOW + 5][0] = ATOM_TYPE_INPUT_WINDOW;

    g_windowStateMap.clear();
    QVector<Atom> states(1);
    states[0] = ATOM_STATE_SKIP_TASKBAR;
    g_windowStateMap[SKIP_TASKBAR_WINDOW] = states;

    // Configure stubs
    qTimerImmediateTimeout = true;
    x11WrapperWMName.clear();
    x11WrapperSendEventCalls.clear();
    x11WrapperTransientForHint.clear();

    gQGraphicsItem_removeSceneEventFilter.clear();
    gSwitcherButtonVisibilityEnabled.clear();
    gSwitcherButtonVisibleInSwitcherProperty.clear();
}

void Ut_Switcher::cleanup()
{
    // Destroy the switcher (and the associated view)
    delete switcher;
    switcher = NULL;
    gHomeWindowMonitorStub->stubReset();
    delete homeWindowMonitor;
    homeWindowMonitor = NULL;
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

void populateWindowList(QList<WindowInfo> &newInfoWindowList)
{
    foreach (Window window, g_windows) {
        newInfoWindowList.append(WindowInfo(window));
    }
}

void addApplicationWindow(Window window)
{
    g_windows.append(window);

    QVector<Atom> types(1);
    types[0] = ATOM_TYPE_NORMAL;
    g_windowTypeMap[window] = types;
}

void removeApplicationWindow(Window window)
{
    g_windows.remove(window);
}

void moveApplicationWindowTop(Window window)
{
    g_windows.remove(g_windows.indexOf(window));
    g_windows.insert(0, window);
}

void Ut_Switcher::updateWindowList()
{
    QList<WindowInfo> newWindowList;
    populateWindowList(newWindowList);
    switcher->handleWindowInfoList(newWindowList);
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

void Ut_Switcher::testWhenCloseWindowThenButtonRemovedFromModel()
{
    updateWindowList();
    WindowListReceiver receiver;
    connect(switcher, SIGNAL(windowListUpdated(const QList<WindowInfo> &)), &receiver,
            SLOT(windowListUpdated(const QList<WindowInfo> &)));

    switcher->closeWindow(FIRST_APPLICATION_WINDOW);

    // Verify that button is in windowsBeingClosed and not in the model
    QCOMPARE(switcher->windowsBeingClosed.count(), 1);
    QVERIFY(switcher->windowsBeingClosed.contains(FIRST_APPLICATION_WINDOW));
    QList<WindowInfo> remainingWindows = receiver.windowList;
    remainingWindows.removeOne(WindowInfo(FIRST_APPLICATION_WINDOW));
    verifyModel(remainingWindows);
}

void Ut_Switcher::testCloseAllWindows()
{
    int numWindows = 5;

    // Add some windows to the model
    QList<QSharedPointer<SwitcherButton> > buttons;
    for (int window = 1; window <= numWindows; window++) {
        QSharedPointer<SwitcherButton> button(new SwitcherButton);
        button->setXWindow(window);
        buttons.append(button);
    }

    switcher->model()->setButtons(buttons);
    switcher->closeAllWindows();

    // Each window should be closed
    for (int window = 1; window <= numWindows; window++) {
        const SendEventCall &sec = x11WrapperSendEventCalls.at(window - 1);
        QCOMPARE(sec.window, DefaultRootWindow(QX11Info::display()));
        QCOMPARE(sec.propagate, False);
        QCOMPARE(sec.event_mask, SubstructureRedirectMask);
        QCOMPARE(sec.event.xclient.type, ClientMessage);
        QCOMPARE(sec.event.xclient.window, (Window)window);
        QCOMPARE(sec.event.xclient.message_type, (Atom)ATOM_CLOSE_WINDOW);
        QCOMPARE(sec.event.xclient.format, 32);
        QCOMPARE(sec.event.xclient.data.l[0], CurrentTime);
        QCOMPARE(sec.event.xclient.data.l[1], (long)QX11Info::appRootWindow(QX11Info::appScreen()));
        QCOMPARE(sec.event.xclient.data.l[2], 0L);
    }
}

void Ut_Switcher::verifyModel(const QList<WindowInfo> &windowList)
{
    // There given windows should be in the switcher model
    QCOMPARE(switcher->model()->buttons().count(), windowList.count());

    // The model should contain all and only the buttons for the windows (not necessary in same order)
    foreach (const WindowInfo &windowInfo, windowList) {
        bool contains = false;
        foreach(const QSharedPointer<SwitcherButton> &button, switcher->model()->buttons()) {
            if ((button->xWindow() == windowInfo.window()) && button->text() == windowInfo.title()) {
                contains = true;
                break;
            }
        }
        QVERIFY(contains);
    }
}

void Ut_Switcher::testX11EventFilterWithPropertyNotify()
{
    WindowListReceiver r;
    connect(switcher, SIGNAL(windowListUpdated(const QList<WindowInfo> &)), &r,
            SLOT(windowListUpdated(const QList<WindowInfo> &)));

    // Verify that X11EventFilter only reacts to events it's supposed to react on
    // (type, window and atom are all set correctly)
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

    QList<WindowInfo> newWindowList;
    populateWindowList(newWindowList);
    switcher->handleWindowInfoList(newWindowList);

    // Make sure the window list change signal was emitted
    QCOMPARE(r.count, 1);
    QCOMPARE(r.windowList.count(), APPLICATION_WINDOWS);
    verifyModel(r.windowList);

    // Update the state of a window that currently isn't an application window
    g_windowStateMap[SKIP_TASKBAR_WINDOW] = QVector<Atom>();
    event.xproperty.window = SKIP_TASKBAR_WINDOW;
    event.xproperty.atom = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_WM_STATE", False);

    // Check that we're actually listening for property changes for this window
    QVERIFY(propertyNotifyWindows.contains(SKIP_TASKBAR_WINDOW));

    QVERIFY(switcher->handleXEvent(event));
    QCOMPARE(r.count, 2);
    QCOMPARE(r.windowList.count(), APPLICATION_WINDOWS + 1);
    verifyModel(r.windowList);

    // Update the type of a window that currently is an application window
    QVERIFY(propertyNotifyWindows.contains(FIRST_APPLICATION_WINDOW + 1));
    g_windowTypeMap[FIRST_APPLICATION_WINDOW + 1][0] = ATOM_TYPE_MENU;
    event.xproperty.window = FIRST_APPLICATION_WINDOW + 1;
    event.xproperty.atom = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_WM_WINDOW_TYPE", False);
    QVERIFY(switcher->handleXEvent(event));
    QCOMPARE(r.count, 3);
    QCOMPARE(r.windowList.count(), APPLICATION_WINDOWS);
    verifyModel(r.windowList);
}

void Ut_Switcher::testX11EventFilterWithClientMessage_data()
{
    QTest::addColumn<int>("type");
    QTest::addColumn<Atom>("messageType");
    QTest::addColumn<Window>("window");

    QTest::addColumn<bool>("verifySuccess");

    QTest::newRow("Wrong event and message type") << 0 << Atom(0) << Window(0) << false;
    QTest::newRow("Wrong message type") << ClientMessage << Atom(0) << Window(0) << false;
    QTest::newRow("Wrong event type")
            << 0 << X11Wrapper::XInternAtom(QX11Info::display(), "_NET_CLOSE_WINDOW", False) << Window(0) << false;
    QTest::newRow("Wrong window")
            << ClientMessage << X11Wrapper::XInternAtom(QX11Info::display(), "_NET_CLOSE_WINDOW", False)
            << Window(0) << true;
    QTest::newRow("Correct event")
            << ClientMessage << X11Wrapper::XInternAtom(QX11Info::display(), "_NET_CLOSE_WINDOW", False)
            << Window(FIRST_APPLICATION_WINDOW) << true;
}

void Ut_Switcher::testX11EventFilterWithClientMessage()
{
    QFETCH(int, type);
    QFETCH(Atom, messageType);
    QFETCH(Window, window);

    QFETCH(bool, verifySuccess);

    // Update window list with an application
    QList<WindowInfo> newWindowList;
    newWindowList.append(WindowInfo(g_windows[0]));
    switcher->handleWindowInfoList(newWindowList);

    XEvent clientEvent;
    clientEvent.type = type;
    clientEvent.xclient.message_type = messageType;
    clientEvent.xclient.window = window;
    QCOMPARE(switcher->handleXEvent(clientEvent), verifySuccess);
}

void Ut_Switcher::testX11EventFilterCloseWindow()
{
    updateWindowList();

    WindowListReceiver receiver;
    connect(switcher, SIGNAL(windowListUpdated(const QList<WindowInfo> &)), &receiver, SLOT(windowListUpdated(const QList<WindowInfo> &)));

    XEvent clientEvent;
    clientEvent.type = ClientMessage;
    clientEvent.xclient.message_type = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_CLOSE_WINDOW", False);
    clientEvent.xclient.window = Window(FIRST_APPLICATION_WINDOW);
    switcher->handleXEvent(clientEvent);

    // _NET_CLOSE_WINDOW shouldn't affect to the window list
    QCOMPARE(receiver.windowList.count(), APPLICATION_WINDOWS);

    // Verify that button is in windowsBeingClosed and not in the model
    QCOMPARE(switcher->windowsBeingClosed.count(), 1);
    QVERIFY(switcher->windowsBeingClosed.contains(FIRST_APPLICATION_WINDOW));
    QList<WindowInfo> remainingWindows = receiver.windowList;
    remainingWindows.removeOne(WindowInfo(FIRST_APPLICATION_WINDOW));
    verifyModel(remainingWindows);
}

void Ut_Switcher::testX11EventFilterCloseWindowForNonExistentWindow()
{
    WindowListReceiver receiver;
    connect(switcher, SIGNAL(windowListUpdated(const QList<WindowInfo> &)), &receiver, SLOT(windowListUpdated(const QList<WindowInfo> &)));
    updateWindowList();

    // Try to close a non-existant window
    XEvent clientEvent;
    clientEvent.type = ClientMessage;
    clientEvent.xclient.message_type = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_CLOSE_WINDOW", False);
    clientEvent.xclient.window = 234;
    switcher->handleXEvent(clientEvent);

    QCOMPARE(switcher->windowsBeingClosed.count(), 0);
    verifyModel(receiver.windowList);
    // Now open a window with the same id, the previous
    // close event shouldn't affect this
    addApplicationWindow(234);
    updateWindowList();

    // Make sure the window list change signal was emitted
    QCOMPARE(receiver.windowList.count(), 3);
    verifyModel(receiver.windowList);
}

void Ut_Switcher::testWhenStackingOrderChangesCorrectWindowsAreStored()
{
    QList<WindowInfo> windowInfoList;
    populateWindowList(windowInfoList);

    switcher->handleWindowInfoList(windowInfoList);

    QCOMPARE(switcher->windowInfoSet.count(), VALID_WINDOWS);
    for (Window window = FIRST_APPLICATION_WINDOW; window < (Window)g_windows.count(); ++window) {
        QVERIFY(switcher->windowInfoSet.contains(WindowInfo(window)));
    }
}

void Ut_Switcher::testWhenStackingOrderChangesTopmostWindowGetsUpdated()
{
    QList<WindowInfo> windowInfoList;
    populateWindowList(windowInfoList);

    switcher->handleWindowInfoList(windowInfoList);

    QCOMPARE(switcher->model()->topmostWindow(), g_windows[FIRST_NON_APPLICATION_WINDOW + NON_APPLICATION_WINDOWS - 1]);

    Window tmp = g_windows[FIRST_APPLICATION_WINDOW];
    g_windows[FIRST_APPLICATION_WINDOW] = g_windows.last();
    g_windows[g_windows.count() - 1] = tmp;

    windowInfoList.clear();
    populateWindowList(windowInfoList);

    switcher->handleWindowInfoList(windowInfoList);

    QCOMPARE(switcher->model()->topmostWindow(), g_windows[FIRST_NON_APPLICATION_WINDOW + NON_APPLICATION_WINDOWS - 1]);
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
    connect(switcher, SIGNAL(windowListUpdated(const QList<WindowInfo> &)), &r,
            SLOT(windowListUpdated(const QList<WindowInfo> &)));
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

#undef None
void Ut_Switcher::testWhenNonTouchEndSceneEventArrivesThenSceneEventFilteringForSwitcherButtonsIsNotAffected()
{
    QList<QSharedPointer<SwitcherButton> > buttons;
    buttons.append(QSharedPointer<SwitcherButton>(new SwitcherButton));
    switcher->model()->setButtons(buttons);

    QEvent nonTouchEndEvent(QEvent::None);
    QCOMPARE(switcher->sceneEvent(&nonTouchEndEvent), false);
    QCOMPARE(gQGraphicsItem_removeSceneEventFilter.count(), 0);
}

void Ut_Switcher::testWhenTouchEndSceneEventArrivesThenSceneEventFilteringForSwitcherButtonStops()
{
    QList<QSharedPointer<SwitcherButton> > buttons;
    buttons.append(QSharedPointer<SwitcherButton>(new SwitcherButton));
    switcher->model()->setButtons(buttons);

    QEvent touchEndEvent(QEvent::TouchEnd);
    QCOMPARE(switcher->sceneEvent(&touchEndEvent), true);
    QCOMPARE(gQGraphicsItem_removeSceneEventFilter.count(), 1);
    QCOMPARE(gQGraphicsItem_removeSceneEventFilter.at(0).first, buttons.at(0).data());
    QCOMPARE(gQGraphicsItem_removeSceneEventFilter.at(0).second, switcher);
}

void Ut_Switcher::testWhenAnimationStatusChangesThenButtonVisibilityPropertySettingIsUpdated()
{
    QList<QSharedPointer<SwitcherButton> > buttons;
    buttons.append(QSharedPointer<SwitcherButton>(new SwitcherButton));
    buttons.append(QSharedPointer<SwitcherButton>(new SwitcherButton));
    buttons.append(QSharedPointer<SwitcherButton>(new SwitcherButton));
    switcher->model()->setButtons(buttons);

    switcher->updateAnimationStatus(true);
    foreach(QSharedPointer<SwitcherButton> button, buttons) {
        QVERIFY(gSwitcherButtonVisibilityEnabled.contains(button.data()));
        QVERIFY(!gSwitcherButtonVisibilityEnabled[button.data()]);
    }

    gSwitcherButtonVisibilityEnabled.clear();

    switcher->updateAnimationStatus(false);
    foreach(QSharedPointer<SwitcherButton> button, buttons) {
        QVERIFY(gSwitcherButtonVisibilityEnabled.contains(button.data()));
        QVERIFY(gSwitcherButtonVisibilityEnabled[button.data()]);
    }
}

void Ut_Switcher::testThatIfApplicationWindowAcquiresSkipTaskbarPropertyButtonIsRemovedFromSwitcher()
{
    updateWindowList();

    Window window = FIRST_APPLICATION_WINDOW;

    // Give the skip taskbar property to the first application window
    g_windowStateMap.clear();
    QVector<Atom> states(1);
    states[0] = ATOM_STATE_SKIP_TASKBAR;
    g_windowStateMap[window] = states;

    XEvent event;
    event.type = PropertyNotify;
    event.xproperty.window = window;
    event.xproperty.atom = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_WM_STATE", False);
    switcher->handleXEvent(event);

    QCOMPARE(switcher->model()->buttons().count(), APPLICATION_WINDOWS - 1);
    QVERIFY(Switcher::windowInfoFromSet(switcher->windowInfoSet, window) != NULL);
    QVERIFY(Switcher::windowInfoFromSet(switcher->applicationWindows.toSet(), window) == NULL);
}

void Ut_Switcher::testThatSwitcherButtonVisibleInSwitcherPropertyIsSetToFalseWhenApplicationWindowIsRemoved()
{
    Window window = 101;

    // Add an application window that will be shown in switcher
    addApplicationWindow(window);
    updateWindowList();

    // Find the button for the new window
    QSharedPointer<SwitcherButton> button;
    foreach(QSharedPointer<SwitcherButton> b, switcher->model()->buttons()) {
        if (b->xWindow() == window) {
            button = b;
        }
    }

    switcher->applicationWindows.removeAll(WindowInfo(window));

    switcher->updateButtons();

    QCOMPARE(switcher->model()->buttons().count(), APPLICATION_WINDOWS);
    QVERIFY(Switcher::windowInfoFromSet(switcher->windowInfoSet, window) != NULL);
    QCOMPARE(gSwitcherButtonVisibleInSwitcherProperty.contains(button.data()), true);
    QCOMPARE(gSwitcherButtonVisibleInSwitcherProperty[button.data()], false);
}

void Ut_Switcher::testRestoringButtonBeingClosedWhenWindowComesOnTop()
{
    updateWindowList();
    WindowListReceiver receiver;
    connect(switcher, SIGNAL(windowListUpdated(const QList<WindowInfo> &)), &receiver,
            SLOT(windowListUpdated(const QList<WindowInfo> &)));

    // Close window so it gets added to the buttons being closed list
    const Window CLOSING_WINDOW = FIRST_APPLICATION_WINDOW + 1;
    switcher->closeWindow(CLOSING_WINDOW);

    QCOMPARE(switcher->windowsBeingClosed.count(), 1);
    QCOMPARE(switcher->model()->buttons().count(), APPLICATION_WINDOWS - 1);

    // Move window being closed to top and update window list
    QList<WindowInfo> newWindowList = receiver.windowList;
    newWindowList.move(newWindowList.indexOf(WindowInfo(CLOSING_WINDOW)), newWindowList.count() - 1);
    switcher->handleWindowInfoList(newWindowList);

    QCOMPARE(switcher->windowsBeingClosed.count(), 0);
    QVERIFY(!switcher->windowsBeingClosed.contains(CLOSING_WINDOW));
    verifyModel(receiver.windowList);
}

void Ut_Switcher::testRestoringButtonBeingClosedWhenButtonCloseTimerTimeouts()
{
    updateWindowList();
    WindowListReceiver receiver;
    connect(switcher, SIGNAL(windowListUpdated(const QList<WindowInfo> &)), &receiver,
            SLOT(windowListUpdated(const QList<WindowInfo> &)));

    // Close window so it gets added to the buttons being closed list
    const Window CLOSING_WINDOW = FIRST_APPLICATION_WINDOW + 1;
    switcher->closeWindow(CLOSING_WINDOW);

    QCOMPARE(switcher->windowsBeingClosed.count(), 1);
    QCOMPARE(switcher->model()->buttons().count(), APPLICATION_WINDOWS - 1);

    foreach(const QSharedPointer<SwitcherButton> &button, switcher->model()->buttons()) {
        QVERIFY(disconnect(button.data(), SIGNAL(closeTimedOutForWindow(Window)),
                           switcher, SLOT(restoreButtonBeingRemoved(Window))));
    }
    switcher->restoreButtonBeingRemoved(CLOSING_WINDOW);
    QCOMPARE(switcher->windowsBeingClosed.count(), 0);
    QVERIFY(!switcher->windowsBeingClosed.contains(CLOSING_WINDOW));
    verifyModel(receiver.windowList);
}

void Ut_Switcher::testWindowsAreClosedWhenSwitcherIsDestroyed()
{
    updateWindowList();
    QList<Window> windows;
    foreach (const QSharedPointer<SwitcherButton> &button, switcher->model()->buttons()) {
        windows.append(button->xWindow());
    }

    delete switcher;
    switcher = NULL;

    QCOMPARE(x11WrapperSendEventCalls.count(), windows.count());
    for (int i = 0; i < APPLICATION_WINDOWS; i++) {
        QCOMPARE(x11WrapperSendEventCalls.at(i).event.xclient.window, windows.at(i));
    }
}

QTEST_APPLESS_MAIN(Ut_Switcher)
