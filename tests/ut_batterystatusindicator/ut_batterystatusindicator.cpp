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
#include <contextproperty.h>
#include "ut_batterystatusindicator.h"
#include "batterystatusindicator.h"

// ContextProperty stubs
QVariantHash contextPropertyValues;
QVariant ContextProperty::value(const QVariant &def) const
{
    return contextPropertyValues.value(key(), def);
}

QVariant ContextProperty::value() const
{
    return contextPropertyValues.value(key());
}

QSet<ContextProperty *> contextPropertySubscriptions;
void ContextProperty::subscribe() const
{
    contextPropertySubscriptions.insert(const_cast<ContextProperty *>(this));
}

void ContextProperty::unsubscribe() const
{
    contextPropertySubscriptions.remove(const_cast<ContextProperty *>(this));
}

ContextProperty *contextProperty(const QString &key)
{
    foreach (ContextProperty *contextProperty, contextPropertySubscriptions) {
        if (contextProperty->key() == key) {
            return contextProperty;
        }
    }
    return 0;
}

void Ut_BatteryStatusIndicator::testConstruction()
{
    contextPropertyValues.insert("Battery.ChargePercentage", 53);
    contextPropertyValues.insert("Battery.IsCharging", false);
    contextPropertyValues.insert("System.PowerSaveMode", false);

    QObject parent;
    BatteryStatusIndicator statusIndicator(&parent);
    QCOMPARE(statusIndicator.parent(), &parent);
    QCOMPARE(statusIndicator.mode(), QString());
    QCOMPARE(contextPropertySubscriptions.count(), 3);
    ContextProperty *batteryChargeBarsContextProperty = contextProperty("Battery.ChargePercentage");
    ContextProperty *batteryIsChargingContextProperty = contextProperty("Battery.IsCharging");
    ContextProperty *systemPowerSaveModeContextProperty = contextProperty("System.PowerSaveMode");
    QCOMPARE(disconnect(batteryChargeBarsContextProperty, SIGNAL(valueChanged()), &statusIndicator, SLOT(setValue())), true);
    QCOMPARE(disconnect(batteryIsChargingContextProperty, SIGNAL(valueChanged()), &statusIndicator, SLOT(setMode())), true);
    QCOMPARE(disconnect(systemPowerSaveModeContextProperty, SIGNAL(valueChanged()), &statusIndicator, SLOT(setMode())), true);
}

void Ut_BatteryStatusIndicator::testSetMode_data()
{
    QTest::addColumn<bool>("batteryIsCharging");
    QTest::addColumn<bool>("systemPowerSaveMode");
    QTest::addColumn<QString>("mode");

    QTest::newRow("Battery is not charging, power save mode disabled") << false << false << QString();
    QTest::newRow("Battery is not charging, power save mode enabled") << false << true << QString("PowerSave");
    QTest::newRow("Battery is charging, power save mode disabled") << true << false << QString("Charging");
    QTest::newRow("Battery is charging, power save mode enabled") << true << true << QString("PowerSaveCharging");
}

void Ut_BatteryStatusIndicator::testSetMode()
{
    QFETCH(bool, batteryIsCharging);
    QFETCH(bool, systemPowerSaveMode);
    QFETCH(QString, mode);

    BatteryStatusIndicator statusIndicator;
    contextPropertyValues.insert("Battery.IsCharging", batteryIsCharging);
    contextPropertyValues.insert("System.PowerSaveMode", systemPowerSaveMode);
    statusIndicator.setMode();
    QCOMPARE(statusIndicator.mode(), mode);
}

void Ut_BatteryStatusIndicator::testSetValue_data()
{
    QTest::addColumn<int>("batteryChargePercentage");
    QTest::addColumn<bool>("batteryIsCharging");
    QTest::addColumn<QStringList>("valueList");
    QTest::addColumn<QString>("value");

    QTest::newRow("Battery not charging, remaining equal to maximum") << 100 << false << (QStringList() << "empty" << "value0" << "value1" << "value2") << "value2";
    QTest::newRow("Battery not charging, remaining less than maximum") << 40 << false << (QStringList() << "empty" << "value0" << "value1" << "value2") << "value1";
    QTest::newRow("Battery not charging, remaining 0") << 0 << false << (QStringList() << "empty" << "value0" << "value1" << "value2") << "empty";
    QTest::newRow("Battery charging, remaining equal to maximum") << 100 << true << (QStringList() << "empty" << "value0" << "value1" << "value2") << "value1";
    QTest::newRow("Battery charging, remaining less than maximum") << 40 << true << (QStringList() << "empty" << "value0" << "value1" << "value2") << "value1";
    QTest::newRow("Battery charging, remaining 0") << 0 << true << (QStringList() << "empty" << "value0" << "value1" << "value2") << "value0";
}

void Ut_BatteryStatusIndicator::testSetValue()
{
    QFETCH(int, batteryChargePercentage);
    QFETCH(bool, batteryIsCharging);
    QFETCH(QStringList, valueList);
    QFETCH(QString, value);

    BatteryStatusIndicator statusIndicator;
    contextPropertyValues.insert("Battery.ChargePercentage", batteryChargePercentage);
    contextPropertyValues.insert("Battery.IsCharging", batteryIsCharging);
    statusIndicator.setValueList(valueList);
    statusIndicator.setValue();
    QCOMPARE(statusIndicator.value(), value);
}

QTEST_MAIN(Ut_BatteryStatusIndicator)
