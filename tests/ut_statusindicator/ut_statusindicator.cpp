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

#include "ut_statusindicator.h"
#include <DuiApplication>
#include <DuiOnDisplayChangeEvent>
#include "statusindicator.h"
#include "testcontextitem.h"
#include <QtTest/QtTest>

QHash<QString, TestContextItem *> testContextItems;

// Test context
class TestContext : public ApplicationContext
{
public:
    virtual ContextItem *createContextItem(const QString &key) {
        Q_UNUSED(key)

        testContextItems[key] = new TestContextItem;

        return testContextItems[key];
    }
};

void Ut_StatusIndicator::init()
{
    testContext = new TestContext();
    testContextItems.clear();
}

void Ut_StatusIndicator::cleanup()
{
    delete testContext;
    testContext = NULL;
}

void Ut_StatusIndicator::initTestCase()
{
    // DuiApplications must be created manually these days due to theme system changes
    static int argc = 1;
    static char *app_name = (char *)"./ut_statusindicator";
    app = new DuiApplication(argc, &app_name);
}

void Ut_StatusIndicator::cleanupTestCase()
{
    // Destroy the DuiApplication
    delete app;
}

void Ut_StatusIndicator::testModelUpdates()
{
    DuiOnDisplayChangeEvent exitDisplayEvent(DuiOnDisplayChangeEvent::FullyOffDisplay, QRectF());
    DuiOnDisplayChangeEvent enterDisplayEvent(DuiOnDisplayChangeEvent::FullyOnDisplay, QRectF());
    statusIndicator = new PhoneNetworkStatusIndicator(*testContext);

    // When the application is visible the model should be updated
    qApp->sendEvent(statusIndicator, &enterDisplayEvent);
    testContextItems["Cellular.NetworkName"]->setValue(QVariant("NetworkName"));
    QCOMPARE(statusIndicator->model()->value(), QVariant("NetworkName"));

    // When the application is not visible the model should not be updated
    qApp->sendEvent(statusIndicator, &exitDisplayEvent);
    testContextItems["Cellular.NetworkName"]->setValue(QVariant("OtherNetworkName"));
    QCOMPARE(statusIndicator->model()->value(), QVariant("NetworkName"));

    // When the application becomes visible the model should be updated
    qApp->sendEvent(statusIndicator, &enterDisplayEvent);
    QCOMPARE(statusIndicator->model()->value(), QVariant("OtherNetworkName"));

    delete statusIndicator;
}

void Ut_StatusIndicator::testPhoneNetwork()
{
    statusIndicator = new PhoneNetworkStatusIndicator(*testContext);
    testContextItems["Cellular.NetworkName"]->setValue(QVariant("foobar"));

    QVERIFY(statusIndicator->model()->value().type() == QVariant::String);
    QCOMPARE(statusIndicator->model()->value(), QVariant("foobar"));

    delete statusIndicator;
}

QTEST_APPLESS_MAIN(Ut_StatusIndicator)
