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

#ifndef UT_MAINWINDOW_H
#define UT_MAINWINDOW_H

#include <QtTest/QtTest>
#include <QObject>

class MainWindow;
class MApplication;

class Ut_MainWindow : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    void testCallUILaunchedWhenCallRelatedKeyPressed();
    void testContentSearchLaunchedWhenNonCallRelatedKeyPressed();
    void testLaunchQueuedWhenAlreadyLaunching();
    void testLaunchQueuedWhenLaunchingFailed();
    void testNothingLaunchedWhenControlModifierPressed();
    void testNothingLaunchedWhenOnlyModifierPressed();
    void testNothingLaunchedWhenMediaKeyPressed();
    void testCloseEventIsIgnored();
    void testOrientationLocking_data();
    void testOrientationLocking();

private:
    // Application for the test
    MApplication *app;

    // The object being tested
    MainWindow *mainWindow;
};

#endif
