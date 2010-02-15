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

#ifndef UT_HOMENOTIFICATIONSINK_H
#define UT_HOMENOTIFICATIONSINK_H

#include <QObject>
#include <QList>
#include <QString>
#include <DuiInfoBanner>
#include "notificationmanager.h"

class DuiApplication;
class MainWindow;
class HomeNotificationSink;

class Ut_HomeNotificationSink : public QObject
{
    Q_OBJECT

public:
    static QList<DuiInfoBanner::BannerType> types;
    static QList<QString> icons;
    static QList<QString> bodies;
    static QList<QString> buttonIcons;
    static QList<QString> contents;
    static QHash<const QGraphicsWidget *, QList<QAction *> > actions;
    static QList<DuiInfoBanner *> notifications;
    static bool instantAnimation;
    static int lastTimeout;
    static QTimer *lastTimer;
    static MainWindow *mainWin;

private:
    DuiApplication *app;
    HomeNotificationSink *sink;

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
    // Test that updating with an ID updates the correct notification
    void testUpdateNotification();
    // Test that canceling with an ID cancels the correct notification
    void testRemoveNotification();
    // Test that finishing a notification animation sends the transferNotification signal in the case of Application event notifications
    void testTransferNotification();
    // Test that notifications disappear after a timeout
    void testTimeout();

signals:
    void addNotification(Notification);
    void removeNotification(uint notificationId);
};

#endif // UT_HOMENOTIFICATIONSINK_H
