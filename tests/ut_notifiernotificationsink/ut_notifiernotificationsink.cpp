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

#include "ut_notifiernotificationsink.h"
#include "notifiernotificationsink.h"
#include "notification.h"

void Ut_NotifierNotificationSink::initTestCase()
{
}

void Ut_NotifierNotificationSink::cleanupTestCase()
{
}

void Ut_NotifierNotificationSink::init()
{
    m_subject = new NotifierNotificationSink();
    connect(this, SIGNAL(addNotification(const Notification &)), m_subject, SLOT(addNotification(const Notification &)));
    connect(this, SIGNAL(removeNotification(uint)), m_subject, SLOT(removeNotification(uint)));
    connect(this, SIGNAL(addGroup(uint, const NotificationParameters &)), m_subject, SLOT(addGroup(uint, const NotificationParameters &)));
    connect(this, SIGNAL(removeGroup(uint)), m_subject, SLOT(removeGroup(uint)));
}

void Ut_NotifierNotificationSink::cleanup()
{
    delete m_subject;
}

void Ut_NotifierNotificationSink::testAddNotification()
{
    QSignalSpy spy(m_subject, SIGNAL(notificationCountChanged(uint)));

    // Adding a notification should cause a notification count change
    NotificationParameters notification1Parameters;
    notification1Parameters.add("count", QVariant((uint)5));
    Notification notification1(1, 0, 2, notification1Parameters, NotificationManagerInterface::ApplicationEvent, 0);
    emit addNotification(notification1);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toInt(), 5);

    // Adding a new notification should cause a notification count change
    NotificationParameters notification2Parameters;
    notification2Parameters.add("count", QVariant((uint)8));
    Notification notification2(2, 0, 2, notification2Parameters, NotificationManagerInterface::ApplicationEvent, 0);
    emit addNotification(notification2);
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0).toInt(), 13);

    // Updating the same notification should cause a notification count change
    notification2Parameters.add("count", QVariant((uint)6));
    notification2.setParameters(notification2Parameters);
    emit addNotification(notification2);
    QCOMPARE(spy.count(), 4);
    QCOMPARE(spy.at(3).at(0).toInt(), 11);
}

void Ut_NotifierNotificationSink::testRemoveNotification()
{
    QSignalSpy spy(m_subject, SIGNAL(notificationCountChanged(uint)));

    // Removing an inexisting notification should do nothing
    emit removeNotification(1);
    QCOMPARE(spy.count(), 0);

    // Removing an existing notification should inform about the change in notification count
    NotificationParameters notification1Parameters;
    notification1Parameters.add("count", QVariant((uint)5));
    Notification notification1(1, 0, 2, notification1Parameters, NotificationManagerInterface::ApplicationEvent, 0);
    emit addNotification(notification1);
    emit removeNotification(1);
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0).toInt(), 0);

    // Removing a notification should really remove the notification
    emit removeNotification(1);
    QCOMPARE(spy.count(), 2);
}

void Ut_NotifierNotificationSink::testNotificationsInGroups()
{
    QSignalSpy spy(m_subject, SIGNAL(notificationCountChanged(uint)));

    // Creating a notification group should do nothing
    NotificationParameters parameters;
    parameters.add("count", QVariant((uint)8));
    emit addGroup(1, parameters);
    QCOMPARE(spy.count(), 0);

    // Adding a notification into the group should cause a notification count change so that the group's count is used
    NotificationParameters notification1Parameters;
    notification1Parameters.add("count", QVariant((uint)5));
    Notification notification1(1, 1, 2, notification1Parameters, NotificationManagerInterface::ApplicationEvent, 0);
    emit addNotification(notification1);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.at(0).at(0).toInt(), 8);

    // Adding a new notification into the same group should not cause a notification count change
    NotificationParameters notification2Parameters;
    notification2Parameters.add("count", QVariant((uint)8));
    Notification notification2(2, 1, 2, notification2Parameters, NotificationManagerInterface::ApplicationEvent, 0);
    emit addNotification(notification2);
    QCOMPARE(spy.count(), 1);

    // Updating the same notification should not cause a notification count change
    notification2Parameters.add("count", QVariant((uint)6));
    notification2.setParameters(notification2Parameters);
    emit addNotification(notification2);
    QCOMPARE(spy.count(), 1);

    // Updating the notification group should cause a notification count change
    parameters.add("count", QVariant((uint)13));
    emit addGroup(1, parameters);
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.at(1).at(0).toInt(), 13);

    // Removing the second notification should not cause a notification count change
    emit removeNotification(2);
    QCOMPARE(spy.count(), 2);

    // Removing the first notification should cause a notification count change
    emit removeNotification(1);
    QCOMPARE(spy.count(), 3);
    QCOMPARE(spy.at(2).at(0).toInt(), 0);

    // Removing a notification group with notifications in it should cause a notification count change
    emit addNotification(notification1);
    emit addNotification(notification2);
    emit removeGroup(1);
    QCOMPARE(spy.count(), 5);
    QCOMPARE(spy.at(4).at(0).toInt(), 0);
}

QTEST_APPLESS_MAIN(Ut_NotifierNotificationSink)
