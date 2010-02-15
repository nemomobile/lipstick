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
#ifndef _UT_LAUNCHERBUTTONVIEW_
#define _UT_LAUNCHERBUTTONVIEW_

#include <QtTest/QtTest>
#include <QObject>

class TestLauncherButtonView;
class LauncherButton;
class DuiApplication;
class DuiScalableImage;

class Ut_LauncherButtonView : public QObject
{
    Q_OBJECT

public:
    static QList<const DuiScalableImage *> drawnScalableImages;
    static QList<QString> availableThumbnails;

private:
    TestLauncherButtonView *view;
    LauncherButton *button;
    DuiApplication *app;

private slots:
    // Executed once before every test case
    void init();
    // Executed once after every test case
    void cleanup();
    // Executed once before first test case
    void initTestCase();
    // Executed once after last test case
    void cleanupTestCase();

    // Test that shortcut container image is used when
    // shortcut thumbnail is valid.
    void testValidShortcut();

    // Test that invalid shortcut container image is used when
    // shortcut thumbnail is not defined or is not found.
    void testInvalidShortcut();

    // Test that feedbacks are shown and played when button is down
    void testButtonPressedFeedbacks();
};
#endif //_UT_LAUNCHERBUTTONVIEW_
