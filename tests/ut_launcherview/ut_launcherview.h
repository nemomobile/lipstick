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

#ifndef UT_LAUNCHERVIEW_H
#define UT_LAUNCHERVIEW_H

#include <QtTest/QtTest>
#include <QObject>
#include "launcherview.h"

class MApplication;
class MApplicationWindow;
class Launcher;
class LauncherDataStore;

class Ut_LauncherView : public QObject
{
    Q_OBJECT

signals:
    void updateDataRequested(const QList<const char *>& modifications);
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
    void testPagedViewportObjectName();
    void testSetButtons();
    void testAddPages();
    void testRemovingPages();
    void testRemovingPagesFromLayoutInDestructor();
    void testSignalConnection();
    void testSetPage();
    void testUpdateData();
    void testFocusToButton();

private:
    // MApplication
    MApplication *app;
    // MApplicationWindow needed by the application
    MApplicationWindow* appWindow;
    // The object being tested
    Launcher *controller;
    LauncherDataStore *launcherDataStore;
    LauncherView *view;

};

#endif
