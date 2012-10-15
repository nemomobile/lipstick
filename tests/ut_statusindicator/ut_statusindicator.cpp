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
#include <QTimeLine>
#include <contextproperty.h>
#include "ut_statusindicator.h"
#include "statusindicator.h"

// QTimeLine stubs
QTimeLine::State qTimeLineState = QTimeLine::NotRunning;
QTimeLine::State QTimeLine::state() const
{
    return qTimeLineState;
}

void QTimeLine::start()
{
    qTimeLineState = QTimeLine::Running;
}

void QTimeLine::stop()
{
    qTimeLineState = QTimeLine::NotRunning;
}

int qTimeLineDuration = 0;
void QTimeLine::setDuration(int duration)
{
    qTimeLineDuration = duration;
}

int qTimeLineUpdateInterval = 0;
void QTimeLine::setUpdateInterval(int interval)
{
    qTimeLineUpdateInterval = interval;
}

int qTimeLineStartFrame = 0;
int qTimeLineEndFrame = 0;
void QTimeLine::setFrameRange(int startFrame, int endFrame)
{
    qTimeLineStartFrame = startFrame;
    qTimeLineEndFrame = endFrame;
}

// ContextProperty stubs
QSet<const ContextProperty *> contextPropertySubscriptions;
void ContextProperty::subscribe() const
{
    contextPropertySubscriptions.insert(this);
}

void ContextProperty::unsubscribe() const
{
    contextPropertySubscriptions.remove(this);
}

void Ut_StatusIndicator::testConstruction()
{
    QObject parent;
    StatusIndicator statusIndicator(&parent);
    QCOMPARE(statusIndicator.parent(), &parent);
    QCOMPARE(statusIndicator.animationTimeLine->curveShape(), QTimeLine::LinearCurve);
    QCOMPARE(statusIndicator.animationTimeLine->loopCount(), 0);
    QCOMPARE(disconnect(statusIndicator.animationTimeLine, SIGNAL(frameChanged(int)), &statusIndicator, SLOT(setCurrentValueIndex(int))), true);
}

void Ut_StatusIndicator::testGettersAndSetters()
{
    StatusIndicator statusIndicator;
    QString mode("testMode");
    QString value("testValue");
    QStringList valueList("testValue1 testValue2");
    int animationDuration = 123;
    bool isOnDisplay = false;

    QCOMPARE(statusIndicator.mode(), QString());
    statusIndicator.setMode(mode);
    QCOMPARE(statusIndicator.mode(), mode);

    QCOMPARE(statusIndicator.value(), QString());
    statusIndicator.setValue(value);
    QCOMPARE(statusIndicator.value(), value);

    QCOMPARE(statusIndicator.valueList(), QStringList());
    statusIndicator.setValueList(valueList);
    QCOMPARE(statusIndicator.valueList(), valueList);

    QCOMPARE(statusIndicator.animationDuration(), 0);
    statusIndicator.setAnimationDuration(animationDuration);
    QCOMPARE(statusIndicator.animationDuration(), animationDuration);

    QCOMPARE(statusIndicator.isOnDisplay(), true);
    statusIndicator.setIsOnDisplay(isOnDisplay);
    QCOMPARE(statusIndicator.isOnDisplay(), isOnDisplay);
}

void Ut_StatusIndicator::testPropertyNotificationSignals_data()
{
    QTest::addColumn<bool>("isOnDisplay");
    QTest::addColumn<int>("signalCount");

    QTest::newRow("Status indicator is on display") << true << 1;
    QTest::newRow("Status indicator is not on display") << false << 0;
}

void Ut_StatusIndicator::testPropertyNotificationSignals()
{
    QFETCH(bool, isOnDisplay);
    QFETCH(int, signalCount);

    StatusIndicator statusIndicator;
    QSignalSpy isOnDisplaySpy(&statusIndicator, SIGNAL(isOnDisplayChanged()));
    statusIndicator.setIsOnDisplay(isOnDisplay);
    QCOMPARE(isOnDisplaySpy.count(), 1);

    QSignalSpy modeChangedSpy(&statusIndicator, SIGNAL(modeChanged()));
    QSignalSpy valueChangedSpy(&statusIndicator, SIGNAL(valueChanged()));
    QSignalSpy animationDurationChangedSpy(&statusIndicator, SIGNAL(animationDurationChanged()));
    statusIndicator.setMode("testMode");
    QCOMPARE(modeChangedSpy.count(), signalCount);
    statusIndicator.setValue("testValue");
    QCOMPARE(valueChangedSpy.count(), signalCount);
    statusIndicator.setValueList(QStringList() << "testValue1" << "testValue2");
    QCOMPARE(valueChangedSpy.count(), 2 * signalCount);
    statusIndicator.setAnimationDuration(123);
    QCOMPARE(animationDurationChangedSpy.count(), signalCount);
}

void Ut_StatusIndicator::testSettingInitialValueIndexSetsValueAndStaysWithinBounds()
{
    StatusIndicator statusIndicator;
    statusIndicator.setValueList(QStringList() << "value0" << "value1");
    QCOMPARE(statusIndicator.value(), QString("value0"));
    statusIndicator.setInitialValueIndex(1);
    QCOMPARE(statusIndicator.value(), QString("value1"));
    statusIndicator.setInitialValueIndex(-1);
    QCOMPARE(statusIndicator.value(), QString("value0"));
    statusIndicator.setInitialValueIndex(2);
    QCOMPARE(statusIndicator.value(), QString("value1"));
    statusIndicator.setValueList(QStringList() << "newValue0" << "newValue1");
    QCOMPARE(statusIndicator.value(), QString("newValue1"));
    statusIndicator.setValueList(QStringList() << "value");
    QCOMPARE(statusIndicator.value(), QString("value"));
}

void Ut_StatusIndicator::testSettingAnimationDurationSetsTimelineState()
{
    StatusIndicator statusIndicator;
    statusIndicator.setValueList(QStringList() << "value0" << "value1" << "value2" << "value3");
    statusIndicator.setAnimationDuration(4000);
    QCOMPARE(qTimeLineStartFrame, 0);
    QCOMPARE(qTimeLineEndFrame, 4);
    QCOMPARE(qTimeLineDuration, 4000);
    QCOMPARE(qTimeLineUpdateInterval, 1000);
    QCOMPARE(qTimeLineState, QTimeLine::Running);

    statusIndicator.setAnimationDuration(0);
    QCOMPARE(qTimeLineState, QTimeLine::NotRunning);
}

void Ut_StatusIndicator::testAnimationIsDisabledWhenNotOnDisplay()
{
    StatusIndicator statusIndicator;
    statusIndicator.setIsOnDisplay(false);
    statusIndicator.setValueList(QStringList() << "value0" << "value1" << "value2" << "value3");
    statusIndicator.setAnimationDuration(4000);
    QCOMPARE(qTimeLineState, QTimeLine::NotRunning);

    statusIndicator.setIsOnDisplay(true);
    QCOMPARE(qTimeLineState, QTimeLine::Running);
}

void Ut_StatusIndicator::testAnimationIsDisabledWhenNotEnoughValuesToAnimate()
{
    StatusIndicator statusIndicator;
    statusIndicator.setValueList(QStringList() << "value0");
    statusIndicator.setAnimationDuration(4000);
    QCOMPARE(qTimeLineState, QTimeLine::NotRunning);
}

void Ut_StatusIndicator::testSettingInitialValueIndexSetsAnimationLength()
{
    StatusIndicator statusIndicator;
    statusIndicator.setValueList(QStringList() << "value0" << "value1" << "value2" << "value3");
    statusIndicator.setAnimationDuration(4000);
    statusIndicator.setInitialValueIndex(1);
    QCOMPARE(qTimeLineStartFrame, 1);
    QCOMPARE(qTimeLineEndFrame, 4);
    QCOMPARE(qTimeLineDuration, 3000);
    QCOMPARE(qTimeLineUpdateInterval, 1000);
    QCOMPARE(qTimeLineState, QTimeLine::Running);
}

void Ut_StatusIndicator::testContextPropertyCreation()
{
    StatusIndicator statusIndicator;
    ContextProperty *property1 = statusIndicator.createContextProperty("testKey1");
    QCOMPARE(disconnect(&statusIndicator, SIGNAL(subscriptionMade()), property1, SIGNAL(valueChanged())), true);
    QCOMPARE(contextPropertySubscriptions.contains(property1), true);
    QCOMPARE(property1->signalsBlocked(), false);

    statusIndicator.setIsOnDisplay(false);
    ContextProperty *property2 = statusIndicator.createContextProperty("testKey2");
    QCOMPARE(disconnect(&statusIndicator, SIGNAL(subscriptionMade()), property2, SIGNAL(valueChanged())), true);
    QCOMPARE(contextPropertySubscriptions.contains(property2), false);
    QCOMPARE(property2->signalsBlocked(), true);
}

void Ut_StatusIndicator::testSettingIsOnDisplaySubscribesContextProperties_data()
{
    QTest::addColumn<bool>("wasOnDisplay");
    QTest::addColumn<bool>("isOnDisplay");
    QTest::addColumn<bool>("subscribed");
    QTest::addColumn<bool>("signalsBlocked");
    QTest::addColumn<int>("subscriptionMadeCount");
    QTest::addColumn<int>("modeChangedCount");
    QTest::addColumn<int>("valueChangedCount");
    QTest::addColumn<int>("isOnDisplayChangedCount");

    QTest::newRow("Status indicator is on display") << false << true << true << false << 1 << 1 << 1 << 1;
    QTest::newRow("Status indicator is not on display") << true << false << false << true << 0 << 0 << 0 << 1;
}

void Ut_StatusIndicator::testSettingIsOnDisplaySubscribesContextProperties()
{
    QFETCH(bool, wasOnDisplay);
    QFETCH(bool, isOnDisplay);
    QFETCH(bool, subscribed);
    QFETCH(bool, signalsBlocked);
    QFETCH(int, subscriptionMadeCount);
    QFETCH(int, modeChangedCount);
    QFETCH(int, valueChangedCount);
    QFETCH(int, isOnDisplayChangedCount);

    StatusIndicator statusIndicator;
    statusIndicator.setIsOnDisplay(wasOnDisplay);
    ContextProperty *property1 = statusIndicator.createContextProperty("testKey1");
    ContextProperty *property2 = statusIndicator.createContextProperty("testKey2");

    QSignalSpy subscriptionMadeSpy(&statusIndicator, SIGNAL(subscriptionMade()));
    QSignalSpy modeChangedSpy(&statusIndicator, SIGNAL(modeChanged()));
    QSignalSpy valueChangedSpy(&statusIndicator, SIGNAL(valueChanged()));
    QSignalSpy isOnDisplayChangedSpy(&statusIndicator, SIGNAL(isOnDisplayChanged()));
    statusIndicator.setIsOnDisplay(isOnDisplay);
    QCOMPARE(contextPropertySubscriptions.contains(property1), subscribed);
    QCOMPARE(contextPropertySubscriptions.contains(property2), subscribed);
    QCOMPARE(property1->signalsBlocked(), signalsBlocked);
    QCOMPARE(property2->signalsBlocked(), signalsBlocked);
    QCOMPARE(subscriptionMadeSpy.count(), subscriptionMadeCount);
    QCOMPARE(modeChangedSpy.count(), modeChangedCount);
    QCOMPARE(valueChangedSpy.count(), valueChangedCount);
    QCOMPARE(isOnDisplayChangedSpy.count(), isOnDisplayChangedCount);
}

QTEST_MAIN(Ut_StatusIndicator)
