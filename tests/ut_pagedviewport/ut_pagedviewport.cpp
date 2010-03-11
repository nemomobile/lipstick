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
#include "ut_pagedviewport.h"
#include "pagedviewport.h"
#include "switcherphysicsintegrationstrategy.h"

static uint checkSnapInterval = 0;
static uint testPanTargetIndex = 0;

// SwitcherIntegrationStrategy stubs
SwitcherPhysicsIntegrationStrategy::SwitcherPhysicsIntegrationStrategy()
{
    snapInterval_ = 0;
    autoPanMode = false;
}

SwitcherPhysicsIntegrationStrategy::~SwitcherPhysicsIntegrationStrategy()
{
}

void SwitcherPhysicsIntegrationStrategy::panToItem(uint itemIndex)
{
    testPanTargetIndex = itemIndex;
    autoPanTargetIndex = itemIndex;
    autoPanMode = true;
}

void SwitcherPhysicsIntegrationStrategy::integrate(qreal &, qreal &, qreal &, qreal &, qreal, qreal, IntegrationData&)
{
    emit snapIndexChanged(testPanTargetIndex);
}


void SwitcherPhysicsIntegrationStrategy::setSnapInterval(uint newSnapInterval)
{
    checkSnapInterval = newSnapInterval;
}

uint SwitcherPhysicsIntegrationStrategy::snapInterval() const
{
    return 0;
}

void SwitcherPhysicsIntegrationStrategy::snapIntegrate(qreal &, qreal &, qreal &, qreal &, qreal, qreal, DuiPhysics2DIntegrationStrategy::IntegrationData &)
{
}

void SwitcherPhysicsIntegrationStrategy::autoPanIntegrate(qreal &, qreal &, qreal &, qreal &, DuiPhysics2DIntegrationStrategy::IntegrationData &)
{
}

/*
 * setPosition of DuiPhysics2DPanning class is stubbed here to call integrator() method
 * from same class. integrator() calls integrate method from set integrations strategy class.
 * In this case it's SwitcherPhysicsIntegrationStrategy. In these tests integrate() method
 * of SwitcherPhysicsIntegrationStrategy class is stubbed to emit snapIndexChanged signal
 * that is monitored in PagedViewport class and in these tests.
 */
void DuiPhysics2DPanning::setPosition(const QPointF &position)
{
    Q_UNUSED(position)
    // integrator() is called with attribute 1 for it to call integrate() method.
    integrator(1);
}

void Ut_PagedViewport::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_pagedviewport" };
    app = new DuiApplication(argc, app_name);
}

void Ut_PagedViewport::cleanupTestCase()
{
    delete app;
}

void Ut_PagedViewport::init()
{
    m_subject = new PagedViewport();
}

void Ut_PagedViewport::cleanup()
{
    delete m_subject;
}

void Ut_PagedViewport::test_updatePageWidth()
{
    checkSnapInterval = 0;
    uint testValue = 12;
    m_subject->updatePageWidth(testValue);

    QCOMPARE(testValue, checkSnapInterval);
}

void Ut_PagedViewport::test_panToPage()
{
    QSignalSpy spy(m_subject, SIGNAL(pageChanged(int)));
    QGraphicsWidget *widget = new QGraphicsWidget;
    widget->setMinimumSize(QSize(100, 1000));
    widget->setMaximumSize(QSize(100, 1000));

    m_subject->setWidget(widget);
    m_subject->updatePageWidth(100);

    m_subject->panToPage(1);

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();

    QVERIFY(arguments.at(0).toInt() == 1);
}

QTEST_APPLESS_MAIN(Ut_PagedViewport)
