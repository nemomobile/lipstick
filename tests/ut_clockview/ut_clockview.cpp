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
#include "ut_clockview.h"
#include "clockview.h"
#include "clock_stub.h"
#include <DuiApplication>
#include <DuiLabel>
#include <QGraphicsLayout>
#include <QGraphicsLinearLayout>


// DuiWidgetController stubs
ClockModel clockModel;
DuiWidgetModel *DuiWidgetController::model()
{
    return &clockModel;
}
const DuiWidgetModel *DuiWidgetController::model() const
{
    return &clockModel;
}

// QGraphicsLinearLayout stubs (prevents crashing)
void QGraphicsLinearLayout::insertItem(int , QGraphicsLayoutItem *)
{
}

// DuiStyleContainer stubs
ClockStyleContainer clockStyleContainer;
ClockStyle *clockStyle = 0;
const DuiStyle *DuiStyleContainer::currentStyle() const
{
    return clockStyle;
}

// DuiWidgetStyleContainer stubs (prevents crashing)
void DuiWidgetStyleContainer::reloadStyles()
{
}

// DuiWidgetView stubs
DuiWidgetStyleContainer &DuiWidgetView::style()
{
    return clockStyleContainer;
}

const DuiWidgetStyleContainer &DuiWidgetView::style() const
{
    return clockStyleContainer;
}

// DuiFeedback stubs (prevents crashing)
DuiFeedback::DuiFeedback(QObject *) : d_ptr(0)
{
}

void DuiLabel::setText(const QString &text)
{
    Ut_ClockView::timeAsString = text;
}

QString Ut_ClockView::timeAsString;

// Called before the first testfunction is executed
void Ut_ClockView::initTestCase()
{
    clockStyle = new ClockStyle();
}

// Called after the last testfunction was executed
void Ut_ClockView::cleanupTestCase()
{
}

// Called before each testfunction is executed
void Ut_ClockView::init()
{
    testClock = new Clock();
    m_subject = new ClockView(testClock);
}

// Called after every testfunction
void Ut_ClockView::cleanup()
{
    delete testClock;  // this also deletes the view
    testClock = NULL;
    m_subject = NULL;
}

void Ut_ClockView::testUpdate()
{
    m_subject->setModel(&clockModel);
    clockStyle->setTimeFormat(QString("HH:mmA"));
    clockModel.setTime(QDateTime(QDate(1, 1, 1), QTime(1, 1)));
    QCOMPARE(Ut_ClockView::timeAsString, QString("01:01AM"));
}

QTEST_MAIN(Ut_ClockView)
