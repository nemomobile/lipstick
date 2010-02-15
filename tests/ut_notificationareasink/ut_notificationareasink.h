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

#ifndef UT_NOTIFICATIONAREASINK_H
#define UT_NOTIFICATIONAREASINK_H

#include <QObject>
#include <QList>
#include <QString>
#include <DuiInfoBanner>
#include "notificationareasink.h"

class DuiApplication;
class NotificationParameters;

class Ut_NotificationAreaSink : public QObject
{
    Q_OBJECT

public:
    static QList<DuiInfoBanner::BannerType> types;
    static QList<QString> icons;
    static QList<QString> bodies;
    static QList<QString> buttonIcons;
    static QList<QString> contents;
    static QList<DuiInfoBanner *> notifications;
    static QList<DuiInfoBanner *> destroyedNotifications;

private:
    DuiApplication *app;
    NotificationAreaSink *sink;

public slots:
    // For faking the addition of a notification to a layout
    void addNotification(DuiInfoBanner &notification);
    // For faking the removal of a notification from a layout
    void removeNotification(DuiInfoBanner &notification);

private slots:
    // Executed once before every test case
    void init();
    // Executed once after every test case
    void cleanup();
    // Executed once before first test case
    void initTestCase();
    // Executed once after last test case
    void cleanupTestCase();

    // Test that correct kinds of notification widgets are displayed that their IDs are unique
    void testAddNotification();
    // Test that adding a group creates correct notification widget but doesn't send signals if empty
    void testAddGroup();
    // Tests that signals for removing the group is sent only if notification was added to the group
    void testRemoveGroup();
    // Tests that adding a notification to a group will show the group notification
    void testAddNotificationToGroup();
    // Tests that a notification group is updated correctly
    void testUpdateGroup();
    // Test that updating with an ID updates the correct notification
    void testUpdateNotification();
    // Test that canceling with an ID cancels the correct notification
    void testRemoveNotification();
    // Test that clicking a notification will remove the notification from the manager
    void testNotificationClicked();

signals:
    void addGroup(uint groupId, const NotificationParameters &parameters);
    void removeGroup(uint groupId);
    void addNotification(Notification);
    void removeNotification(uint notificationId);
    void click();
};

#endif // UT_NOTIFICATIONAREASINK_H
