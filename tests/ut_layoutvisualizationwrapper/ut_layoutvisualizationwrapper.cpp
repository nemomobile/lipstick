/*
 * This file is part of mhome
 *
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * Contact: Tomas Junnonen <tomas.junnonen@nokia.com>
 *
 * This software, including documentation, is protected by copyright
 * controlled by Nokia Corporation. All rights are reserved. Copying,
 * including reproducing, storing, adapting or translating, any or all of
 * this material requires the prior written consent of Nokia Corporation.
 * This material also contains confidential information which may not be
 * disclosed to others without the prior written consent of Nokia.
 */

#include <QtTest/QtTest>
#include "ut_layoutvisualizationwrapper.h"
#include "layoutvisualizationwrapper.h"

#include <QGraphicsLinearLayout>
#include <QGraphicsWidget>


const qreal WIDGET_WIDTH = 10.0;
const qreal WIDGET_HEIGHT = 20.0;
const qreal ORIGO = 0.0;
const int NUMBER_OF_WIDGETS = 3;
const qreal PARENT_WIDGET_WIDTH = NUMBER_OF_WIDGETS * WIDGET_WIDTH;

void Ut_LayoutVisualizationWrapper::initTestCase()
{
    m_subject = NULL;
    testableLayout = NULL;
    testableWidget = NULL;
}

void Ut_LayoutVisualizationWrapper::cleanupTestCase()
{
}

void Ut_LayoutVisualizationWrapper::init()
{
    buildTestableWidget(Qt::Horizontal);

    m_subject = new LayoutVisualizationWrapper(*testableLayout);
}

void Ut_LayoutVisualizationWrapper::cleanup()
{
    delete m_subject;
    m_subject = NULL;
    delete testableWidget;
    testableWidget = NULL;
}

void Ut_LayoutVisualizationWrapper::buildTestableWidget(Qt::Orientation orientation)
{
    testableLayout = new QGraphicsLinearLayout(orientation);
    testableLayout->setContentsMargins(0.0, 0.0, 0.0, 0.0);
    testableLayout->setSpacing(0.0);

    for (int i = 0; i < NUMBER_OF_WIDGETS; ++i) {
        QGraphicsWidget *widget = new QGraphicsWidget();
        widget->setPreferredSize(WIDGET_WIDTH, WIDGET_HEIGHT);
        testableLayout->addItem(widget);
    }

    delete testableWidget;
    testableWidget = new QGraphicsWidget();
    testableWidget->setLayout(testableLayout);
    testableWidget->setGeometry(ORIGO, ORIGO, PARENT_WIDGET_WIDTH, WIDGET_HEIGHT);
}

qreal Ut_LayoutVisualizationWrapper::firstWidgetXTranslation() const
{
    return static_cast<QGraphicsWidget*>(testableWidget->layout()->itemAt(0))->transform().dx();
}

QGraphicsWidget* Ut_LayoutVisualizationWrapper::getLastWidget() const
{
    return static_cast<QGraphicsWidget*>(testableWidget->layout()->itemAt(NUMBER_OF_WIDGETS - 1));
}

qreal Ut_LayoutVisualizationWrapper::lastWidgetXTranslation() const
{
    return getLastWidget()->transform().dx();
}

void Ut_LayoutVisualizationWrapper::testSettingRightmostItemToAppearOnTheLeftSide()
{
    m_subject->setWrappingMode(LayoutVisualizationWrapper::WrapRightEdgeToLeft);
    QCOMPARE(lastWidgetXTranslation(), -PARENT_WIDGET_WIDTH);
}

void Ut_LayoutVisualizationWrapper::testSettingLeftmostItemToAppearOnTheRightSide()
{
    m_subject->setWrappingMode(LayoutVisualizationWrapper::WrapLeftEdgeToRight);
    QCOMPARE(firstWidgetXTranslation(), PARENT_WIDGET_WIDTH);
}

void Ut_LayoutVisualizationWrapper::testChangingWrappingModeFromLeftToNoneSetsTransformationsCorrectly()
{
    m_subject->setWrappingMode(LayoutVisualizationWrapper::WrapRightEdgeToLeft);
    m_subject->setWrappingMode(LayoutVisualizationWrapper::NoWrap);
    QCOMPARE(firstWidgetXTranslation(), 0.0);
    QCOMPARE(lastWidgetXTranslation(), 0.0);
}

void Ut_LayoutVisualizationWrapper::testChangingWrappingModeFromRightToNoneSetsTransformationsCorrectly()
{
    m_subject->setWrappingMode(LayoutVisualizationWrapper::WrapLeftEdgeToRight);
    m_subject->setWrappingMode(LayoutVisualizationWrapper::NoWrap);
    QCOMPARE(firstWidgetXTranslation(), 0.0);
    QCOMPARE(lastWidgetXTranslation(), 0.0);
}

void Ut_LayoutVisualizationWrapper::testChangingWrappingModeFromLeftToRightSetsTransformationsCorrectly()
{
    m_subject->setWrappingMode(LayoutVisualizationWrapper::WrapRightEdgeToLeft);
    m_subject->setWrappingMode(LayoutVisualizationWrapper::WrapLeftEdgeToRight);
    QCOMPARE(firstWidgetXTranslation(), PARENT_WIDGET_WIDTH);
    QCOMPARE(lastWidgetXTranslation(), 0.0);
}

void Ut_LayoutVisualizationWrapper::testChangingWrappingModeFromRightToLeftSetsTransformationsCorrectly()
{
    m_subject->setWrappingMode(LayoutVisualizationWrapper::WrapLeftEdgeToRight);
    m_subject->setWrappingMode(LayoutVisualizationWrapper::WrapRightEdgeToLeft);
    QCOMPARE(firstWidgetXTranslation(), 0.0);
    QCOMPARE(lastWidgetXTranslation(), -PARENT_WIDGET_WIDTH);
}

void Ut_LayoutVisualizationWrapper::testParentSizeChangeUpdatesTransformationsCorrectly()
{
    m_subject->setWrappingMode(LayoutVisualizationWrapper::WrapRightEdgeToLeft);
    QCOMPARE(lastWidgetXTranslation(), -PARENT_WIDGET_WIDTH);

    //Simulate size change by swapping height and width
    testableWidget->setGeometry(ORIGO, ORIGO, WIDGET_HEIGHT, PARENT_WIDGET_WIDTH);
    m_subject->setWrappingMode(LayoutVisualizationWrapper::WrapRightEdgeToLeft);
    QCOMPARE(lastWidgetXTranslation(), -WIDGET_HEIGHT);
    //Reset transformation
    getLastWidget()->setTransform(QTransform::fromTranslate(0.0, 0.0));
    m_subject->setWrappingMode(LayoutVisualizationWrapper::WrapRightEdgeToLeft);
    //Transformation should not happen when we have not rotated
    QCOMPARE(lastWidgetXTranslation(), 0.0);
}

void Ut_LayoutVisualizationWrapper::testUsingVerticalLayoutDoesNothing()
{
    buildTestableWidget(Qt::Vertical);
    delete m_subject;
    m_subject = new LayoutVisualizationWrapper(*testableLayout);

    m_subject->setWrappingMode(LayoutVisualizationWrapper::WrapRightEdgeToLeft);
    QCOMPARE(firstWidgetXTranslation(), 0.0);
    QCOMPARE(lastWidgetXTranslation(), 0.0);
}

QTEST_MAIN(Ut_LayoutVisualizationWrapper)
