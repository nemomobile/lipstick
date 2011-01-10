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

#ifndef UT_PAGEPOSITIONINDICATORVIEW_H
#define UT_PAGEPOSITIONINDICATORVIEW_H

#include <QtTest/QtTest>
#include <QObject>
#include <MApplication>
#include <QList>

class MApplication;
class MScalableImage;
class PagePositionIndicatorView;
class TestPagePositionIndicator;

class Ut_PagePositionIndicatorView : public QObject
{
    Q_OBJECT
public:
   static QList<const MScalableImage *> drawnScalableImages;

private slots:
    // Called before the first testfunction is executed
    void initTestCase();
    // Called after the last testfunction was executed
    void cleanupTestCase();
    // Called before each testfunction is executed
    void init();
    // Called after every testfunction
    void cleanup();

    void testPositionIndicatorDrawing();
    void testIndicatorNotDrawnWhenOnlyOnePage();
    void testIndicatorNotDrawnWhenControllerDisabled();
    void testPositionIndicatorFocusing();
    void testFocusBetweenPagesStyleChanges();
    void testWhenDrawUnFocusedIconOnlyIsTrueOnlyUnfocusedIconsAreDrawn();

private:
    void initializeViewport(int amountOfPages, M::Orientation orientation = M::Landscape);
    void verifyPositionIndicatorDrawing(int expectedFocused, int amountOfPages);

    MApplication *app;
    PagePositionIndicatorView *m_subject;

    TestPagePositionIndicator *positionIndicator;
};

#endif
