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
#ifndef _UT_SWITCHERBUTTONWITHTITLEBARVIEW_
#define _UT_SWITCHERBUTTONWITHTITLEBARVIEW_

#include <QtGui>
#include <QObject>
#include "switcherbutton.h"
#include "switcherbuttonwithtitlebarview.h"

class MainWindow;
class TestHomeApplication;

class Ut_SwitcherButtonStyle : public SwitcherButtonWithTitleBarStyle
{
};

class Ut_SwitcherButtonStyleContainer : public SwitcherButtonWithTitleBarStyleContainer
{
public:
    QString currentMode()
    {
        return MButtonStyleContainer::currentMode();
    }
};

class TestSwitcherButtonView : public SwitcherButtonWithTitleBarView
{
    M_VIEW(SwitcherButtonModel, Ut_SwitcherButtonStyle)

public:
    TestSwitcherButtonView(SwitcherButton &button);

    SwitcherButtonWithTitleBarStyle *modifiableStyle() {
        SwitcherButtonWithTitleBarStyleContainer &sc = style();
        const SwitcherButtonWithTitleBarStyle *const_s = sc.operator ->();
        SwitcherButtonWithTitleBarStyle *s = const_cast<SwitcherButtonWithTitleBarStyle *>(const_s);
        return s;
    }
    Ut_SwitcherButtonStyleContainer& styleContainer() {
        return style();
    }
};

class TestSwitcherButton : public SwitcherButton
{
public:
    TestSwitcherButton(QGraphicsItem *parent = NULL);

    TestSwitcherButtonView *getView();
    void emitDisplayEntered();
    void emitDisplayExited();

private:
    TestSwitcherButtonView *view;
};

class Ut_SwitcherButtonWithTitleBarView : public QObject
{
    Q_OBJECT

public:
    // Main window
    static MainWindow *mainWindow;

private:
    TestHomeApplication *app;

    // SwitcherButton instance under testing
    TestSwitcherButton *button;

    // SwitcherButtonView instance under testing
    TestSwitcherButtonView *m_subject;

    // The current top cropping
    int croppingTop;

signals:
    // Signal for notifying about close button presses
    void closeButtonPressed();

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();
    void testClosing();
    void testApplyingStyle();
    void testThumbnailPosition();
};

#endif //_UT_SWITCHERBUTTONWITHTITLEBARVIEW_
