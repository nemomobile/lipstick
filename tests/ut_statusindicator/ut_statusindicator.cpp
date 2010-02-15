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

#include <DuiOnDisplayChangeEvent>
#include "ut_statusindicator.h"
#include "statusindicator.h"
#include "statusindicatorimageview.h"
#include "statusindicatorlabelview.h"
#include "testcontextitem.h"

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

// Stubs for status indicator views

QVariant gModelValue;

TestStatusIndicatorImageView::TestStatusIndicatorImageView(StatusIndicator *controller) :
    StatusIndicatorImageView(controller)
{
}

void TestStatusIndicatorImageView::updateData(const QList<const char *>& modifications)
{
    DuiWidgetView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == StatusIndicatorModel::Value) {
            gModelValue = model()->value();
        }
    }
}

TestStatusIndicatorLabelView::TestStatusIndicatorLabelView(StatusIndicator *controller) :
    StatusIndicatorImageView(controller)
{
}



void Ut_StatusIndicator::init()
{
    testContext = new TestContext();
    testContextItems.clear();
    gModelValue.clear();
}

void Ut_StatusIndicator::cleanup()
{
    delete testContext;
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
    statusIndicator = new ClockAlarmStatusIndicator(*testContext);

    // When the application is visible the model should be updated
    qApp->sendEvent(statusIndicator, &enterDisplayEvent);
    testContextItems["UserAlarm.Present"]->setValue(QVariant(true));
    QCOMPARE(statusIndicator->model()->value(), QVariant(1));

    // When the application is not visible the model should not be updated
    qApp->sendEvent(statusIndicator, &exitDisplayEvent);
    testContextItems["UserAlarm.Present"]->setValue(QVariant(false));
    QCOMPARE(statusIndicator->model()->value(), QVariant(1));

    // When the application becomes visible the model should be updated
    qApp->sendEvent(statusIndicator, &enterDisplayEvent);
    QCOMPARE(statusIndicator->model()->value(), QVariant(0));

    delete statusIndicator;
}

void Ut_StatusIndicator::testPhoneNetworkSignalStrength()
{
    statusIndicator = new PhoneNetworkSignalStrengthStatusIndicator(*testContext);
    statusIndicator->setView(new TestStatusIndicatorImageView(statusIndicator));

    testContextItems["Cellular.SignalStrength"]->setValue(QVariant(100));

    QVERIFY(statusIndicator->model()->value().type() == QVariant::Double);
    QCOMPARE(qRound(statusIndicator->model()->value().toDouble() * 100), 100);

    delete statusIndicator;
}

void Ut_StatusIndicator::testBattery()
{
    statusIndicator = new BatteryStatusIndicator(*testContext);

    testContextItems["Battery.ChargePercentage"]->setValue(QVariant(100));
    QVERIFY(statusIndicator->model()->value().type() == QVariant::Double);
    QCOMPARE(qRound(statusIndicator->model()->value().toDouble() * 100), 100);

    testContextItems["Battery.IsCharging"]->setValue(QVariant(false));
    QVERIFY(statusIndicator->objectName().indexOf("Level") >= 0);

    testContextItems["Battery.IsCharging"]->setValue(QVariant(true));
    QVERIFY(statusIndicator->objectName().indexOf("Charging") >= 0);

    delete statusIndicator;
}

void Ut_StatusIndicator::testAlarm()
{
    statusIndicator = new ClockAlarmStatusIndicator(*testContext);

    testContextItems["UserAlarm.Present"]->setValue(QVariant(false));
    QCOMPARE(statusIndicator->model()->value(), QVariant(false));

    testContextItems["UserAlarm.Present"]->setValue(QVariant(true));
    QCOMPARE(statusIndicator->model()->value(), QVariant(true));

    delete statusIndicator;
}

void Ut_StatusIndicator::testBluetooth()
{
    statusIndicator = new BluetoothStatusIndicator(*testContext);

    testContextItems["Bluetooth.Enabled"]->setValue(QVariant(false));
    QCOMPARE(statusIndicator->model()->value(), QVariant(false));

    testContextItems["Bluetooth.Enabled"]->setValue(QVariant(true));
    QCOMPARE(statusIndicator->model()->value(), QVariant(true));

    delete statusIndicator;
}

void Ut_StatusIndicator::testInternetConnection()
{
    statusIndicator = new InternetConnectionStatusIndicator(*testContext);

    testContextItems["Internet.SignalStrength"]->setValue(QVariant(100));
    QVERIFY(statusIndicator->model()->value().type() == QVariant::Double);
    QCOMPARE(qRound(statusIndicator->model()->value().toDouble() * 100), 100);

    delete statusIndicator;
}

void Ut_StatusIndicator::testAnimation()
{
    DuiOnDisplayChangeEvent exitDisplayEvent(DuiOnDisplayChangeEvent::FullyOffDisplay, QRectF());
    DuiOnDisplayChangeEvent enterDisplayEvent(DuiOnDisplayChangeEvent::FullyOnDisplay, QRectF());
    statusIndicator = new BatteryStatusIndicator(*testContext);

    testContextItems["Battery.IsCharging"]->setValue(QVariant(true));
    qApp->sendEvent(statusIndicator, &exitDisplayEvent);
    QCOMPARE(statusIndicator->model()->animate(), false);

    qApp->sendEvent(statusIndicator, &enterDisplayEvent);
    QCOMPARE(statusIndicator->model()->animate(), true);
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
