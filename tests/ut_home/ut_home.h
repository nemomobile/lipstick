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

#ifndef UT_HOME_H
#define UT_HOME_H

#include <QtTest/QtTest>
#include <QObject>

class DuiApplication;
class MainWindow;
class StatusArea;
class Home;
class NotificationArea;
class DuiSceneWindow;

class Ut_Home : public QObject
{
    Q_OBJECT

public:
    static DuiSceneWindow *launcherButtonWindow;
    static QString launcherButtonIconID;
    static MainWindow *mainWin;

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
    // Test bounding rectangle
    void testBoundingRect();
    // Test showing and hiding launcher
    void testShowingHidingLauncher();
    // Test notification area visibility
    void testNotificationAreaVisibility();
    // Test notification area panning
    void testNotificationAreaPanning();
    // Test notification area transition
    void testNotificationAreaTransition();
    // Test launcher button hiding
    void testHideLauncherButton();

signals:
    void launcherButtonClicked();
    void hideLauncherButton(bool hide);

private:
    // DuiApplication
    DuiApplication *app;
    // The object being tested
    Home *home;
    // NotificationArea
    NotificationArea *notificationAreaWidget;
    // StatusArea
    StatusArea *statusArea;
};

#endif
