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

#ifndef UT_LAUNCHERBUTTONVIEW_H
#define UT_LAUNCHERBUTTONVIEW_H

#include <QObject>
#include <MWidgetView>

#include "launcherbuttonview.h"
#include "launcherbuttonmodel.h"

Q_DECLARE_METATYPE(LauncherButtonModel::State);

class MApplication;
class LauncherButton;
class LauncherButtonView;

class Ut_LauncherButtonStyle : public LauncherButtonStyle
{
};

class Ut_LauncherButtonStyleContainer : public LauncherButtonStyleContainer
{
public:
    QString currentMode()
    {
        return LauncherButtonStyleContainer::currentMode();
    }
};

class TestLauncherButtonView : public LauncherButtonView
{
    M_VIEW(LauncherButtonModel, Ut_LauncherButtonStyle)

public:
    TestLauncherButtonView(LauncherButton *button) : LauncherButtonView(button) {}

    LauncherButtonStyle *modifiableStyle() {
        LauncherButtonStyleContainer &sc = style();
        const LauncherButtonStyle *const_s = sc.operator ->();
        LauncherButtonStyle *s = const_cast<LauncherButtonStyle *>(const_s);
        return s;
    }
    Ut_LauncherButtonStyleContainer& styleContainer() {
        return style();
    }

    friend class Ut_LauncherButtonView;
};

class Ut_LauncherButtonView : public QObject
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
    void testInitialization();
    void testApplyStyle();
    void testResetProgressIndicator_data();
    void testResetProgressIndicator();
    void testLaunchingProgress();
    void testUpdateProgressWhenDownloading();
    void testUpdateProgressWhenNotDownloading();
    void testUpdatingIconFromDesktopEntry();
    void testUpdatingAbsoluteIconFromDesktopEntry();
    void testUpdatingFreeDesktopIconFromDesktopEntry();
    void testUpdatingPlaceholderIcons_data();
    void testUpdatingPlaceholderIcons();
    void testUpdatingIconFromDesktopEntryAfterPlaceholderIcon();
    void testEnablingAccordingToState_data();
    void testEnablingAccordingToState();
    void testWhenStateIsChangedToLaunchingThenProgressIndicatorIsShownAccordingToStyle_data();
    void testWhenStateIsChangedToLaunchingThenProgressIndicatorIsShownAccordingToStyle();

signals:
    void frameChanged(int frame);

private:
    // MApplication
    MApplication *app;
    // The object being tested
    TestLauncherButtonView *m_subject;
    // Controller for the object
    LauncherButton *controller;
};

#endif
