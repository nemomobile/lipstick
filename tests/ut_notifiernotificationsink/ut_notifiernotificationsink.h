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

#ifndef UT_NOTIFIERNOTIFICATIONSINK_H
#define UT_NOTIFIERNOTIFICATIONSINK_H

#include <QtTest/QtTest>
#include <QObject>

class NotifierNotificationSink;
class Notification;
class NotificationParameters;

class Ut_NotifierNotificationSink : public QObject
{
    Q_OBJECT

private slots:
    // Called before the first testfunction is executed
    void initTestCase();
    // Called after the last testfunction was executed
    void cleanupTestCase();
    // Called before each testfunction is executed
    void init();
    // Called after every testfunction
    void cleanup();

    // Test cases
    void testAddNotification();
    void testRemoveNotification();
    void testNotificationsInGroups();

signals:
    void addNotification(const Notification &notification);
    void removeNotification(uint notificationId);
    void addGroup(uint groupId, const NotificationParameters &parameters);
    void removeGroup(uint groupId);

private:
    // The object being tested
    NotifierNotificationSink *m_subject;
};

#endif
