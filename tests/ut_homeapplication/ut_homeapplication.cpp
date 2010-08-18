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
#include "switcher_stub.h"
#include "windowinfo_stub.h"
#include "xeventlistener.h"
#include <signal.h>

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

QList<QString> Ut_HomeApplication::validInterfaces;
QList<QString> Ut_HomeApplication::serviceInterfaces;
QList<QString> Ut_HomeApplication::asyncCallMethods;
QList< QList<QVariant> > Ut_HomeApplication::asyncCallArguments;


class MockXEventListener : public XEventListener, public StubBase
{
    static QPair<XEventListener*, XEventListener*> ListenerAddingPair;
    static QPair<XEventListener*, XEventListener*> ListenerRemovingPair;

public:
    //! Sets up a removing hook for listener objects. While \a called is handlind and event,
    //! \a removed gets unregistered for listening the events.
    static void RegisterAddingPair(XEventListener* called, XEventListener* added)
    {
        ListenerAddingPair = qMakePair(called, added);
    }

    //! Sets up a removing hook for listener objects. While \a called is handlind and event,
    //! \a removed gets unregistered for listening the events.
    static void RegisterRemovingPair(XEventListener* called, XEventListener* removed)
    {
        ListenerRemovingPair = qMakePair(called, removed);
    }

    bool handleXEvent(const XEvent &event)
    {
        if (this == ListenerAddingPair.first) {
            Ut_HomeApplication::m_subject->addXEventListener(ListenerAddingPair.second);
        }

        if (this == ListenerRemovingPair.first) {
            Ut_HomeApplication::m_subject->removeXEventListener(ListenerRemovingPair.second);
        }

        QList<ParameterBase*> params;
        params.append(new Parameter<const XEvent*>(&event));
        stubMethodEntered("handleXEvent", params);
        return stubReturnValue<bool>("handleXEvent");
    }
};
QPair<XEventListener*, XEventListener*> MockXEventListener::ListenerAddingPair;
QPair<XEventListener*, XEventListener*> MockXEventListener::ListenerRemovingPair;

HomeApplication* Ut_HomeApplication::m_subject = NULL;

void Ut_HomeApplication::initTestCase()
{
    qRegisterMetaType<Window>("Window");
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

    static char *args[] = {(char *) "./ut_homeapplication"};
    static int argc = sizeof(args) / sizeof(char *);
    m_subject = new HomeApplication(argc, args);

    gMApplicationStub->stubReset();
    gMApplicationStub->stubSetReturnValue("x11EventFilter", false);
}

void Ut_HomeApplication::cleanup()
{
    delete m_subject;
    m_subject = NULL;
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
    m_subject = new HomeApplication(argc, args);
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
    m_subject = new HomeApplication(argc, args);
    QCoreApplication::processEvents();
    // in non-upstart mode, dbus-signal is sent and process is NOT stopped
    QCOMPARE(signalPid, 0);
    QCOMPARE(signalValue, 0);
    compareDbusValues();
}

void Ut_HomeApplication::testWhenNoEventListenerExistsXEventFilterReturnsMApplicationReturnValue()
{
    XEvent event;

    gMApplicationStub->stubSetReturnValue("x11EventFilter", false);
    QCOMPARE(m_subject->x11EventFilter(&event), false);
    QCOMPARE(gMApplicationStub->stubCallCount("x11EventFilter"), 1);

    gMApplicationStub->stubSetReturnValue("x11EventFilter", true);
    QCOMPARE(m_subject->x11EventFilter(&event), true);
    QCOMPARE(gMApplicationStub->stubCallCount("x11EventFilter"), 2);
}

void Ut_HomeApplication::testWhenEventFilterReturnsFalseXEventFilterReturnsMApplicationReturnValue()
{
    MockXEventListener mockXEventListener;
    mockXEventListener.stubSetReturnValue("handleXEvent", false);
    m_subject->addXEventListener(&mockXEventListener);

    XEvent event;
    QCOMPARE(m_subject->x11EventFilter(&event), false);
    QCOMPARE(mockXEventListener.stubCallCount("handleXEvent"), 1);
    QCOMPARE(mockXEventListener.stubLastCallTo("handleXEvent").parameter<const XEvent*>(0), &event);
    QCOMPARE(gMApplicationStub->stubCallCount("x11EventFilter"), 1);

    gMApplicationStub->stubSetReturnValue("x11EventFilter", true);
    QCOMPARE(m_subject->x11EventFilter(&event), true);
    QCOMPARE(mockXEventListener.stubCallCount("handleXEvent"), 2);
    QCOMPARE(mockXEventListener.stubLastCallTo("handleXEvent").parameter<const XEvent*>(0), &event);
    QCOMPARE(gMApplicationStub->stubCallCount("x11EventFilter"), 2);

    m_subject->removeXEventListener(&mockXEventListener);
}

void Ut_HomeApplication::testXEventFilterReturnsTrueWhenEventFilterReturnsTrue()
{
    MockXEventListener mockXEventListener;
    mockXEventListener.stubSetReturnValue("handleXEvent", true);
    m_subject->addXEventListener(&mockXEventListener);

    XEvent event;
    QCOMPARE(m_subject->x11EventFilter(&event), true);
    QCOMPARE(mockXEventListener.stubCallCount("handleXEvent"), 1);
    QCOMPARE(mockXEventListener.stubLastCallTo("handleXEvent").parameter<const XEvent*>(0), &event);

    m_subject->removeXEventListener(&mockXEventListener);
}

void Ut_HomeApplication::testXEventFilterReturnsTrueWhenThereAreTwoEventFiltersAndOnlyTheOtherReturnsTrue()
{
    MockXEventListener mockXEventListener1;
    mockXEventListener1.stubSetReturnValue("handleXEvent", true);
    m_subject->addXEventListener(&mockXEventListener1);

    MockXEventListener mockXEventListener2;
    mockXEventListener2.stubSetReturnValue("handleXEvent", false);
    m_subject->addXEventListener(&mockXEventListener2);

    XEvent event;
    QCOMPARE(m_subject->x11EventFilter(&event), true);
    QCOMPARE(mockXEventListener1.stubCallCount("handleXEvent"), 1);
    QCOMPARE(mockXEventListener1.stubLastCallTo("handleXEvent").parameter<const XEvent*>(0), &event);
    QCOMPARE(mockXEventListener2.stubCallCount("handleXEvent"), 1);
    QCOMPARE(mockXEventListener2.stubLastCallTo("handleXEvent").parameter<const XEvent*>(0), &event);

    m_subject->removeXEventListener(&mockXEventListener1);
    m_subject->removeXEventListener(&mockXEventListener2);
}

void Ut_HomeApplication::testSameXEventFilterCanBeAddedOnlyOnce()
{
    MockXEventListener mockXEventListener;
    m_subject->addXEventListener(&mockXEventListener);
    m_subject->addXEventListener(&mockXEventListener);

    XEvent event;
    m_subject->x11EventFilter(&event);
    QCOMPARE(mockXEventListener.stubCallCount("handleXEvent"), 1);
}

void Ut_HomeApplication::testAddingNewEventFilterDuringEventProcessing()
{
    MockXEventListener mockXEventListener1;
    m_subject->addXEventListener(&mockXEventListener1);
    MockXEventListener mockXEventListener2;

    // While calling the first filter, add a second one
    MockXEventListener::RegisterAddingPair(&mockXEventListener1, &mockXEventListener2);

    XEvent event;
    m_subject->x11EventFilter(&event);

    QCOMPARE(mockXEventListener1.stubCallCount("handleXEvent"), 1);
    QCOMPARE(mockXEventListener2.stubCallCount("handleXEvent"), 0);

    m_subject->x11EventFilter(&event);

    // On the second event handling round both of the listeners need to be called
    QCOMPARE(mockXEventListener1.stubCallCount("handleXEvent"), 2);
    QCOMPARE(mockXEventListener2.stubCallCount("handleXEvent"), 1);
}

void Ut_HomeApplication::testRemovingOfAlreadyCalledEventFilterDuringEventProcessing()
{
    MockXEventListener mockXEventListener1;
    m_subject->addXEventListener(&mockXEventListener1);
    MockXEventListener mockXEventListener2;
    m_subject->addXEventListener(&mockXEventListener2);
    MockXEventListener mockXEventListener3;
    m_subject->addXEventListener(&mockXEventListener3);

    // While calling the second filter, remove the first one
    MockXEventListener::RegisterRemovingPair(&mockXEventListener2, &mockXEventListener1);

    XEvent event;
    m_subject->x11EventFilter(&event);

    QCOMPARE(mockXEventListener1.stubCallCount("handleXEvent"), 1);
    QCOMPARE(mockXEventListener2.stubCallCount("handleXEvent"), 1);
    QCOMPARE(mockXEventListener3.stubCallCount("handleXEvent"), 1);
}

void Ut_HomeApplication::testRemovingOfTheEventFilterThatIsCurrentlyProcessingAnEvent()
{
    MockXEventListener mockXEventListener1;
    m_subject->addXEventListener(&mockXEventListener1);
    MockXEventListener mockXEventListener2;
    m_subject->addXEventListener(&mockXEventListener2);

    // While calling the first filter, remove that one
    MockXEventListener::RegisterRemovingPair(&mockXEventListener1, &mockXEventListener1);

    XEvent event;
    m_subject->x11EventFilter(&event);

    QCOMPARE(mockXEventListener1.stubCallCount("handleXEvent"), 1);
    QCOMPARE(mockXEventListener2.stubCallCount("handleXEvent"), 1);
}

void Ut_HomeApplication::testRemovingOfAnEventFilterThatHasNotYetProcessedAnEvent()
{
    MockXEventListener mockXEventListener1;
    m_subject->addXEventListener(&mockXEventListener1);
    MockXEventListener mockXEventListener2;
    m_subject->addXEventListener(&mockXEventListener2);

    // While calling the first filter, remove the second one
    MockXEventListener::RegisterRemovingPair(&mockXEventListener1, &mockXEventListener2);

    XEvent event;
    m_subject->x11EventFilter(&event);

    QCOMPARE(mockXEventListener1.stubCallCount("handleXEvent"), 1);
    QCOMPARE(mockXEventListener2.stubCallCount("handleXEvent"), 0);
}

QTEST_APPLESS_MAIN(Ut_HomeApplication)
