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

#ifndef UT_DUIFEEDBACKNOTIFICATIONSINK_H
#define UT_DUIFEEDBACKNOTIFICATIONSINK_H

#include <QtTest/QtTest>
#include <QObject>
#include <QList>
#include "notification.h"

class DuiApplication;
class DuiFeedbackNotificationSink;
class QSettings;

class Ut_DuiFeedbackNotificationSink : public QObject
{
    Q_OBJECT

public:
    static QList<QString> played;

private slots:
    // Called before the first testfunction is executed
    void initTestCase();
    // Called after the last testfunction was executed
    void cleanupTestCase();
    // Called before each testfunction is executed
    void init();
    // Called after every testfunction
    void cleanup();

    // Test that feedback is played
    void testAddNotification();
    // Test that removing does nothing (at least for now)
    void testRemoveNotification();

    void testNotificationWhileApplicationEventsDisabled();
    void testWithoutEventTypeOrFeedbackId();
    void testWithEventTypeWithoutFeedbackId();
    void testWithEventTypeAndFeedbackId();
    void testWithoutEventTypeWithFeedbackId();
    void testDetermineFeedBackId();

private:
    // DuiApplication
    DuiApplication *app;
    // The object being tested
    DuiFeedbackNotificationSink *sink;
    // QSettings for the event type store
    QSettings *eventTypeSettings;

signals:
    void addNotification(Notification notification);
    void removeNotification(uint notificationId);
};

#endif
