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

#ifndef UT_TRANSFORMLAYOUTANIMATION_H
#define UT_TRANSFORMLAYOUTANIMATION_H

#include <QtTest/QtTest>
#include <QObject>
#include <MApplication>
#include <MLayout>
#include "transformlayoutanimation.h"

class TransformLayoutAnimation;
class QGraphicsLayoutItem;

class TestLayout : public MLayout
{
public:
    TestLayout() : MLayout(new QGraphicsWidget) {}
    virtual QGraphicsLayoutItem *itemAt(int index) const;
};

class TestTransformLayoutAnimation : public TransformLayoutAnimation
{
    M_ANIMATION(TransformLayoutAnimationStyle)

public:
    TestTransformLayoutAnimation(MLayout *layout) : TransformLayoutAnimation(layout) {}

    TransformLayoutAnimationStyle *modifiableStyle() {
        TransformLayoutAnimationStyleContainer &sc = style();
        const TransformLayoutAnimationStyle *const_s = sc.operator ->();
        TransformLayoutAnimationStyle *s = const_cast<TransformLayoutAnimationStyle *>(const_s);
        return s;
    }

protected:
    virtual TransformAnimation *createAnimation(QGraphicsWidget *widget);
};

class Ut_TransformLayoutAnimation : public QObject
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
    void testItemAddingInitialGeometry();
    void testAddingNonWidgets();
    void testSetGeometryAfterItemRemoval();
    void testTransformAnimation();
    void testCancelingAnimation();
    void testContainerAnimation();
    void testManualControl();
    void testAnimationCreation();

private:
    MApplication *app;
    MLayout *layout;
    QGraphicsWidget *container;
    // The object being tested
    TestTransformLayoutAnimation *m_subject;
};

#endif
