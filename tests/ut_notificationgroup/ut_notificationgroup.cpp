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

#include <QByteArray>
#include "ut_notificationgroup.h"
#include "notificationparameters.h"
#include "notificationgroup.h"

void Ut_NotificationGroup::initTestCase()
{
}

void Ut_NotificationGroup::cleanupTestCase()
{
}

void Ut_NotificationGroup::init()
{
}

void Ut_NotificationGroup::cleanup()
{
}

void Ut_NotificationGroup::testSerializationAndDeserialization()
{
    NotificationParameters parameters0;
    parameters0.add("imageId", "icon0");

    NotificationGroup ng1(20, 1234, parameters0);
    NotificationGroup ng2;

    // Transfer the state of notification group ng1 to ng2
    // through a byte array
    QByteArray ba;
    QDataStream stream(&ba, QIODevice::ReadWrite);
    stream << ng1;
    stream.device()->seek(0);
    stream >> ng2;

    QCOMPARE(ng2.groupId(), uint(20));
    QCOMPARE(ng2.userId(), uint(1234));
    QCOMPARE(ng2.parameters().value("imageId").toString(), QString("icon0"));
}


QTEST_MAIN(Ut_NotificationGroup)
