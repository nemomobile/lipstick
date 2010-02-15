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
#include "ut_notificationparameters.h"
#include "notificationparameters.h"
#include "notificationparameter.h"

class TestSink
{
public:
    static NotificationParameter createTest1Parameter(int value) {
        return NotificationParameter("test1", QVariant(value));
    }
    static NotificationParameter createTest2Parameter(const QString &value) {
        return NotificationParameter("test2", QVariant(value));
    }
};

void Ut_NotificationParameters::initTestCase()
{
}

void Ut_NotificationParameters::cleanupTestCase()
{
}

void Ut_NotificationParameters::init()
{
    parameters = new NotificationParameters();
}

void Ut_NotificationParameters::cleanup()
{
    delete parameters;
}

void Ut_NotificationParameters::testKeyValueAPI()
{
    QVariant v1(5);
    QVariant v2("Test");
    parameters->add("test1", v1);
    parameters->add("test2", v2);
    QCOMPARE(parameters->value("test1"), v1);
    QCOMPARE(parameters->value("test2"), v2);
    QCOMPARE(parameters->value("test3").isNull(), true);
}

void Ut_NotificationParameters::testParameterAPI()
{
    NotificationParameter p1 = TestSink::createTest1Parameter(5);
    NotificationParameter p2 = TestSink::createTest2Parameter("Test");
    parameters->add(p1);
    parameters->add(p2);
    QCOMPARE(parameters->value("test1"), QVariant(5));
    QCOMPARE(parameters->value("test2"), QVariant("Test"));
    QCOMPARE(parameters->value("test3").isNull(), true);
}

void Ut_NotificationParameters::testSerialization()
{
    NotificationParameter p1 = TestSink::createTest1Parameter(5);
    NotificationParameter p2 = TestSink::createTest2Parameter("Test");

    NotificationParameters params1;
    NotificationParameters params2;

    params1.add(p1);
    params1.add(p2);

    // Transfer the parameters from params1 to params2
    // through a byte array
    QByteArray ba;
    QDataStream stream(&ba, QIODevice::ReadWrite);
    stream << params1;
    stream.device()->seek(0);
    stream >> params2;

    QCOMPARE(params2.value("test1"), QVariant(5));
    QCOMPARE(params2.value("test2"), QVariant("Test"));
    QCOMPARE(params2.value("test3").isNull(), true);
}

QTEST_MAIN(Ut_NotificationParameters)
