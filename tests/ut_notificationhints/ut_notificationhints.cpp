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

#include <QtTest/QtTest>
#include "ut_notificationhints.h"
#include "notificationhints.h"
#include "qdbusargument_fake.h"

void Ut_NotificationHints::testGettersAndSetters()
{
    NotificationHints hints;
    QString hint1("hint1");
    QString hint2("hint2");
    QString hint3("hint3");
    QVariant value1("value1");
    QVariant value2(true);
    hints.setHint(hint1, value1);
    hints.setHint(hint2, value2);
    QCOMPARE(hints.hintValue(hint1), value1);
    QCOMPARE(hints.hintValue(hint2), value2);
    QCOMPARE(hints.hintValue(hint3).isValid(), false);
    QCOMPARE(hints.hints().count(), 2);
    QCOMPARE(hints.hints().contains(hint1), QBool(true));
    QCOMPARE(hints.hints().contains(hint2), QBool(true));
    QCOMPARE(hints.hints().contains(hint3), QBool(false));
}

void Ut_NotificationHints::testDBusSerialization()
{
    NotificationHints hints1;
    NotificationHints hints2;
    QString hint1("hint1");
    QString hint2("hint2");
    QString hint3("hint3");
    QVariant value1("value1");
    QVariant value2(true);
    hints1.setHint(hint1, value1);
    hints1.setHint(hint2, value2);
    QDBusArgument arg;
    arg << hints1;
    arg >> hints2;
    QCOMPARE(hints2.hints().count(), hints1.hints().count());
    QCOMPARE(hints2.hintValue(hint1), value1);
    QCOMPARE(hints2.hintValue(hint2), value2);
    QCOMPARE(hints2.hintValue(hint3).isValid(), false);
}

QTEST_MAIN(Ut_NotificationHints)
