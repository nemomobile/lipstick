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

#include "ut_notificationareasink.h"
#include "notificationwidgetparameterfactory.h"

#include <QtTest/QtTest>
#include <DuiInfoBanner>
#include <DuiRemoteAction>
#include <DuiApplication>
#include "../stubs/testnotificationparameters.h"
#include "homeapplication_stub.h"
#include "notificationmanager_stub.h"
#include "eventtypestore.h"
#include <QSettings>
#include "applicationcontext.h"

static QSettings *settings;
// QCoreApplication stubs to avoid crashing in processEvents()
QStringList QCoreApplication::arguments()
{
    return QStringList();
}

// DuiRemoteAction stubs (used by NotificationAreaSink)
DuiRemoteAction::DuiRemoteAction(const QString &action, QObject *parent) : DuiAction(parent)
{
    Ut_NotificationAreaSink::contents.append(action);
}

// DuiInfoBanner stubs (used by NotificationAreaSink)
DuiInfoBanner::~DuiInfoBanner()
{
    Ut_NotificationAreaSink::destroyedNotifications.append(this);
}

void DuiInfoBanner::setImageID(const QString &imageId)
{
    int index = Ut_NotificationAreaSink::notifications.indexOf(this);
    if (index >= 0) {
        Ut_NotificationAreaSink::icons.replace(index, imageId);
    } else {
        Ut_NotificationAreaSink::icons.append(imageId);
    }
}

void DuiInfoBanner::setBodyText(const QString &body)
{
    int index = Ut_NotificationAreaSink::notifications.indexOf(this);
    if (index >= 0) {
        Ut_NotificationAreaSink::bodies.replace(index, body);
    } else {
        Ut_NotificationAreaSink::bodies.append(body);
    }
}

void DuiInfoBanner::setIconID(const QString &iconId)
{
    int index = Ut_NotificationAreaSink::notifications.indexOf(this);
    if (index >= 0) {
        Ut_NotificationAreaSink::buttonIcons.replace(index, iconId);
    } else {
        Ut_NotificationAreaSink::buttonIcons.append(iconId);
    }
}

// DuiSceneWindow stubs (used by NotificationAreaSink)
void DuiSceneWindow::disappear()
{
    emit windowHidden();
}

// QFileInfo stubs
bool QFileInfo::exists() const
{
    return true;
}

// QDir Stubs
bool QDir::exists() const
{
    return true;
}

QStringList QDir::entryList(const QStringList &, Filters, SortFlags) const
{
    return QStringList();
}

//EventTypeStore stubs
EventTypeStore::EventTypeStore(const QString &, uint)
{
}

const QSettings *EventTypeStore::settingsForEventType(QString const &) const
{
    return settings;
}

maemosec::storage::~storage()
{
}

QList<QString> Ut_NotificationAreaSink::icons;
QList<QString> Ut_NotificationAreaSink::bodies;
QList<QString> Ut_NotificationAreaSink::buttonIcons;
QList<QString> Ut_NotificationAreaSink::contents;
QList<DuiInfoBanner *> Ut_NotificationAreaSink::notifications;
QList<DuiInfoBanner *> Ut_NotificationAreaSink::destroyedNotifications;

// Tests
void Ut_NotificationAreaSink::initTestCase()
{
    // Create a DuiAapplication
    static int argc = 1;
    static char *app_name = (char *)"./ut_notificationareasink";
    app = new DuiApplication(argc, &app_name);
}

void Ut_NotificationAreaSink::cleanupTestCase()
{
    // Destroy DuiApplication
    delete app;
    delete settings;
}

void Ut_NotificationAreaSink::init()
{
    sink = new NotificationAreaSink();
    settings = new QSettings();
    connect(this, SIGNAL(addNotification(Notification)), sink, SLOT(addNotification(Notification)));
    connect(this, SIGNAL(removeNotification(uint)), sink, SLOT(removeNotification(uint)));
    connect(this, SIGNAL(addGroup(uint, const NotificationParameters &)), sink, SLOT(addGroup(uint, const NotificationParameters &)));
    connect(this, SIGNAL(removeGroup(uint)), sink, SLOT(removeGroup(uint)));
    connect(sink, SIGNAL(addNotification(DuiInfoBanner &)), this, SLOT(addNotification(DuiInfoBanner &)));
    connect(sink, SIGNAL(removeNotification(DuiInfoBanner &)), this, SLOT(removeNotification(DuiInfoBanner &)));
}

void Ut_NotificationAreaSink::addNotification(DuiInfoBanner &notification)
{
    Ut_NotificationAreaSink::notifications.append(&notification);

    notification.setParentItem(new DuiWidget());
}

void Ut_NotificationAreaSink::removeNotification(DuiInfoBanner &notification)
{
    int index = Ut_NotificationAreaSink::notifications.indexOf(&notification);

    if (index >= 0) {
        Ut_NotificationAreaSink::icons.removeAt(index);
        Ut_NotificationAreaSink::bodies.removeAt(index);
        Ut_NotificationAreaSink::buttonIcons.removeAt(index);
        Ut_NotificationAreaSink::notifications.removeAt(index);

        notification.setParentItem(0);
    }
}

void Ut_NotificationAreaSink::cleanup()
{
    delete sink;
    icons.clear();
    bodies.clear();
    buttonIcons.clear();
    contents.clear();
    notifications.clear();
    destroyedNotifications.clear();
}

void Ut_NotificationAreaSink::testAddNotification()
{
    QSignalSpy addSpy(sink, SIGNAL(addNotification(DuiInfoBanner &)));

    // Create three notifications - two with a content link and one without
    TestNotificationParameters parameters0("icon0", "body0", "buttonicon0", "content0");
    emit addNotification(Notification(0, 0, 2, parameters0, NotificationManagerInterface::ApplicationEvent, 1000));
    TestNotificationParameters parameters1("icon1", "body1", "buttonicon1", "content1");
    emit addNotification(Notification(1, 0, 2, parameters1, NotificationManagerInterface::SystemEvent, 1000));
    TestNotificationParameters parameters2("icon2", "body2", "buttonicon2", "");
    emit addNotification(Notification(2, 0, 2, parameters2, NotificationManagerInterface::ApplicationEvent, 1000));

    // Check that the addNotification() signal was emitted by the sink two times
    QCOMPARE(addSpy.count(), 2);
    QCOMPARE(notifications.count(), 2);
}

void Ut_NotificationAreaSink::testUpdateNotification()
{
    QSignalSpy addSpy(sink, SIGNAL(addNotification(DuiInfoBanner &)));

    // Add two notifications with the same id; the second should update the existing one.
    TestNotificationParameters parameters0("icon0", "body0", "buttonicon0", "content0");
    emit addNotification(Notification(0, 0, 2, parameters0, NotificationManagerInterface::ApplicationEvent, 1000));
    QCOMPARE(addSpy.count(), 1);
    TestNotificationParameters parameters1("icon1", "body1", "buttonicon1", "content1");
    emit addNotification(Notification(0, 0, 2, parameters1, NotificationManagerInterface::ApplicationEvent, 1000));
    QCOMPARE(addSpy.count(), 1);
    QCOMPARE(notifications.count(), 1);

    // TODO: even though contents.length is 2, there's only 1 action in the duinotification
    // clearing of the actions should be stubbed somehow...
    QCOMPARE(contents.length(), 2);
    QCOMPARE(contents[0], QString("content0"));
    QCOMPARE(contents[1], QString("content1"));
}

void Ut_NotificationAreaSink::testRemoveNotification()
{
    QSignalSpy addSpy(sink, SIGNAL(addNotification(DuiInfoBanner &)));
    QSignalSpy removeSpy(sink, SIGNAL(removeNotification(DuiInfoBanner &)));

    // Create three notifications
    TestNotificationParameters parameters0("icon0", "body0", "buttonicon0", "content0");
    emit addNotification(Notification(0, 0, 2, parameters0, NotificationManagerInterface::SystemEvent, 1000));
    TestNotificationParameters parameters1("icon1", "body1", "buttonicon1", "content1");
    emit addNotification(Notification(1, 0, 2, parameters1, NotificationManagerInterface::ApplicationEvent, 1000));
    TestNotificationParameters parameters2("icon2", "body2", "buttonicon2", "");
    emit addNotification(Notification(2, 0, 2, parameters2, NotificationManagerInterface::ApplicationEvent, 1000));

    // Check that the addNotification() signal was emitted by the sink two times
    QCOMPARE(addSpy.count(), 2);
    QCOMPARE(notifications.count(), 2);

    // Cancel the second one
    emit removeNotification(1);

    // Check that the removeNotification() signal was emitted by the sink once
    QCOMPARE(removeSpy.count(), 1);
    QCOMPARE(notifications.count(), 1);
    QCOMPARE(destroyedNotifications.count(), 1);

    // Recreate the second notification and create an additional one
    emit addNotification(Notification(1, 0, 2, parameters1, NotificationManagerInterface::ApplicationEvent, 1000));
    TestNotificationParameters parameters3("icon3", "body3", "buttonicon3", "");
    emit addNotification(Notification(3, 0, 2, parameters3, NotificationManagerInterface::SystemEvent, 1000));

    // Check that the addNotification() signal was emitted by the sink three times in total
    QCOMPARE(addSpy.count(), 3);
    QCOMPARE(icons.length(), 2);
    QCOMPARE(icons[0], QString("icon2"));
    QCOMPARE(icons[1], QString("icon1"));
}

void Ut_NotificationAreaSink::testAddGroup()
{
    QSignalSpy addSpy(sink, SIGNAL(addNotification(DuiInfoBanner &)));

    // Creating a group should not send signals, just create the duinotification
    TestNotificationParameters parameters0("icon0", "body0", "buttonicon0", "content0");
    emit addGroup(1, parameters0);
    QCOMPARE(addSpy.count(), 0);
    QCOMPARE(notifications.count(), 0);
}

void Ut_NotificationAreaSink::testRemoveGroup()
{
    QSignalSpy remSpy(sink, SIGNAL(removeNotification(DuiInfoBanner &)));

    // A signal from removeGroup is not sent if no notification has been added to the group
    TestNotificationParameters parameters0("icon0", "body0", "buttonicon0", "content0");
    emit addGroup(1, parameters0);
    emit removeGroup(1);
    QCOMPARE(remSpy.count(), 0);

    emit addGroup(1, parameters0);
    TestNotificationParameters parameters1("icon1", "body1", "buttonicon1", "content1");
    emit addNotification(Notification(0, 1, 2, parameters1, NotificationManagerInterface::ApplicationEvent, 1000));
    emit removeGroup(1);
    QCOMPARE(remSpy.count(), 1);
}

void Ut_NotificationAreaSink::testAddNotificationToGroup()
{
    QSignalSpy addSpy(sink, SIGNAL(addNotification(DuiInfoBanner &)));
    TestNotificationParameters parameters0("image0", "body0", "icon0", "content0");
    emit addGroup(1, parameters0);
    TestNotificationParameters parameters1("image1", "body1", "icon1", "content1");
    emit addNotification(Notification(0, 1, 2, parameters1, NotificationManagerInterface::ApplicationEvent, 1000));

    QCOMPARE(addSpy.count(), 1);
    QCOMPARE(notifications.count(), 1);
}

void Ut_NotificationAreaSink::testUpdateGroup()
{
    TestNotificationParameters parameters0("icon0", "body0", "buttonicon0", "content0");
    emit addGroup(1, parameters0);
    TestNotificationParameters parameters1("icon1", "body1", "buttonicon1", "content1");
    emit addNotification(Notification(0, 1, 2, parameters1, NotificationManagerInterface::ApplicationEvent, 1000));

    QCOMPARE(icons.length(), 1);
    QCOMPARE(icons[0], QString("icon0"));
    QCOMPARE(bodies.length(), 1);
    QCOMPARE(bodies[0], QString("body0"));
    QCOMPARE(buttonIcons.length(), 1);
    QCOMPARE(buttonIcons[0], QString("buttonicon0"));
    QCOMPARE(contents.length(), 1);
    QCOMPARE(contents[0], QString("content0"));

    emit addGroup(1, parameters1);

    QCOMPARE(icons.length(), 1);
    QCOMPARE(icons[0], QString("icon1"));
    QCOMPARE(bodies.length(), 1);
    QCOMPARE(bodies[0], QString("body1"));
    QCOMPARE(buttonIcons.length(), 1);
    QCOMPARE(buttonIcons[0], QString("buttonicon1"));
    // TODO: even though contents.length is 2, there's only 1 action in the duinotification
    // clearing of the actions should be stubbed somehow...
    QCOMPARE(contents.length(), 2);
    QCOMPARE(contents[1], QString("content1"));
}

void Ut_NotificationAreaSink::testNotificationClicked()
{
    QSignalSpy remSpy(sink, SIGNAL(removeNotification(DuiInfoBanner &)));

    // Create a notification
    TestNotificationParameters parameters0("icon0", "body0", "buttonicon0", "content0");
    emit addNotification(Notification(0, 0, 2, parameters0, NotificationManagerInterface::ApplicationEvent, 1000));

    // Clicking it should emit removeNotification() and destroy the info banner
    connect(this, SIGNAL(click()), Ut_NotificationAreaSink::notifications.at(0), SIGNAL(clicked()));
    emit click();
    QApplication::processEvents();
    QCOMPARE(remSpy.count(), 1);
    QCOMPARE(Ut_NotificationAreaSink::notifications.count(), 0);
    QCOMPARE(Ut_NotificationAreaSink::destroyedNotifications.count(), 1);

    // Create a group and a notification in it
    TestNotificationParameters parameters1("icon1", "body1", "buttonicon1", "content1");
    emit addGroup(1, parameters1);
    emit addNotification(Notification(0, 1, 2, parameters0, NotificationManagerInterface::ApplicationEvent, 1000));

    // Clicking it should emit removeNotification() but not destroy the info banner
    connect(this, SIGNAL(click()), Ut_NotificationAreaSink::notifications.at(0), SIGNAL(clicked()));
    emit click();
    QApplication::processEvents();
    QCOMPARE(remSpy.count(), 2);
    QCOMPARE(Ut_NotificationAreaSink::notifications.count(), 0);
    QCOMPARE(Ut_NotificationAreaSink::destroyedNotifications.count(), 1);
}

QTEST_APPLESS_MAIN(Ut_NotificationAreaSink)
