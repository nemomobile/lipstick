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

#include "ut_homeapplication.h"
#include <QtGui>
#include <QDBusPendingReply>
#include <QDBusAbstractInterface>
#include <QDBusMessage>
#include <QDBusConnection>
#include <MScene>
#include <MSceneManager>
#include <MLocale>
#include "homeapplication.h"
#include "mainwindow_stub.h"
#include "mwindow_stub.h"
#include "mapplication_stub.h"
#include <signal.h>

#include "x11wrapper.h"

#define ATOM_TYPE 0x00010000
#define ATOM_TYPE_NORMAL 0x00010001
#define ATOM_CLIENT_LIST 0x00020000
#define ATOM_CLIENT_LIST_STACKING 0x00030000
#define ATOM_CLOSE_WINDOW 0x00040000

#define NET_WM_STATE 0x00060001
#define ATOM_NET_WM_NAME 0x00070001
#define ATOM_WM_NAME 0x00080001

#define INVALID_WINDOWS 16
#define APPLICATION_WINDOWS 2
#define NON_APPLICATION_WINDOWS 5
#define VALID_WINDOWS (NON_APPLICATION_WINDOWS + APPLICATION_WINDOWS)

#define NUMBER_OF_WINDOWS (INVALID_WINDOWS + VALID_WINDOWS)

#define ATOM_TYPE_DESKTOP 100
#define ATOM_TYPE_NOTIFICATION 101
#define ATOM_TYPE_DIALOG 102
#define ATOM_TYPE_DOCK 103
#define ATOM_TYPE_MENU 104
#define ATOM_STATE_SKIP_TASKBAR 105

#define ATOM_TYPE_INVALID 666

WId QWidget::winId() const
{
    return 0xbadf00d;
}

static QMap<Window, QVector<Atom> > g_windowTypeMap;
static QMap<Window, QVector<Atom> > g_windowStateMap;
static bool g_windowName;

// QCoreApplication stubs to avoid crashing in processEvents()
QStringList QCoreApplication::arguments()
{
    return QStringList();
}

void MLocale::setDefault(const MLocale &)
{
}

// X11 Stubs
Atom X11Wrapper::XInternAtom(Display *, const char *atom_name, Bool)
{
    if (strcmp(atom_name, "_NET_WM_WINDOW_TYPE") == 0) {
        return ATOM_TYPE;
    } else if (strcmp(atom_name, "_NET_CLIENT_LIST_STACKING") == 0) {
        return ATOM_CLIENT_LIST_STACKING;
    } else if (strcmp(atom_name, "_NET_CLOSE_WINDOW") == 0) {
        return ATOM_CLOSE_WINDOW;
    } else if (strcmp(atom_name, "_NET_WM_STATE") == 0) {
        return NET_WM_STATE;
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
        Ut_HomeApplication::visibilityNotifyWindows.append(window);
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
            *nitems_return = Ut_HomeApplication::clientListNumberOfWindows;
            *prop_return = new unsigned char[Ut_HomeApplication::clientListNumberOfWindows * sizeof(Window)];

            Window *windows = (Window *) * prop_return;
            for (int i = 0; i < Ut_HomeApplication::clientListNumberOfWindows; i++)
                windows[i] = i;
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
    } else if (property == NET_WM_STATE) {
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

Status X11Wrapper::XGetWMName(Display *, Window , XTextProperty* )
{
    return g_windowName ? 1 : 0;
}

XWMHints *X11Wrapper::XGetWMHints(Display *, Window)
{
    return NULL;
}

Status X11Wrapper::XGetTextProperty(Display *, Window, XTextProperty *text_prop_return, Atom)
{
    if (g_windowName) {
        QString textValue("title");
        std::string::size_type strSize = textValue.toStdString().length();
        text_prop_return->value = new unsigned char[strSize + 1];
        strncpy((char *)text_prop_return->value, textValue.toStdString().c_str(), strSize + 1);
        return 1;
    } else {
        return 0;
    }
}

void XSetWMProperties(Display *, Window, XTextProperty *, XTextProperty *, char **, int, XSizeHints *, XWMHints *, XClassHint *)
{
}

// QDBusAbstractInterface stubs (used by HomeApplication through service framework)
QDBusPendingCall QDBusAbstractInterface::asyncCallWithArgumentList(const QString &method, const QList<QVariant> & args)
{
    Ut_HomeApplication::asyncCallMethods.append(method);
    Ut_HomeApplication::asyncCallArguments.append(args);

    QDBusPendingReply<bool> reply;
    return reply;
}

// QDBusPendingReplyData stubs (used by StatusArea through service framework)
QVariant QDBusPendingReplyData::argumentAt(int index) const
{
    Q_UNUSED(index);
    return QVariant();
}

void QDBusPendingReplyData::setMetaTypes(int count, const int *metaTypes)
{
    Q_UNUSED(count);
    Q_UNUSED(metaTypes);
}

// stubs needed for the d-bus signal and process stopping

static pid_t signalPid = 0;
static int signalValue = 0;
extern "C" int kill(pid_t pid, int sig)
{
    signalPid = pid;
    signalValue = sig;
    return 0;
}

static const QString HOME_READY_SIGNAL_PATH = "/com/nokia/duihome";
static const QString HOME_READY_SIGNAL_INTERFACE =
    "com.nokia.duihome.readyNotifier";
static const QString HOME_READY_SIGNAL_NAME = "ready";

static QString dbusMessagePath;
static QString dbusMessageInterface;
static QDBusMessage::MessageType dbusMessageType = QDBusMessage::InvalidMessage;
static QString dbusMessageMember;

bool QDBusConnection::send(const QDBusMessage &message) const
{
    dbusMessagePath = message.path();
    dbusMessageInterface = message.interface();
    dbusMessageType = message.type();
    dbusMessageMember = message.member();
    return true;
}

// Test class
class TestHomeApplication : public HomeApplication
{
public:
    TestHomeApplication(int &argc, char **argv);

    virtual ~TestHomeApplication();

    bool testX11EventFilter(XEvent *event);

};

TestHomeApplication::TestHomeApplication(int &argc, char **argv) :
    HomeApplication(argc, argv)
{
}

TestHomeApplication::~TestHomeApplication()
{
}

bool TestHomeApplication::testX11EventFilter(XEvent *event)
{
    return HomeApplication::x11EventFilter(event);
}

QList<Window> Ut_HomeApplication::visibilityNotifyWindows;
QList<QString> Ut_HomeApplication::validInterfaces;
QList<QString> Ut_HomeApplication::serviceInterfaces;
QList<QString> Ut_HomeApplication::asyncCallMethods;
QList< QList<QVariant> > Ut_HomeApplication::asyncCallArguments;
int Ut_HomeApplication::clientListNumberOfWindows;

void Ut_HomeApplication::initTestCase()
{
    qRegisterMetaType<Window>("Window");
    gMApplicationStub->stubSetReturnValue("x11EventFilter", false);
}

void Ut_HomeApplication::cleanupTestCase()
{
}


void Ut_HomeApplication::init()
{
    validInterfaces.clear();
    serviceInterfaces.clear();
    asyncCallMethods.clear();
    asyncCallArguments.clear();
    clientListNumberOfWindows = NUMBER_OF_WINDOWS;

    static char *args[] = {(char *) "./ut_homeapplication"};
    static int argc = sizeof(args) / sizeof(char *);
    m_subject = new TestHomeApplication(argc, args);
    visibilityNotifyWindows.clear();

    g_windowName = true;
    g_windowTypeMap.clear();
    for (int w = 0; w < clientListNumberOfWindows; w++) {
        QVector<Atom> types(1);
        types[0] = ATOM_TYPE_NORMAL;
        g_windowTypeMap[w] = types;
    }
    // Add non-application windows
    g_windowTypeMap[INVALID_WINDOWS + APPLICATION_WINDOWS + 0][0] = ATOM_TYPE_DESKTOP;
    g_windowTypeMap[INVALID_WINDOWS + APPLICATION_WINDOWS + 1][0] = ATOM_TYPE_NOTIFICATION;
    g_windowTypeMap[INVALID_WINDOWS + APPLICATION_WINDOWS + 2][0] = ATOM_TYPE_DIALOG;
    g_windowTypeMap[INVALID_WINDOWS + APPLICATION_WINDOWS + 3][0] = ATOM_TYPE_DOCK;
    g_windowTypeMap[INVALID_WINDOWS + APPLICATION_WINDOWS + 4][0] = ATOM_TYPE_MENU;

    g_windowStateMap.clear();
}

void Ut_HomeApplication::cleanup()
{
    delete m_subject;
}

static void resetDbusAndSignalExpectedValues()
{
    signalPid = 0;
    signalValue = 0;
    dbusMessagePath = "";
    dbusMessageInterface = "";
    dbusMessageType = QDBusMessage::InvalidMessage;
    dbusMessageMember = "";
}

static void compareDbusValues()
{
    QCOMPARE(dbusMessagePath, HOME_READY_SIGNAL_PATH);
    QCOMPARE(dbusMessageInterface, HOME_READY_SIGNAL_INTERFACE);
    QCOMPARE(dbusMessageType, QDBusMessage::SignalMessage);
    QCOMPARE(dbusMessageMember, HOME_READY_SIGNAL_NAME);
}


void Ut_HomeApplication::testUpstartStartup()
{
    resetDbusAndSignalExpectedValues();
    delete m_subject;
    char *args[] = {(char *) "./ut_homeapplication",
                    (char *) "--upstart"
                   };
    int argc = sizeof(args) / sizeof(char *);
    m_subject = new TestHomeApplication(argc, args);
    QCoreApplication::processEvents();
    // in upstart mode, dbus-signal is sent and process is stopped
    QCOMPARE(signalPid, getpid());
    QCOMPARE(signalValue, SIGSTOP);
    compareDbusValues();
}

void Ut_HomeApplication::testNonUpstartStartup()
{
    resetDbusAndSignalExpectedValues();
    delete m_subject;
    // check that any option containing 'u' is not interpreted
    // as the upstart param. Practical example is -output-level.
    char *args[] = {(char *) "./ut_homeapplication",
                    (char *) "-output-level debug"
                   };
    int argc = sizeof(args) / sizeof(char *);
    m_subject = new TestHomeApplication(argc, args);
    QCoreApplication::processEvents();
    // in non-upstart mode, dbus-signal is sent and process is NOT stopped
    QCOMPARE(signalPid, 0);
    QCOMPARE(signalValue, 0);
    compareDbusValues();
}

void Ut_HomeApplication::testX11EventFilterWithWmStateChange()
{
    WindowListReceiver r;
    connect(m_subject, SIGNAL(windowListUpdated(const QList<WindowInfo> &)), &r, SLOT(windowListUpdated(const QList<WindowInfo> &)));

    XEvent event;
    event.xproperty.atom = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_CLIENT_LIST_STACKING", False);
    event.type = PropertyNotify;
    event.xproperty.window = DefaultRootWindow(QX11Info::display());

    QVERIFY(m_subject->testX11EventFilter(&event));

    // Make sure the window list change signal was emitted
    QCOMPARE(r.count, 1);

    QCOMPARE(r.windowList.count(), APPLICATION_WINDOWS);
}

void Ut_HomeApplication::testX11EventFilterWithPropertyNotify()
{
    WindowListReceiver r;
    connect(m_subject, SIGNAL(windowListUpdated(const QList<WindowInfo> &)), &r, SLOT(windowListUpdated(const QList<WindowInfo> &)));

    // Verify that X11EventFilter only reacts to events it's supposed to react on (type, window and atom are all set correctly)
    XEvent event;
    event.type = 0;
    event.xproperty.window = 0;
    event.xproperty.atom = 0;
    QVERIFY(!m_subject->testX11EventFilter(&event));
    event.type = PropertyNotify;
    QVERIFY(!m_subject->testX11EventFilter(&event));
    event.xproperty.window = DefaultRootWindow(QX11Info::display());
    QVERIFY(!m_subject->testX11EventFilter(&event));
    event.type = 0;
    QVERIFY(!m_subject->testX11EventFilter(&event));

    event.xproperty.atom = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_CLIENT_LIST_STACKING", False);
    QVERIFY(!m_subject->testX11EventFilter(&event));

    event.xproperty.window = 0;
    QVERIFY(!m_subject->testX11EventFilter(&event));
    event.type = PropertyNotify;
    QVERIFY(!m_subject->testX11EventFilter(&event));

    event.xproperty.window = DefaultRootWindow(QX11Info::display());
    QVERIFY(m_subject->testX11EventFilter(&event));

    // Make sure the window list change signal was emitted
    QCOMPARE(r.count, 1);
    QCOMPARE(r.windowList.count(), APPLICATION_WINDOWS);

    // Update the state of a window that currently is an application window
    QVector<Atom> states(1);
    states[0] = ATOM_STATE_SKIP_TASKBAR;
    g_windowStateMap[INVALID_WINDOWS] = states;
    event.xproperty.window = INVALID_WINDOWS;
    event.xproperty.atom = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_WM_STATE", False);
    QVERIFY(m_subject->testX11EventFilter(&event));
    QCOMPARE(r.count, 2);
    QCOMPARE(r.windowList.count(), APPLICATION_WINDOWS - 1);

    // Update the type of a window that currently is an application window
    g_windowTypeMap[INVALID_WINDOWS + 1][0] = ATOM_TYPE_MENU;
    event.xproperty.window = INVALID_WINDOWS + 1;
    event.xproperty.atom = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_WM_WINDOW_TYPE", False);
    QVERIFY(m_subject->testX11EventFilter(&event));
    QCOMPARE(r.count, 3);
    QCOMPARE(r.windowList.count(), 0);
}

void Ut_HomeApplication::testX11EventFilterWithVisibilityNotify()
{
    MainWindow *w = MainWindow::instance(true);
    QList<MWindow *> windowList;
    windowList.append(w);
    gMApplicationStub->stubSetReturnValue<QList<MWindow *> >("windows", windowList);

    int x11EventFilterCallCount;
    WindowVisibilityReceiver r;
    connect(m_subject, SIGNAL(windowVisibilityChanged(Window)), &r, SLOT(windowVisibilityChanged(Window)));

    // Verify that X11EventFilter only reacts to events it's supposed to react on
    XEvent event;
    event.type = VisibilityNotify;
    event.xvisibility.window = 303;
    event.xvisibility.state = VisibilityUnobscured;
    event.xvisibility.send_event = TRUE;

    // Make sure the window visibility change signal is not emitted if state is VisibilityUnobscured
    x11EventFilterCallCount = gMApplicationStub->stubCallCount("x11EventFilter");
    QVERIFY(!m_subject->testX11EventFilter(&event));
    QCOMPARE(r.windowList.count(), 0);
    QCOMPARE(gMApplicationStub->stubCallCount("x11EventFilter"), x11EventFilterCallCount + 1);

    // Make sure the window visibility change signal was emitted with the given window if state is VisibilityFullyObscured
    x11EventFilterCallCount = gMApplicationStub->stubCallCount("x11EventFilter");
    event.xvisibility.state = VisibilityFullyObscured;
    QVERIFY(m_subject->testX11EventFilter(&event));
    QCOMPARE(r.windowList.count(), 1);
    QCOMPARE(r.windowList.at(0), (Window)303);
    QCOMPARE(gMApplicationStub->stubCallCount("x11EventFilter"), x11EventFilterCallCount);

    // Make sure the window visibility change signal is not emitted if send_event is FALSE but the event is processed
    event.xvisibility.send_event = FALSE;
    QVERIFY(m_subject->testX11EventFilter(&event));
    QCOMPARE(r.windowList.count(), 1);
    QCOMPARE(gMApplicationStub->stubCallCount("x11EventFilter"), x11EventFilterCallCount);

    // Make sure the window visibility change signal is not emitted if the window is the homescreen itself
    event.xvisibility.window = w->winId();
    QVERIFY(!m_subject->testX11EventFilter(&event));
    QCOMPARE(r.windowList.count(), 1);
    QCOMPARE(gMApplicationStub->stubCallCount("x11EventFilter"), x11EventFilterCallCount + 1);
}

void Ut_HomeApplication::testX11EventFilterWithClientMessage()
{
    WindowListReceiver r;
    connect(m_subject, SIGNAL(windowListUpdated(const QList<WindowInfo> &)), &r, SLOT(windowListUpdated(const QList<WindowInfo> &)));

    // Verify that X11EventFilter only reacts to events it's supposed to react on (type and message_type are both set correctly)
    XEvent clientEvent;
    clientEvent.type = 0;
    clientEvent.xclient.message_type = 0;
    QVERIFY(!m_subject->testX11EventFilter(&clientEvent));
    clientEvent.type = ClientMessage;
    QVERIFY(!m_subject->testX11EventFilter(&clientEvent));
    clientEvent.type = 0;
    clientEvent.xclient.message_type = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_CLOSE_WINDOW", False);
    QVERIFY(!m_subject->testX11EventFilter(&clientEvent));
    clientEvent.type = ClientMessage;
    clientEvent.xclient.window = INVALID_WINDOWS + 1; // This is the first application window id
    QVERIFY(m_subject->testX11EventFilter(&clientEvent));

    // Make sure the window list change signal was emitted
    QCOMPARE(r.count, 1);
    // We have closed one window
    QCOMPARE(r.windowList.count(), APPLICATION_WINDOWS - 1);

    // Change the client list so that the window being closed was actually closed
    XEvent propertyEvent;
    propertyEvent.type = PropertyNotify;
    propertyEvent.xproperty.window = DefaultRootWindow(QX11Info::display());
    propertyEvent.xproperty.atom = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_CLIENT_LIST_STACKING", False);
    QVERIFY(m_subject->testX11EventFilter(&propertyEvent));

    // Make sure the window list change signal was not emitted
    QCOMPARE(r.count, 1);
    
    // There should be 2 windows in the window list
    QCOMPARE(r.windowList.count(), APPLICATION_WINDOWS - 1);

    
}

/*
void Ut_HomeApplication::testContentSearchLaunch()
{
    // Add interface so that isValid() returns true
    validInterfaces.append(QString("com.nokia.ContentSearchIf"));

    // Launch the content search service
    m_subject->launchContentSearchService();

    // Verify that content search is launched
    QCOMPARE(serviceInterfaces.count(), 1);
    QCOMPARE(serviceInterfaces.at(0), QString("com.nokia.ContentSearchIf"));
    QCOMPARE(asyncCallMethods.at(0), QString("launch"));
    QCOMPARE(asyncCallArguments.at(0).count(), 1);
    QCOMPARE(asyncCallArguments.at(0).at(0).type(), QVariant::String);
    QCOMPARE(asyncCallArguments.at(0).at(0).toString(), QString(""));
}

void Ut_HomeApplication::testContentSearchLaunchWithoutServiceFW()
{
    // Click the search button
    m_subject->launchContentSearchService();

    // Verify that home application doesn't try to launch the service
    QCOMPARE(serviceInterfaces.count(), 1);
    QCOMPARE(serviceInterfaces.at(0), QString("com.nokia.ContentSearchIf"));
    QCOMPARE(asyncCallMethods.count(), 0);
}
*/



void Ut_HomeApplication::testX11EventWindowNameChange_data()
{
    QTest::addColumn<Window>("window");
    QTest::addColumn<QString>("property");

    QTest::newRow("_NET_WM_NAME") << (Window) INVALID_WINDOWS + 1 << "_NET_WM_NAME";
}

void Ut_HomeApplication::testX11EventWindowNameChange()
{
    //    gWindowInfoStub->stubSetReturnValue("updateWindowTitle", true);
    g_windowName = true;
    QFETCH(Window, window);
    QFETCH(QString, property);

    QSignalSpy spy(m_subject, SIGNAL(windowTitleChanged(Window, QString)));

    XEvent event;
    event.xproperty.atom = X11Wrapper::XInternAtom(QX11Info::display(), property.toUtf8(), False);
    event.type = PropertyNotify;
    event.xproperty.window = window;
    QVERIFY(m_subject->testX11EventFilter(&event));

    QCOMPARE(spy.count(), 1);
    g_windowName = false;
    //    gWindowInfoStub->stubSetReturnValue("updateWindowTitle", false);

    // Check that the signal is not emitted when the window info can not get the title
    spy.clear();
    QVERIFY(m_subject->testX11EventFilter(&event));
    QCOMPARE(spy.count(), 0);
}

QTEST_APPLESS_MAIN(Ut_HomeApplication)
