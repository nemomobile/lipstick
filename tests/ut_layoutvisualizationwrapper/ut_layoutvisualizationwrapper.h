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

#ifndef UT_LAYOUTVISUALIZATIONWRAPPER_H
#define UT_LAYOUTVISUALIZATIONWRAPPER_H

#include <QObject>

class LayoutVisualizationWrapper;
class QGraphicsWidget;
class QGraphicsLinearLayout;

class Ut_LayoutVisualizationWrapper : public QObject
{
    Q_OBJECT

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
    void testSettingRightmostItemToAppearOnTheLeftSide();
    void testSettingLeftmostItemToAppearOnTheRightSide();
    void testChangingWrappingModeFromLeftToNoneSetsTransformationsCorrectly();
    void testChangingWrappingModeFromRightToNoneSetsTransformationsCorrectly();
    void testChangingWrappingModeFromLeftToRightSetsTransformationsCorrectly();
    void testChangingWrappingModeFromRightToLeftSetsTransformationsCorrectly();
    void testParentSizeChangeUpdatesTransformationsCorrectly();
    void testUsingVerticalLayoutDoesNothing();

private:
    //! Returns a testable layout
    void buildTestableWidget(Qt::Orientation orientation);

    //! Returns the x translation of the first widget in the test layout
    qreal firstWidgetXTranslation() const;
    //! Returns the x translation of the last widget in the test layout
    qreal lastWidgetXTranslation() const;
    //! Return the last widget
    QGraphicsWidget* getLastWidget() const;

    // The object being tested
    LayoutVisualizationWrapper *m_subject;
    // The widget used by the test subject
    QGraphicsWidget *testableWidget;
    // The layout used by the test subject
    QGraphicsLinearLayout *testableLayout;
};

#endif
