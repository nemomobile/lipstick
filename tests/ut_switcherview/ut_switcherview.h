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

#ifndef UT_SWITCHERVIEW_H
#define UT_SWITCHERVIEW_H

#include <QtTest/QtTest>
#include <QObject>
#include <MApplication>
#include "windowinfo.h"

class MWindow;
class TestSwitcherView;
class Switcher;
class SwitcherButton;
class MSceneManager;
class QPinchGesture;
class QGestureEvent;

class Ut_SwitcherView : public QObject
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

    // Test cases for detail view
    void testSnapIndexChangedInDetailView();
    void testPanningStoppedInDetailView();

    // Test cases for over view
    void testButtonModesInOverviewMode();
    void testPanningStoppedInOverView();

    // Test cases for mode changing
    void testDetailToOverviewModeChange();
    void testOverviewToDetailModeChange();
    void testModeChangeCancel();
    void testTransitionControl();
    void testBounceAnimation();
    void testOverpinchOverview();
    void testOverpinchDetailview();

    // Tests that correct number of buttons are in overview and detailview modes
    void testButtonCounts();

    // Test case for event handler
    void testEvent();

    // Test that the buttons are positioned correctly
    void testSwitcherButtonAlignment();

    // Test that buttons are removed correctly
    void testRemovingButtons();

    // Test that switcher pans to the correct page when button is added in overview mode
    void testPanToSwitcherPageInOverviewMode();

    // Test that switcher pans to the correct page when button is added in detailview mode
    void testPanToSwitcherPageInDetailviewMode();

    // Test when pinching empty area nearest button is detected
    void testWhenPinchingOnEmptyAreaNearestButtonIsDetected();

    void testWhenPinchGestureHasBeenPerformedOnTopOfPressedSwitcherButtonThenSwitcherButtonIsUp();
    void testWhenPinchGestureHasBeenPerformedOnTopOfNonPressedSwitcherButtonThenAllOfTheSwitcherButtonsAreUp();

    void testWhenPinchGestureStartsThenSceneEventFilteringForSwitcherButtonStarts();
    void testGraphicsSceneMouseMoveEventsDoNotGetFilteredForSwitcherButtonsByDefault();
    void testGraphicsSceneMouseMoveEventsGetFilteredForSwitcherButtons();
    void testOtherThanGraphicsSceneMouseMoveEventsDoNotGetFilteredForSwitcherButtons();
    void testGraphicsSceneMouseMoveEventsDoNotGetFilteredForOtherThanSwitcherButtons();
    void testPanningDisabledWhenNoSwitcherButtons_NB186716();
    void testWhenPinchingStartsOrStopsThenViewportPhysicsEnabledOrDisabled();

private:
    void verifyButtonModesInOverviewMode(M::Orientation orientation);
    void verifyButtonModesInOverviewMode(QList< QSharedPointer<SwitcherButton> > &buttonList);
    void verifyLayoutPolicyContentMargins(const QSizeF &buttonSize);

    QList< QSharedPointer<SwitcherButton> > createButtonList(int buttons);
    void appendMoreButtonsToList(int newButtons, QList< QSharedPointer<SwitcherButton> > &buttonList);

    void pinchGesture(qreal scaleFactor, Qt::GestureState state);

signals:
    void pageChanged(int newPosition);
    void panningStopped();

private:
    MApplication *app;
    MSceneManager *mSceneManager;
    Switcher *switcher;
    // The object being tested
    TestSwitcherView *m_subject;

    QPinchGesture *mPinch;
    QGestureEvent *mEvent;
};

#endif
