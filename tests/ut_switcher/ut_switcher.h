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
#ifndef _UT_SWITCHER_
#define _UT_SWITCHER_

#include <QObject>
#include "windowinfo.h"

class MApplication;
class Switcher;
class SwitcherButton;
class MSceneManager;
class QSizeF;
class QRectF;
class QPointF;

class MainWindow : public QObject
{
    Q_OBJECT

public:

    MainWindow();
    ~MainWindow();

    MainWindow *instance(bool);

    void exitDisplay();

signals:

    void displayExited();
};


class Ut_Switcher : public QObject
{
    Q_OBJECT

public:
    static QList<SwitcherButton *> iconGeometryUpdated;

private:
    MApplication *app;
    Switcher *switcher;

    QList<WindowInfo> createWindowList(int numWindows);

signals:
    // signal for bringing a window to front
    void windowToFront(Window window);

    // signal for closing a window
    void closeWindow(Window window);

    // Signal for updating the window list
    void windowListUpdated(const QList<WindowInfo> &windowList);

    // Signal for panning
    void sizePosChanged(const QSizeF &viewportSize, const QRectF &pannedRange, const QPointF &pannedPos);

    void windowTitleChanged(Window window, const QString &title);

private slots:
    // Executed once before every test case
    void init();

    // Executed once after every test case
    void cleanup();

    // Executed once before first test case
    void initTestCase();

    // Executed once after last test case
    void cleanupTestCase();

    // Test that the constructor connects proper signals
    void testConstruction();

    // Test bringing windows to front
    void testWindowToFront();

    // Test bringing windows to front
    void testCloseWindow();

    // Test adding windows
    void testWindowAdding();

    // Test removing windows
    void testWindowRemoving();

    void testWindowTitleChangeWhenWindowListIsUpdated();

    // Test changing window title
    void testWindowTitleChange();

    // Tests that panning updates the switcher button icon geometries
    void testPanning();

    // Test window ordering
    void testWindowOrder();

    // Test call window priority
    void testCallWindowAdding();

    // Test call window priority
    void testCallWindowFromExisistingWindow();

    // Test that the Switcher update is delayed when launching new applications
    void testUpdateDelay();

private:
    MSceneManager *mSceneManager;
};
#endif //_UT_SWITCHER_
