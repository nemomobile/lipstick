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
#include <QByteArray>
#include "ut_notification.h"
#include "notificationparameters.h"
#include "notification.h"

void Ut_Notification::initTestCase()
{
}

void Ut_Notification::cleanupTestCase()
{
}

void Ut_Notification::init()
{
}

void Ut_Notification::cleanup()
{
}

void Ut_Notification::testSerializationAndDeserialization()
{
    NotificationParameters parameters0;
    parameters0.add("imageId", "icon0");

    Notification n1(1234, 20, 678, parameters0, NotificationManagerInterface::ApplicationEvent, 2345);
    Notification n2;

    // Transfer the state of notification n1 to n2
    // through a byte array
    QByteArray ba;
    QDataStream stream(&ba, QIODevice::ReadWrite);
    stream << n1;
    stream.device()->seek(0);
    stream >> n2;

    QCOMPARE(n2.notificationId(), uint(1234));
    QCOMPARE(n2.groupId(), uint(20));
    QCOMPARE(n2.userId(), uint(678));
    QCOMPARE(n2.type(), NotificationManagerInterface::ApplicationEvent);
    QCOMPARE(n2.timeout(), int(2345));
    QCOMPARE(n2.parameters().value("imageId").toString(), QString("icon0"));
}


QTEST_MAIN(Ut_Notification)
