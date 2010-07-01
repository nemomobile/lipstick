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

#ifndef UT_DESKTOPVIEW_H
#define UT_DESKTOPVIEW_H

#include <QtTest/QtTest>
#include <QObject>
#include "desktopview.h"

class MApplication;
class MainWindow;
class Desktop;
class QPainter;
class QPixmap;
class MButton;

class TestDesktopBackgroundExtension : public MDesktopBackgroundExtensionInterface
{
public:
    TestDesktopBackgroundExtension();
    ~TestDesktopBackgroundExtension();

    mutable QRectF boundingRect;
    virtual bool initialize(const QString &);
    virtual void setDesktopInterface(MDesktopInterface &desktopInterface);
    virtual void drawBackground(QPainter *painter, const QRectF &boundingRect) const;
};

class TestDesktopView : public DesktopView
{
    M_VIEW(DesktopModel, DesktopStyle)

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

    // Test opening launcher to first page
    void testToggleLauncher();
    // Test bounding rectangle and background drawing
    void testBoundingRectAndDrawBackground();
    // Test showing and hiding launcher
    void testUpdatingLauncherVisibilityWithApplicationOnTop();
    void testUpdatingLauncherVisibilityWithNotificationOnTop();
    void testUpdatingLauncherVisibilityWithStatusIndicatorMenuOnTop();
    void testUpdatingLauncherVisibilityWithDialogOnTop();
    void testUpdatingLauncherVisibilityWithDesktopOnTop();
    // Test showing launcher when showLauncherAndPanToPage is called
    void testShowLauncherAndPanToPageWithCorrectDesktopFile();
    // Test that launcher is not shown when bad desktop file is given
    void testShowLauncherAndPanToPageWithBadDesktopFile();
    // Test show launcher with empty desktop file path given
    void testShowLauncherAndPanToPageWithEmptyDesktopFile();

public:
    // The main window
    static MainWindow *mainWindow;

signals:
    void windowStackingOrderChanged(const QList<WindowInfo> &);

private:
    void verifyAppearDisappear(MSceneWindow *appear, MSceneWindow *disappear);
    void verifyLauncherVisibility(int topMostWindowId, bool shouldBeVisible);

    // The mapp
    MApplication *app;
    // The object being tested
    TestDesktopView *desktopView;
    // Desktop for the view
    Desktop *desktop;
    // Painter for painting
    QPainter *painter;
    // Images
    QPixmap *backgroundImage;
    QPixmap *backgroundTopImage;
    QPixmap *backgroundBottomImage;
};

#endif
