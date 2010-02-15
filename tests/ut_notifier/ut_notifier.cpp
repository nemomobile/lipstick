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

#include <DuiApplication>
#include "ut_notifier.h"
#include "notifier.h"
#include "notifiernotificationsink.h"
#include "homeapplication_stub.h"
#include "notificationsink_stub.h"

// NotifierNotificationSink stubs (used by Notifier)
NotifierNotificationSink::NotifierNotificationSink()
{
}

NotifierNotificationSink::~NotifierNotificationSink()
{
}


void NotifierNotificationSink::addGroup(uint, const NotificationParameters &)
{
}

void NotifierNotificationSink::removeGroup(uint)
{
}

void NotifierNotificationSink::addNotification(const Notification &)
{
}

void NotifierNotificationSink::removeNotification(uint)
{
}

void Ut_Notifier::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_notifier";
    app = new DuiApplication(argc, &app_name);
}

void Ut_Notifier::cleanupTestCase()
{
    delete app;
}

void Ut_Notifier::init()
{
    m_subject = new Notifier();
    connect(this, SIGNAL(notificationCountChanged(uint)), m_subject, SLOT(notificationCountChanged(uint)));
}

void Ut_Notifier::cleanup()
{
    delete m_subject;
}

void Ut_Notifier::testNotificationCountChanged()
{
    // The notifier should be visible when there are notifications
    emit notificationCountChanged(5);
    QCOMPARE(m_subject->text(), QString("5"));
    QVERIFY(m_subject->isVisible());

    // The notifier should not be visible when there are no notifications
    emit notificationCountChanged(0);
    QCOMPARE(m_subject->text(), QString("0"));
    QVERIFY(!m_subject->isVisible());
}

QTEST_APPLESS_MAIN(Ut_Notifier)
