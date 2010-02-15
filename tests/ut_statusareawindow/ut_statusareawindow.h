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

#ifndef UT_STATUSAREAWINDOW_H
#define UT_STATUSAREAWINDOW_H

#include <QtTest/QtTest>
#include <QObject>
#include <DuiNamespace>

class HomeApplication;
class MainWindow;
class StatusAreaWindow;

class Ut_StatusAreaWindow : public QObject
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
    void testRotate();

signals:
    // Signal for rotating
    void orientationChangeFinished(const Dui::Orientation &orientation);

private:
    // HomeApplication
    HomeApplication *app;
    // MainWindow
    MainWindow *mainWindow;
    // The object being tested
    StatusAreaWindow *m_subject;
};

#endif
