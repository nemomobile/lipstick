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

#include "ut_pagepositionindicatorview.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <MScalableImage>
#include <MPositionIndicator>
#include "pagepositionindicatorview.h"
#include "pagepositionindicatorstyle.h"
#include "pagepositionindicator.h"
#include "pagepositionindicatormodel.h"

QList<const MScalableImage *> Ut_PagePositionIndicatorView::drawnScalableImages;

MScalableImage *focusedIndicator;
MScalableImage *unfocusedIndicator;

class TestPagePositionIndicatorView : public PagePositionIndicatorView
{
    M_VIEW(PagePositionIndicatorModel, PagePositionIndicatorStyle)
public:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0)
    {
        Q_UNUSED(widget)
        drawContents(painter, option);
    }

    TestPagePositionIndicatorView(PagePositionIndicator *cont)
        : PagePositionIndicatorView(cont)
    {
    }

    PagePositionIndicatorStyle *modifiableStyle() {
        PagePositionIndicatorStyleContainer &sc = style();
        const PagePositionIndicatorStyle *const_s = sc.operator ->();
        PagePositionIndicatorStyle *s = const_cast<PagePositionIndicatorStyle *>(const_s);
        return s;
    }

    PagePositionIndicatorStyleContainer& styleContainer() {
        return style();
    }
};

class TestPagePositionIndicator : public PagePositionIndicator
{
public:
    TestPagePositionIndicator()
    {
        view = new TestPagePositionIndicatorView(this);
        setView(view);
        getView()->modifiableStyle()->setUnfocusedIndicatorImage(unfocusedIndicator);
        getView()->modifiableStyle()->setFocusedIndicatorImage(focusedIndicator);
        getView()->modifiableStyle()->setFocusedBetweenPages(false);
    }

    TestPagePositionIndicatorView *getView()
    {
        return view;
    }
private:
    TestPagePositionIndicatorView *view;
};

void Ut_PagePositionIndicatorView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_pagepositionindicatorview" };
    app = new MApplication(argc, app_name);

    focusedIndicator = new MScalableImage(MTheme::pixmap("icon-s-current-page"), 0, 0, 0, 0);
    unfocusedIndicator = new MScalableImage(MTheme::pixmap("icon-s-unselected-page"), 0, 0, 0, 0);
}

void Ut_PagePositionIndicatorView::cleanupTestCase()
{
    delete focusedIndicator;
    delete unfocusedIndicator;

    delete app;
}

void Ut_PagePositionIndicatorView::init()
{
    positionIndicator = new TestPagePositionIndicator();
    m_subject = positionIndicator->getView();
}

void Ut_PagePositionIndicatorView::cleanup()
{
    delete positionIndicator;
}

// MScalableImage stubs
void MScalableImage::draw(int x, int y, int w, int h, QPainter *painter) const
{
    Q_UNUSED(x);
    Q_UNUSED(y);
    Q_UNUSED(w);
    Q_UNUSED(h);
    Q_UNUSED(painter);
    Ut_PagePositionIndicatorView::drawnScalableImages.append(this);
}

static const int SCREEN_WIDTH = 846;
static const int SCREEN_HEIGHT = 480;

void Ut_PagePositionIndicatorView::initializeViewport(int amountOfPages, M::Orientation orientation)
{
    int viewportWidth, viewportHeight;
    if (orientation == M::Landscape) {
        viewportWidth = SCREEN_WIDTH;
        viewportHeight = SCREEN_HEIGHT;
    } else {
        viewportWidth = SCREEN_HEIGHT;
        viewportHeight = SCREEN_WIDTH;
    }
    positionIndicator->setViewportSize(QSizeF(viewportWidth, viewportHeight));
    positionIndicator->model()->setPageCount(amountOfPages);

    Ut_PagePositionIndicatorView::drawnScalableImages.clear();
}

void Ut_PagePositionIndicatorView::verifyPositionIndicatorDrawing(int expectedFocused, int amountOfPages)
{
    positionIndicator->paint(NULL, NULL, NULL);

    QCOMPARE(Ut_PagePositionIndicatorView::drawnScalableImages.count(), amountOfPages);

    // verify images and focused
    for(int i = 0; i < Ut_PagePositionIndicatorView::drawnScalableImages.count(); i++){
        if (i == expectedFocused) {
            QVERIFY(Ut_PagePositionIndicatorView::drawnScalableImages.at(i) == focusedIndicator);
        } else {
            QVERIFY(Ut_PagePositionIndicatorView::drawnScalableImages.at(i) == unfocusedIndicator);
        }
    }
}

void Ut_PagePositionIndicatorView::testPositionIndicatorDrawing()
{
    int amountOfPages = 5;
    initializeViewport(amountOfPages);
    verifyPositionIndicatorDrawing(0, amountOfPages);

    amountOfPages = 2;
    initializeViewport(amountOfPages);
    verifyPositionIndicatorDrawing(0, amountOfPages);

    amountOfPages = 10;
    initializeViewport(amountOfPages);
    verifyPositionIndicatorDrawing(0, amountOfPages);
}

void Ut_PagePositionIndicatorView::testIndicatorNotDrawnWhenOnlyOnePage()
{
    initializeViewport(1);
    positionIndicator->paint(NULL, NULL, NULL);
    QCOMPARE(Ut_PagePositionIndicatorView::drawnScalableImages.count(), 0);

    initializeViewport(0);
    positionIndicator->paint(NULL, NULL, NULL);
    QCOMPARE(Ut_PagePositionIndicatorView::drawnScalableImages.count(), 0);
}

void Ut_PagePositionIndicatorView::testIndicatorNotDrawnWhenControllerDisabled()
{
    initializeViewport(5);
    positionIndicator->setEnabled(false);
    positionIndicator->paint(NULL, NULL, NULL);
    QCOMPARE(Ut_PagePositionIndicatorView::drawnScalableImages.count(), 0);
}

void Ut_PagePositionIndicatorView::testPositionIndicatorFocusing()
{
    int amountOfPages = 10;
    int focusedPage = 9;
    positionIndicator->model()->setFocusedPage(focusedPage);
    initializeViewport(amountOfPages);
    verifyPositionIndicatorDrawing(9, amountOfPages);

    amountOfPages = 5;
    focusedPage = 0;
    positionIndicator->model()->setFocusedPage(focusedPage);
    initializeViewport(amountOfPages);
    verifyPositionIndicatorDrawing(0, amountOfPages);

    amountOfPages = 2;
    focusedPage = 1;
    positionIndicator->model()->setFocusedPage(focusedPage);
    initializeViewport(amountOfPages);
    verifyPositionIndicatorDrawing(1, amountOfPages);

    amountOfPages = 3;
    focusedPage = 1;
    positionIndicator->model()->setFocusedPage(focusedPage);
    initializeViewport(amountOfPages);
    verifyPositionIndicatorDrawing(1, amountOfPages);
}

void Ut_PagePositionIndicatorView::testFocusBetweenPagesStyleChanges()
{
    positionIndicator->getView()->modifiableStyle()->setFocusedBetweenPages(false);
    m_subject->applyStyle();
    QCOMPARE(disconnect(positionIndicator, SIGNAL(pageIsPanning(bool)), m_subject, SLOT(setForceUnfocusedIcon(bool))), true);

    positionIndicator->getView()->modifiableStyle()->setFocusedBetweenPages(true);
    m_subject->applyStyle();
    QCOMPARE(disconnect(positionIndicator, SIGNAL(pageIsPanning(bool)), m_subject, SLOT(setForceUnfocusedIcon(bool))), false);
    QCOMPARE(m_subject->forceUnfocusedIcon, false);
}

void Ut_PagePositionIndicatorView::testWhenDrawUnFocusedIconOnlyIsTrueOnlyUnfocusedIconsAreDrawn()
{
    m_subject->forceUnfocusedIcon = true;
    QPainter painter;
    QStyleOptionGraphicsItem option;
    initializeViewport(2);
    m_subject->drawContents(&painter, &option);
    QCOMPARE(Ut_PagePositionIndicatorView::drawnScalableImages.count(), 2);
    QCOMPARE(Ut_PagePositionIndicatorView::drawnScalableImages.at(0), m_subject->style()->unfocusedIndicatorImage());
    QCOMPARE(Ut_PagePositionIndicatorView::drawnScalableImages.at(1), m_subject->style()->unfocusedIndicatorImage());
}

QTEST_APPLESS_MAIN(Ut_PagePositionIndicatorView)
