/***************************************************************************
**
** Copyright (C) 2012 Jolla Ltd.
** Contact: Robin Burchell <robin.burchell@jollamobile.com>
**
** This file is part of lipstick.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#ifndef UT_NOTIFICATIONFEEDBACKPLAYER_H_
#define UT_NOTIFICATIONFEEDBACKPLAYER_H_

#include <QObject>

class NotificationFeedbackPlayer;

class Ut_NotificationFeedbackPlayer : public QObject
{
Q_OBJECT

signals:
    void timeout();

private slots:
    void initTestCase();
    void init();
    void cleanup();

    // Test that feedback is played and stopped
    void testAddAndRemoveNotification();
    void testWithoutFeedbackId();
    void testMultipleFeedbackIds();
    void testHiddenNotification();
    void testUpdateNotification();
    void testUpdateNotificationAfterRestart();
    void testNotificationPreviewsDisabled_data();
    void testNotificationPreviewsDisabled();
    void testNotificationPriority_data();
    void testNotificationPriority();
    void testLEDDisabledWhenNoSummaryAndBody_data();
    void testLEDDisabledWhenNoSummaryAndBody();

private:
    NotificationFeedbackPlayer *player;
};

#endif // UT_NOTIFICATIONFEEDBACKPLAYER_H_
