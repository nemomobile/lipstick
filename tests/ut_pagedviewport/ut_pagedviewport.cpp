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
#include "ut_pagedviewport.h"
#include "pagedviewport.h"
#include "pagedviewportview.h"
#include "pagedpanning.h"
#include "pagepositionindicator.h"

static uint checkPageCount = 0;
static uint testPanTargetPage = 0;

void MPannableViewport::setWidget(QGraphicsWidget*)
{

}

PagedPanning::PagedPanning(QObject* parent) : MPhysics2DPanning(parent),
                                              pageCount_(1),
                                              currentPage(0),
                                              snapMode(false),
                                              velocityThreshold_(7.0),
                                              dragThreshold_(0.5),
                                              pageSnapSpringK_(0.7),
                                              pageSnapFriction_(0.7),
                                              previousPosition(0),
                                              targetPage(0),
                                              pageWidth_(0)
{
}

PagedPanning::~PagedPanning()
{
}

void PagedPanning::panToPage(int page)
{
    testPanTargetPage = page;
    targetPage = page;
    snapMode = true;
    emit pageChanged(page);
}

void PagedPanning::panToCurrentPage()
{
    emit pageChanged(currentPage);
}

void PagedPanning::integrateAxis(Qt::Orientation, qreal &, qreal &, qreal &, qreal &, bool)
{
    emit pageChanged(testPanTargetPage);
}


void PagedPanning::setPageCount(int pageCount)
{
    checkPageCount = pageCount;
}

int PagedPanning::pageCount() const
{
    return checkPageCount;
}

void PagedPanning::setVelocityThreshold(qreal)
{
}

void PagedPanning::setDragThreshold(qreal)
{
}

void PagedPanning::setSlideLimit(int)
{
}

void PagedPanning::setPageSnapSpringK(qreal)
{
}

void PagedPanning::setPageSnapFriction(qreal)
{
}

void PagedPanning::pointerPress(const QPointF &pos)
{
    Q_UNUSED(pos);
}

void PagedPanning::pointerMove(const QPointF &pos)
{
    Q_UNUSED(pos);
}

void PagedPanning::pointerRelease()
{
}

int PagedPanning::activePage() const
{
    return currentPage;
}

float PagedPanning::pageWidth() const
{
    return 0;
}

uint gPagedPanningSetPage = -1;

void PagedPanning::setPage(uint page)
{
    gPagedPanningSetPage = page;
}

void PagedPanning::setPanTreshold(qreal value)
{
    Q_UNUSED(value)
}

void Ut_PagedViewport::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_pagedviewport" };
    app = new MApplication(argc, app_name);
}

void Ut_PagedViewport::cleanupTestCase()
{
    delete app;
}

void Ut_PagedViewport::init()
{
    m_subject = new PagedViewport(NULL);
    gPagedPanningSetPage = -1;
}

void Ut_PagedViewport::cleanup()
{
    delete m_subject;
}

void Ut_PagedViewport::test_updatePageCount()
{
    checkPageCount = 0;
    uint target = 12;
    m_subject->updatePageCount(target);

    QCOMPARE(target, checkPageCount);
}

void Ut_PagedViewport::test_panToPage()
{
    QSignalSpy spy(m_subject, SIGNAL(pageChanged(int)));
    QGraphicsWidget *widget = new QGraphicsWidget;
    widget->setMinimumSize(QSize(100, 1000));
    widget->setMaximumSize(QSize(100, 1000));

    m_subject->setWidget(widget);
    m_subject->updatePageCount(10);

    m_subject->panToPage(1);

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();

    QVERIFY(arguments.at(0).toInt() == 1);
}

void Ut_PagedViewport::test_setPage()
{
    m_subject->setPage(5);
    QCOMPARE(gPagedPanningSetPage, uint(5));
}

void Ut_PagedViewport::test_focusFirstPage()
{
    m_subject->focusFirstPage();
    QCOMPARE(gPagedPanningSetPage, uint(0));
}

QTEST_APPLESS_MAIN(Ut_PagedViewport)
