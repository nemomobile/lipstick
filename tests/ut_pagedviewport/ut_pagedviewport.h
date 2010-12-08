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

#ifndef UT_PAGEDVIEWPORT_H
#define UT_PAGEDVIEWPORT_H

#include <QtTest/QtTest>
#include <QObject>
#include "layoutvisualizationwrapper.h"

class MApplication;
class PagedViewport;
class QGraphicsLinearLayout;

class Ut_PagedViewport : public QObject
{
    Q_OBJECT

signals:
    void panningStopped();
    void pageWrapped();
    void pageChanged(int);

private slots:
    // Called before the first testfunction is executed
    void initTestCase();
    // Called after the last testfunction was executed
    void cleanupTestCase();
    // Called before each testfunction is executed
    void init();
    // Called after every testfunction
    void cleanup();

    // Test cases

    void testInitialState();
    // Tests updatePageWidth method
    void test_updatePageCount();
    // Tests that pageChanged signal is emitted when panToPage
    // is called
    void test_panToPage();
    // Tests setFirstPagePosition method
    void test_focusFirstPage();
    // Tests setPage method
    void test_setPage();
    void testWhenPagePansPositionIndicatorsGetToKnow();
    void testWhenTargetPageIsCalledThenTheInformationIsFetchedFromThePhysicsObject();

    void testWhenThereIsNoPannebleWidgetAndWrappingIsEnabledThenVisualizationWrapperDoesNotGetCalled();
    void testWhenThereIsNoLinearLayoutInThePannebleWidgetAndWrappingIsEnabledThenVisualizationWrapperDoesNotGetCalled();

    void testWhenPageWrappingIsEnabledAndPanningStopsOnFirstPageThenVisualizationWrapperWrapsRightEdgeToLeft_data();
    void testWhenPageWrappingIsEnabledAndPanningStopsOnFirstPageThenVisualizationWrapperWrapsRightEdgeToLeft();
    void testWhenPageWrappingIsEnabledAndPanningStopsOnLastPageThenVisualizationWrapperWrapsLeftEdgeToRight_data();
    void testWhenPageWrappingIsEnabledAndPanningStopsOnLastPageThenVisualizationWrapperWrapsLeftEdgeToRight();
    void testWhenPageWrappingIsEnabledAndPanningStopsOnNonEdgePageThenVisualizationWrapperDoesNotWrap();
    void testWhenPageWrappingIsEnabledAndWrappingHappensFromFirstPageToLastPageThenVisualizationWrapperWrapsLeftEdgeToRight_data();
    void testWhenPageWrappingIsEnabledAndWrappingHappensFromFirstPageToLastPageThenVisualizationWrapperWrapsLeftEdgeToRight();
    void testWhenPageWrappingIsEnabledAndWrappingHappensFromLastPageToFirstPageThenVisualizationWrapperWrapsRightEdgeToLeft_data();
    void testWhenPageWrappingIsEnabledAndWrappingHappensFromLastPageToFirstPageThenVisualizationWrapperWrapsRightEdgeToLeft();

    void testWhenPageWrappingIsEnabledAndWrappingHappensFromSecondPageToFirstPageThenVisualizationWrapperWrapsRightEdgeToLeft_data();
    void testWhenPageWrappingIsEnabledAndWrappingHappensFromSecondPageToFirstPageThenVisualizationWrapperWrapsRightEdgeToLeft();
    void testWhenPageWrappingIsEnabledAndWrappingHappensFromFirstPageToSecondPageThenVisualizationWrapperDoesNotWrap();
    void testWhenPageWrappingIsEnabledAndWrappingHappensFromSecondLastPageToLastPageThenVisualizationWrapperWrapsLeftEdgeToRight_data();
    void testWhenPageWrappingIsEnabledAndWrappingHappensFromSecondLastPageToLastPageThenVisualizationWrapperWrapsLeftEdgeToRight();
    void testWhenPageWrappingIsEnabledAndWrappingHappensFromLastPageToSecondLastPageThenVisualizationWrapperDoesNotWrap();

    void testWhenPageWrappingIsEnabledAndWrappingHappensBetweenNonEndPagesThenVisualizationWrapperDoesNotWrap();

    void testWhenPageWrappingGetsDisabledThenVisualizationWrapperDoesNotGetCalled();

private:
    // MApplication
    MApplication *app;
    // The object being tested
    PagedViewport *m_subject;
    // The layout that is in the panneble widget in the test subject if any
    QGraphicsLinearLayout *pannedLayout;

    // Puts a widget with the specified amount of pages to the test subject
    void fillSubjectWithPages(int numPages);

    // A helper method for verifying correct use of layout visualization wrapper
    void verifyLayoutWrapper(LayoutVisualizationWrapper::WrappingMode wrapMode, bool verifyConstruction = true) const;

};

#endif
