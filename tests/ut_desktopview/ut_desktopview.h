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

#ifndef UT_DESKTOPVIEW_H
#define UT_DESKTOPVIEW_H

#include <QtTest/QtTest>
#include <QObject>
#include "desktopview.h"

class DuiApplication;
class MainWindow;
class Desktop;
class QPainter;
class QPixmap;
class MockPaintDevice;
class DuiButton;

class TestDesktopView : public DesktopView
{
    DUI_VIEW(DesktopModel, DesktopStyle)

public:
    TestDesktopView(Desktop *desktop);

    DesktopStyle *modifiableStyle() {
        DesktopStyleContainer &sc = style();
        const DesktopStyle *const_s = sc.operator ->();
        DesktopStyle *s = const_cast<DesktopStyle *>(const_s);
        return s;
    }

};

class Ut_DesktopView : public QObject
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

    // Test geometry setting
    void testSetGeometry();
    // Test bounding rectangle and background drawing
    void testBoundingRectAndDrawBackground();
    // Test showing and hiding launcher
    void testShowingHidingLauncher();

public:
    static QRectF paintArea;

    // The main window
    static MainWindow *mainWindow;

signals:
    void launcherButtonClicked();

private:
    // The duiapp
    DuiApplication *app;
    // The object being tested
    TestDesktopView *desktopView;
    // Desktop for the view
    Desktop *desktop;
    // Painter for painting
    QPainter *painter;
    // Mock paint device for painting
    MockPaintDevice *paintDevice;
    // Images
    QPixmap *backgroundImage;
    QPixmap *backgroundTopImage;
    QPixmap *backgroundBottomImage;
};

#endif
