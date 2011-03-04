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

#include <QKeyEvent>
#include <QCloseEvent>
#include "ut_mainwindow.h"
#include "x11wrapper_stub.h"
#include "mainwindow.h"
#include "mainwindowstyle.h"
#include "homeapplication_stub.h"
#include <MSceneWindow>
#include <QDBusInterface>

// Stub the Dbus call used by status bar to open a menu
bool dBusCallMade = false;
QString dBusService = QString();
QString dBusPath = QString();
QString dBusInterface = QString();
QString dBusMethod = QString();
QList<QVariant> dBusArguments = QList<QVariant>();
QDBus::CallMode dBusCallMode = QDBus::NoBlock;
bool QDBusAbstractInterface::callWithCallback(const QString &method, const QList<QVariant> &args, QObject *, const char *, const char *)
{
    dBusCallMade = true;
    dBusInterface = this->interface();
    dBusService = this->service();
    dBusPath = this->path();
    dBusMethod = method;
    dBusArguments = args;
    return true;
}

QDBusMessage QDBusAbstractInterface::call(QDBus::CallMode mode,
                  const QString &method,
                  const QVariant &arg1,
                  const QVariant &arg2,
                  const QVariant &arg3,
                  const QVariant &arg4,
                  const QVariant &arg5,
                  const QVariant &arg6,
                  const QVariant &arg7,
                  const QVariant &arg8)
{
    dBusCallMade = true;
    dBusCallMode = mode;
    dBusInterface = this->interface();
    dBusService = this->service();
    dBusPath = this->path();
    dBusMethod = method;
    dBusArguments << arg1 << arg2 << arg3 << arg4 << arg5 << arg6 << arg7 << arg8;
    QDBusMessage message;
    return message;
}

void resetDBusStub()
{
    dBusCallMade = false;
    dBusService = QString();
    dBusPath = QString();
    dBusInterface = QString();
    dBusMethod = QString();
    dBusArguments = QList<QVariant>();
}

// Home stubs
class Home : public MSceneWindow
{
public:
    Home(QGraphicsItem *parent = 0);
};

Home::Home(QGraphicsItem *parent) : MSceneWindow(parent)
{
}

// MWindow stubs
bool mWindowOrientationLocked = false;
void MWindow::setOrientationLocked(bool locked)
{
    mWindowOrientationLocked = locked;
}

bool mWindowOrientationAngleLocked = false;
void MWindow::setOrientationAngleLocked(bool locked)
{
    mWindowOrientationAngleLocked = locked;
}

QString mWindowOrientation;
void MWindow::setLandscapeOrientation()
{
    mWindowOrientation = "landscape";
}

void MWindow::setPortraitOrientation()
{
    mWindowOrientation = "portrait";
}

M::OrientationAngle mWindowOrientationAngle;
void MWindow::setOrientationAngle(M::OrientationAngle angle)
{
    mWindowOrientationAngle = angle;
}

void Ut_MainWindow::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_mainwindow";
    app = new HomeApplication(argc, &app_name);

    mainWindow = MainWindow::instance(true);

    // Check that receiving of VisibilityChange events was requested
    QCOMPARE(true, (bool)(gX11WrapperStub->stubLastCallTo("XSelectInput").parameter<long>(2) & VisibilityChangeMask));

    Atom netWmStateAtom = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_WM_STATE", FALSE);
    Atom skipTaskbarAtom = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_WM_STATE_SKIP_TASKBAR", False);
    XEvent event = gX11WrapperStub->stubLastCallTo("XSendEvent").parameter<XEvent>(4);

    QCOMPARE(event.xclient.message_type, (unsigned long)netWmStateAtom);
    QCOMPARE(event.xclient.format, (int)32);
    QCOMPARE(event.xclient.data.l[0], (long)1);
    QCOMPARE(event.xclient.data.l[1], (long)skipTaskbarAtom);
    QCOMPARE(event.xclient.data.l[2], (long)0);
    QCOMPARE(event.xclient.data.l[3], (long)0);
    QCOMPARE(event.xclient.data.l[4], (long)0);
}

void Ut_MainWindow::cleanupTestCase()
{
    delete mainWindow;
    delete app;
}

void Ut_MainWindow::init()
{
    mainWindow->keyPressesBeingSent.clear();
    mainWindow->keyPressesToBeSent.clear();
    resetDBusStub();
}

void Ut_MainWindow::cleanup()
{
}

#undef KeyPress
void addIntRangeToSet(QSet<int> &set, int from, int to)
{
    for (int i = from; i <= to; i++) {
        set.insert(i);
    }
}

void Ut_MainWindow::testCallUILaunchedWhenCallRelatedKeyPressed()
{
    // Add a relevant set of keys to the map
    QSet<int> keys;
    addIntRangeToSet(keys, Qt::Key_Space, Qt::Key_QuoteDbl);
    addIntRangeToSet(keys, Qt::Key_Dollar, Qt::Key_ParenRight);
    addIntRangeToSet(keys, Qt::Key_Comma, Qt::Key_Slash);
    addIntRangeToSet(keys, Qt::Key_Colon, Qt::Key_At);
    addIntRangeToSet(keys, Qt::Key_BracketLeft, Qt::Key_ydiaeresis);
    addIntRangeToSet(keys, Qt::Key_Escape, Qt::Key_MediaLast);
    addIntRangeToSet(keys, Qt::Key_Context1, Qt::Key_Flip);

    foreach (int key, keys.values()) {
        // Create a key event for the key. The test uses the string " " as the text produced for most keys.
        QKeyEvent keyEvent(QEvent::KeyPress, key, Qt::NoModifier, key < Qt::Key_Context1 ? " " : "");
        mainWindow->keyPressEvent(&keyEvent);

        QString searchString = keyEvent.text();
        if (!searchString.isEmpty() && key < Qt::Key_Escape && ((key >= Qt::Key_0 && key <= Qt::Key_9) || key == Qt::Key_Asterisk || key == Qt::Key_Plus || key == Qt::Key_NumberSign)) {
            // The call UI should only be launched if the key is a call related one (0-9, +, * and #)
            QCOMPARE(dBusCallMade, true);
            QCOMPARE(dBusService, MainWindow::CALL_UI_DBUS_SERVICE);
            QCOMPARE(dBusPath, MainWindow::CALL_UI_DBUS_PATH);
            QCOMPARE(dBusInterface, MainWindow::CALL_UI_DBUS_INTERFACE);
            QCOMPARE(dBusMethod, QString("dialer"));
            QCOMPARE(dBusArguments.at(0).toString(), searchString);
            mainWindow->markKeyPressesSentAndSendRemainingKeyPresses();
        } else {
            // Otherwise a call may or may not occur but it should not be to the call UI
            QVERIFY(dBusService != MainWindow::CALL_UI_DBUS_SERVICE);
            QVERIFY(dBusPath != MainWindow::CALL_UI_DBUS_PATH);
            QVERIFY(dBusInterface != MainWindow::CALL_UI_DBUS_INTERFACE);
        }

        resetDBusStub();
    }
}

void Ut_MainWindow::testContentSearchLaunchedWhenNonCallRelatedKeyPressed()
{
    // Add a relevant set of keys to the map
    QSet<int> keys;
    addIntRangeToSet(keys, Qt::Key_Space, Qt::Key_QuoteDbl);
    addIntRangeToSet(keys, Qt::Key_Dollar, Qt::Key_ParenRight);
    addIntRangeToSet(keys, Qt::Key_Comma, Qt::Key_Slash);
    addIntRangeToSet(keys, Qt::Key_Colon, Qt::Key_At);
    addIntRangeToSet(keys, Qt::Key_BracketLeft, Qt::Key_ydiaeresis);
    addIntRangeToSet(keys, Qt::Key_Escape, Qt::Key_MediaLast);
    addIntRangeToSet(keys, Qt::Key_Context1, Qt::Key_Flip);

    foreach (int key, keys.values()) {
        // Create a key event for the key. The test uses the string " " as the text produced for most keys.
        QKeyEvent keyEvent(QEvent::KeyPress, key, Qt::NoModifier, key < Qt::Key_Context1 ? " " : "");
        mainWindow->keyPressEvent(&keyEvent);

        QString searchString = keyEvent.text();
        if (!searchString.isEmpty() && key < Qt::Key_Escape && !((key >= Qt::Key_0 && key <= Qt::Key_9) || key == Qt::Key_Asterisk || key == Qt::Key_Plus || key == Qt::Key_NumberSign)) {
            // The search should only occur if the key is not a call related one (0-9, +, * and #)
            QCOMPARE(dBusCallMade, true);
            QCOMPARE(dBusService, MainWindow::CONTENT_SEARCH_DBUS_SERVICE);
            QCOMPARE(dBusPath, MainWindow::CONTENT_SEARCH_DBUS_PATH);
            QCOMPARE(dBusInterface, MainWindow::CONTENT_SEARCH_DBUS_INTERFACE);
            QCOMPARE(dBusMethod, QString("launch"));
            QCOMPARE(dBusArguments.at(0).toString(), searchString);
            mainWindow->markKeyPressesSentAndSendRemainingKeyPresses();
        } else {
            // Otherwise a call may or may not occur but it should not be to the content search service
            QVERIFY(dBusService != MainWindow::CONTENT_SEARCH_DBUS_SERVICE);
            QVERIFY(dBusPath != MainWindow::CONTENT_SEARCH_DBUS_PATH);
            QVERIFY(dBusInterface != MainWindow::CONTENT_SEARCH_DBUS_INTERFACE);
        }

        resetDBusStub();
    }
}

void Ut_MainWindow::testLaunchQueuedWhenAlreadyLaunching()
{
    // Pressing the key A should try to launch the content search with the search string A
    QKeyEvent keyEventA(QEvent::KeyPress, Qt::Key_A, Qt::NoModifier, "A");
    mainWindow->keyPressEvent(&keyEventA);
    QCOMPARE(dBusCallMade, true);
    QCOMPARE(dBusArguments.at(0).toString(), QString("A"));

    resetDBusStub();

    // Pressing the keys B and C before the content search has launched should not launch another content search
    QKeyEvent keyEventB(QEvent::KeyPress, Qt::Key_B, Qt::NoModifier, "B");
    mainWindow->keyPressEvent(&keyEventB);
    QKeyEvent keyEventC(QEvent::KeyPress, Qt::Key_B, Qt::NoModifier, "C");
    mainWindow->keyPressEvent(&keyEventC);
    QCOMPARE(dBusCallMade, false);

    // When the content search has successfully launched (with the search string A) the content search should be relaunched with the search string BC
    mainWindow->markKeyPressesSentAndSendRemainingKeyPresses();
    QCOMPARE(dBusCallMade, true);
    QCOMPARE(dBusArguments.at(0).toString(), QString("BC"));
}

void Ut_MainWindow::testLaunchQueuedWhenLaunchingFailed()
{
    // Pressing the key A should try to launch the content search with the search string A
    QKeyEvent keyEventA(QEvent::KeyPress, Qt::Key_A, Qt::NoModifier, "A");
    mainWindow->keyPressEvent(&keyEventA);

    resetDBusStub();

    // Pressing the key B before the content search has launched should not launch another content search
    QKeyEvent keyEventB(QEvent::KeyPress, Qt::Key_B, Qt::NoModifier, "B");
    mainWindow->keyPressEvent(&keyEventB);

    // Pressing the key 0 before the content search has launched should not launch call UI
    QKeyEvent keyEvent0(QEvent::KeyPress, Qt::Key_0, Qt::NoModifier, "0");
    mainWindow->keyPressEvent(&keyEvent0);

    // When the content search launching fails (with the search string A) no new content search should occur
    mainWindow->markKeyPressesNotSent();
    QCOMPARE(dBusCallMade, false);

    resetDBusStub();

    // Pressing the key C should try to launch the content search with the search string AB0C
    QKeyEvent keyEventC(QEvent::KeyPress, Qt::Key_C, Qt::NoModifier, "C");
    mainWindow->keyPressEvent(&keyEventC);
    QCOMPARE(dBusCallMade, true);
    QCOMPARE(dBusArguments.at(0).toString(), QString("AB0C"));
}

void Ut_MainWindow::testNothingLaunchedWhenControlModifierPressed()
{
    // Add a relevant set of keys to the map
    QSet<int> keys;
    addIntRangeToSet(keys, Qt::Key_Space, Qt::Key_QuoteDbl);
    addIntRangeToSet(keys, Qt::Key_Dollar, Qt::Key_ParenRight);
    addIntRangeToSet(keys, Qt::Key_Comma, Qt::Key_Slash);
    addIntRangeToSet(keys, Qt::Key_Colon, Qt::Key_At);
    addIntRangeToSet(keys, Qt::Key_BracketLeft, Qt::Key_ydiaeresis);
    addIntRangeToSet(keys, Qt::Key_Escape, Qt::Key_MediaLast);
    addIntRangeToSet(keys, Qt::Key_Context1, Qt::Key_Flip);

    foreach (int key, keys.values()) {
        // Create a key event for the key with the Control on. The test uses the string " " as the text produced for most keys.
        QKeyEvent keyEvent(QEvent::KeyPress, key, Qt::ControlModifier, " ");
        mainWindow->keyPressEvent(&keyEvent);
        QCOMPARE(dBusCallMade, false);
    }
}

void Ut_MainWindow::testNothingLaunchedWhenOnlyModifierPressed()
{
    QSet<Qt::KeyboardModifier> modifiers;
    modifiers << Qt::ShiftModifier;
    modifiers << Qt::ControlModifier;
    modifiers << Qt::AltModifier;
    modifiers << Qt::MetaModifier;
    modifiers << Qt::KeypadModifier;
    modifiers << Qt::GroupSwitchModifier;

    foreach (Qt::KeyboardModifier modifier, modifiers.values()) {
        QKeyEvent keyEvent(QEvent::KeyPress, 0, modifier);
        mainWindow->keyPressEvent(&keyEvent);
        QCOMPARE(dBusCallMade, false);
        resetDBusStub();
    }
}

void Ut_MainWindow::testNothingLaunchedWhenMediaKeyPressed()
{
    QSet<int> keys;
    addIntRangeToSet(keys, Qt::Key_Escape, Qt::Key_MediaLast);
    addIntRangeToSet(keys, Qt::Key_Context1, Qt::Key_Flip);

    foreach (int key, keys.values()) {
        QKeyEvent keyEvent(QEvent::KeyPress, key, Qt::NoModifier);
        mainWindow->keyPressEvent(&keyEvent);
        QCOMPARE(dBusCallMade, false);
        resetDBusStub();
    }
}

void Ut_MainWindow::testCloseEventIsIgnored()
{
    QCloseEvent event;
    event.setAccepted(true);
    mainWindow->closeEvent(&event);
    QCOMPARE(event.isAccepted(), false);
}

void Ut_MainWindow::testOrientationLocking_data()
{
    QTest::addColumn<QVariant>("commandLineLockedOrientation");
    QTest::addColumn<QString>("styleLockedOrientation");
    QTest::addColumn<bool>("orientationLocked");
    QTest::addColumn<QString>("expectedOrientation");
    QTest::addColumn<int>("expectedOrientationAngle");

    QTest::newRow("Command line: Default / Style: No locking") << QVariant(QVariant::Invalid) << QString() << false << QString() << (int)M::Angle180;
    QTest::newRow("Command line: Default / Style: Locked to landscape") << QVariant(QVariant::Invalid) << QString("landscape") << true << QString("landscape") << (int)M::Angle0;
    QTest::newRow("Command line: Default / Style: Locked to portrait") << QVariant(QVariant::Invalid) << QString("portrait") << true << QString("portrait") << (int)M::Angle270;
    QTest::newRow("Command line: Default / Style: Locked to something else") << QVariant(QVariant::Invalid) << QString("unknown") << false << QString() << (int)M::Angle180;
    QTest::newRow("Command line: No locking / Style: Locked to portrait") << QVariant("") << QString("portrait") << false << QString() << (int)M::Angle180;
    QTest::newRow("Command line: Locked to landscape / Style: Locked to portrait") << QVariant("landscape") << QString("portrait") << true << QString("landscape") << (int)M::Angle0;
    QTest::newRow("Command line: Locked to portrait / Style: Locked to landscape") << QVariant("portrait") << QString("landscape") << true << QString("portrait") << (int)M::Angle270;
    QTest::newRow("Command line: Locked to something else / Style: Locked to landscape") << QVariant("unknown") << QString("landscape") << false << QString() << (int)M::Angle180;
}

void Ut_MainWindow::testOrientationLocking()
{
    QFETCH(QVariant, commandLineLockedOrientation);
    QFETCH(QString, styleLockedOrientation);
    QFETCH(bool, orientationLocked);
    QFETCH(QString, expectedOrientation);
    QFETCH(int, expectedOrientationAngle);

    // Reset the stubs
    mWindowOrientationLocked = false;
    mWindowOrientationAngleLocked = false;
    mWindowOrientation = QString();
    mWindowOrientationAngle = M::Angle180;
    gHomeApplicationStub->stubSetReturnValue("lockedOrientation", commandLineLockedOrientation);

    // Set the style
    MainWindowStyle *style = const_cast<MainWindowStyle *>(static_cast<const MainWindowStyle *>(MTheme::style("MainWindowStyle", "", "", "", M::Landscape, NULL)));
    style->setLockedOrientation(styleLockedOrientation);

    // Create a new window
    delete mainWindow;
    mainWindow = MainWindow::instance(true);
    QCOMPARE(mWindowOrientationLocked, orientationLocked);
    QCOMPARE(mWindowOrientationAngleLocked, orientationLocked);
    QCOMPARE(mWindowOrientation, expectedOrientation);
    QCOMPARE(mWindowOrientationAngle, (M::OrientationAngle)expectedOrientationAngle);
}

QTEST_APPLESS_MAIN(Ut_MainWindow)
