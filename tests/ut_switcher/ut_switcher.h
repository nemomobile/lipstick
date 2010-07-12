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
#include <QPair>
#include "mwindow.h"
#include "windowinfo.h"

class MApplication;
class Switcher;
class SwitcherButton;
class MSceneManager;
class QSizeF;
class QRectF;
class QPointF;

class WindowListReceiver : public QObject
{
    Q_OBJECT

public:
    WindowListReceiver() : count(0), stackingCount(0) { }
    QList<WindowInfo> windowList;
    QList<WindowInfo> stackingWindowList;
    int count;
    int stackingCount;

    QPair<Window, QString> changedTitle;

private slots:

    void windowListUpdated(const QList<WindowInfo> &windowList) {
        this->windowList = windowList;
        this->count++;
    }

    void stackingWindowListUpdated(const QList<WindowInfo> &windowList) {
        this->stackingWindowList = windowList;
        this->stackingCount++;
    }

    void changeWindowTitle(Window w, const QString &title)
    {
        changedTitle.first = w;
        changedTitle.second = title;
    }
};

class WindowVisibilityReceiver : public QObject
{
    Q_OBJECT

public:
    WindowVisibilityReceiver() { }
    QList<Window> windowList;

private slots:
    void windowVisibilityChanged(Window window) {
        this->windowList.append(window);
    }
};

class MainWindow : public MWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

    static MainWindow *instance(bool);

    void exitDisplay();

signals:

    void displayExited();
};


class Ut_Switcher : public QObject
{
    Q_OBJECT

public:
    static QList<SwitcherButton *> iconGeometryUpdated;
    static QList<Window> visibilityNotifyWindows;
    static int clientListNumberOfWindows;

private:
    MApplication *app;
    Switcher *switcher;

    void verifyModel(const QList<WindowInfo> &windowList);
    QString titleForWindow(Window window, const QList<WindowInfo> &windowList);

private slots:
    // Executed once before every test case
    void init();

    // Executed once after every test case
    void cleanup();

    // Executed once before first test case
    void initTestCase();

    // Executed once after last test case
    void cleanupTestCase();

    // Test bringing windows to front
    void testWindowToFront();

    // Test bringing windows to front
    void testCloseWindow();

    // Test X11EventFilter with PropertyNotify events
    void testX11EventFilterWithPropertyNotify();

    // Test X11EventFilter with VisibilityNotify events
    void testX11EventFilterWithVisibilityNotify();

    // Test X11EventFilter with ClientMessage events
    void testX11EventFilterWithClientMessage();

    // Test the stacking order signal
    void testWindowStackingOrder();

    // Test data for test testX11EventWindowNameChange
    void testX11EventWindowNameChange_data();
    // Test that title change signal is emitted when X window property changes
    void testX11EventWindowNameChange();

    // Test that the Switcher update is delayed when launching new applications
    void testUpdateDelay();

private:
    MSceneManager *mSceneManager;
};
#endif //_UT_SWITCHER_
