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
#include "pagedpanning.h"

static uint checkPageWidth = 0;
static uint testPanTargetPage = 0;

PagedPanning::PagedPanning(QObject* parent) : DuiPhysics2DPanning(parent),
					      pageWidth_(0),
					      currentPage(0),
					      autoIntegrateMode(false),
					      autoIntegrateTargetPage(0)
{

}

PagedPanning::~PagedPanning()
{
}

void PagedPanning::panToPage(uint page)
{
    testPanTargetPage = page;
    autoIntegrateTargetPage = page;
    autoIntegrateMode = true;
    emit pageChanged(page);
}

void PagedPanning::integrateAxis(Qt::Orientation, qreal &, qreal &, qreal &, qreal &, bool)
{
    emit pageChanged(testPanTargetPage);
}


void PagedPanning::setPageWidth(uint pageWidth)
{
    checkPageWidth = pageWidth;
}

uint PagedPanning::pageWidth() const
{
    return checkPageWidth;
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
    m_subject = new PagedViewport(NULL);
}

void Ut_PagedViewport::cleanup()
{
    delete m_subject;
}

void Ut_PagedViewport::test_updatePageWidth()
{
    checkPageWidth = 0;
    uint target = 12;
    m_subject->updatePageWidth(target);

    QCOMPARE(target, checkPageWidth);
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

    QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();

    QVERIFY(arguments.at(0).toInt() == 1);
}

QTEST_APPLESS_MAIN(Ut_PagedViewport)
