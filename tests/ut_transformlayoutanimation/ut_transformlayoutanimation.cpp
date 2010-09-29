/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of mhome.
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
#include <MApplication>
#include <MGridLayoutPolicy>
#include <MLinearLayoutPolicy>
#include <QGraphicsLayout>
#include <QGraphicsLinearLayout>
#include "ut_transformlayoutanimation.h"
#include "transformlayoutanimation.h"
#include <QGestureEvent>
#include <QPinchGesture>


QList<QGraphicsLayoutItem*> items;

QGraphicsLayoutItem *TestLayout::itemAt(int index) const
{
    return items[index];
}

QGraphicsWidget *gAnimationCreatedForWidget;
TransformAnimation *TestTransformLayoutAnimation::createAnimation(QGraphicsWidget *widget)
{
    gAnimationCreatedForWidget = widget;
    return TransformLayoutAnimation::createAnimation(widget);
}

void Ut_TransformLayoutAnimation::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_transformlayoutanimation";
    app = new MApplication(argc, &app_name);
}

void Ut_TransformLayoutAnimation::cleanupTestCase()
{
    delete app;
}

void Ut_TransformLayoutAnimation::init()
{
    layout = new TestLayout();
    m_subject = new TestTransformLayoutAnimation(layout);
    m_subject->modifiableStyle()->setSpeedSmoothness(0.7);
    gAnimationCreatedForWidget = NULL;
}

void Ut_TransformLayoutAnimation::cleanup()
{
    delete m_subject;
    qDeleteAll(items);
    items.clear();
}

void Ut_TransformLayoutAnimation::testItemAddingInitialGeometry()
{
    QGraphicsWidget *widget1 = new QGraphicsWidget();
    QGraphicsWidget *widget2 = new QGraphicsWidget();
    widget1->setVisible(false);
    widget2->setVisible(true);
    items.append(widget1);
    items.append(widget2);

    QRectF itemGeometry1(11, 22, 33, 44);
    QRectF itemGeometry2(44, 33, 22, 11);

    m_subject->itemAddedToLayout(0);
    m_subject->itemAddedToLayout(1);
    m_subject->setItemGeometry(0, itemGeometry1);
    m_subject->setItemGeometry(1, itemGeometry2);
    QCOMPARE(widget1->geometry(), itemGeometry1);
    QCOMPARE(widget2->geometry(), itemGeometry2);
    QVERIFY(widget1->isVisible());
    QVERIFY(widget2->isVisible());
}

void Ut_TransformLayoutAnimation::testAddingNonWidgets()
{
    QGraphicsLinearLayout *item1 = new QGraphicsLinearLayout();
    QGraphicsWidget *widget1 = new QGraphicsWidget();
    items.append(item1);
    items.append(widget1);
    m_subject->itemAddedToLayout(0);
    m_subject->itemAddedToLayout(1);

    QRectF itemGeometry1(11, 22, 33, 44);
    // doesn't do anything for the item at index 0, but sets the geometry of widget at index 1
    m_subject->setItemGeometry(0, itemGeometry1);
    m_subject->setItemGeometry(1, itemGeometry1);
    QCOMPARE(widget1->geometry(), itemGeometry1);
}

void Ut_TransformLayoutAnimation::testSetGeometryAfterItemRemoval()
{
    QGraphicsWidget *widget1 = new QGraphicsWidget();
    QGraphicsWidget *widget2 = new QGraphicsWidget();
    items.append(widget1);
    items.append(widget2);

    QRectF itemGeometry1(11, 22, 33, 44);

    m_subject->itemAddedToLayout(0);
    m_subject->itemAddedToLayout(1);

    m_subject->itemRemovedFromLayout(0);
    m_subject->setItemGeometry(0, itemGeometry1);
    QCOMPARE(widget1->geometry(), QRectF());
    QCOMPARE(widget2->geometry(), itemGeometry1);
}

bool fuzzyCompare(const QTransform &t1, const QTransform &t2)
{
    float threshold = 0.0001f;
    return (qAbs(t1.m11() - t2.m11()) < threshold) &&
            (qAbs(t1.m12() - t2.m12()) < threshold) &&
            (qAbs(t1.m13() - t2.m13()) < threshold) &&
            (qAbs(t1.m21() - t2.m21()) < threshold) &&
            (qAbs(t1.m22() - t2.m22()) < threshold) &&
            (qAbs(t1.m23() - t2.m23()) < threshold) &&
            (qAbs(t1.m31() - t2.m31()) < threshold) &&
            (qAbs(t1.m32() - t2.m32()) < threshold) &&
            (qAbs(t1.m33() - t2.m33()) < threshold);
}

void Ut_TransformLayoutAnimation::testTransformAnimation()
{
    QGraphicsWidget *widget1 = new QGraphicsWidget();
    items.append(widget1);
    m_subject->itemAddedToLayout(0);

    QRectF itemGeometry1(11, 22, 33, 44);
    QRectF itemGeometry2(0, 0, 0, 0);

    // animate from zero geometry to itemGeometry1
    m_subject->setItemGeometry(0, itemGeometry1);
    QCOMPARE(widget1->geometry(), itemGeometry1);
    m_subject->setManualControl(true);
    m_subject->start();
    QCOMPARE(widget1->transform(), QTransform(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -itemGeometry1.x(), -itemGeometry1.y(), 1.0f));
    m_subject->setProgress(0.5);
    m_subject->setCurrentTime(0);
    QVERIFY(fuzzyCompare(widget1->transform(), QTransform(0.5f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, -itemGeometry1.x() / 2.0f, -itemGeometry1.y() / 2.0f, 1.0f)));
    m_subject->setProgress(1.0);
    m_subject->setCurrentTime(0);
    QVERIFY(fuzzyCompare(widget1->transform(), QTransform(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f)));
    m_subject->stop();

    // animate back from itemGeometry1 to zero geometry
    m_subject->setItemGeometry(0, itemGeometry2);
    QCOMPARE(widget1->geometry(), itemGeometry2);
    m_subject->setManualControl(true);
    m_subject->start();
    m_subject->setProgress(0.0);
    m_subject->setCurrentTime(0);
    QVERIFY(fuzzyCompare(widget1->transform(), QTransform(itemGeometry1.width(), 0.0f, 0.0f, 0.0f, itemGeometry1.height(), 0.0f, itemGeometry1.x(), itemGeometry1.y(), 1.0f)));
    m_subject->setProgress(0.5);
    m_subject->setCurrentTime(0);
    QVERIFY(fuzzyCompare(widget1->transform(), QTransform(itemGeometry1.width() / 2.0f, 0.0f, 0.0f, 0.0f, itemGeometry1.height() / 2.0f, 0.0f, itemGeometry1.x() / 2.0f, itemGeometry1.y() / 2.0f, 1.0f)));
    m_subject->setProgress(1.0);
    m_subject->setCurrentTime(0);
    QVERIFY(fuzzyCompare(widget1->transform(), QTransform(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f)));
}

void Ut_TransformLayoutAnimation::testContainerAnimation()
{
    QGraphicsWidget *container = static_cast<QGraphicsWidget*>(layout->parentLayoutItem());
    QGraphicsWidget *widget1 = new QGraphicsWidget();
    items.append(widget1);
    m_subject->itemAddedToLayout(0);

    QRectF sourceGeometry(0, 0, 10, 10);
    container->setGeometry(sourceGeometry);

    // animate container x position from 0 to 100
    m_subject->setManualControl(true);
    m_subject->start();
    m_subject->setProgress(0.0);
    m_subject->setCurrentTime(0);
    container->setGeometry(100, 0, 10, 10);
    QCOMPARE(container->transform().dx(), qreal(sourceGeometry.x() - container->geometry().x()));
    m_subject->setProgress(0.5);
    m_subject->setCurrentTime(0);
    QCOMPARE(container->transform().dx(), qreal(sourceGeometry.x() - container->geometry().x())  * 0.5f);
    m_subject->setProgress(1.0);
    m_subject->setCurrentTime(0);
    QCOMPARE(container->transform().dx(), qreal(0.0f));
    m_subject->stop();

    // animate container x position from 100 to 0
    sourceGeometry = container->geometry();
    m_subject->setManualControl(true);
    m_subject->start();
    m_subject->setProgress(0.0);
    m_subject->setCurrentTime(0);
    container->setGeometry(0, 0, 10, 10);
    QCOMPARE(container->transform().dx(), qreal(sourceGeometry.x() - container->geometry().x()));
    m_subject->setProgress(0.5);
    m_subject->setCurrentTime(0);
    QCOMPARE(container->transform().dx(), qreal(sourceGeometry.x() - container->geometry().x()) * 0.5f);
    m_subject->setProgress(1.0);
    m_subject->setCurrentTime(0);
    QCOMPARE(container->transform().dx(), qreal(0.0f));
}

void Ut_TransformLayoutAnimation::testCancelingAnimation()
{
    QSignalSpy spy(m_subject, SIGNAL(willFinish()));
    m_subject->setManualControl(true);
    m_subject->start();
    m_subject->setProgress(0.5);
    m_subject->setCurrentTime(0);
    m_subject->setManualControl(false);
    m_subject->cancelAnimation();

    float lastProgress = m_subject->progress();
    do {
        m_subject->setCurrentTime(0);
        QVERIFY(m_subject->progress() < lastProgress);
    } while(m_subject->progress() > 0.0f);

    QCOMPARE(spy.count(), 1);
}

void Ut_TransformLayoutAnimation::testManualControl()
{
    QSignalSpy spy(m_subject, SIGNAL(willFinish()));
    m_subject->setManualControl(true);
    m_subject->start();
    m_subject->setProgress(0.5);
    m_subject->setCurrentTime(0);
    QCOMPARE(m_subject->progress(), 0.5f);

    m_subject->setManualControl(false);
    float lastProgress = m_subject->progress();
    do {
        m_subject->setCurrentTime(0);
        QVERIFY(m_subject->progress() > lastProgress);
    } while(m_subject->progress() < 1.0f);

    QCOMPARE(spy.count(), 1);
}

void Ut_TransformLayoutAnimation::testAnimationCreation()
{
    QGraphicsWidget *widget1 = new QGraphicsWidget();
    items.append(widget1);

    m_subject->itemAddedToLayout(0);
    QCOMPARE(gAnimationCreatedForWidget, widget1);
}

QTEST_APPLESS_MAIN(Ut_TransformLayoutAnimation)
