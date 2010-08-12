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

#ifndef UT_LAUNCHERBUTTONPROGRESSINDICATORVIEW_H
#define UT_LAUNCHERBUTTONPROGRESSINDICATORVIEW_H

#include <QObject>
#include <MWidgetView>

#include "launcherbuttonprogressindicatorview.h"
#include "launcherbuttonprogressindicatormodel.h"
#include "launcherbuttonmodel.h"

Q_DECLARE_METATYPE(LauncherButtonModel::State);

class MApplication;
class LauncherButtonProgressIndicator;
class LauncherButtonProgressIndicatorView;

class Ut_LauncherButtonProgressIndicatorStyle : public LauncherButtonProgressIndicatorStyle
{
};

class Ut_LauncherButtonProgressIndicatorStyleContainer : public LauncherButtonProgressIndicatorStyleContainer
{
public:
    QString currentMode()
    {
        return LauncherButtonProgressIndicatorStyleContainer::currentMode();
    }
};

class TestLauncherButtonProgressIndicatorView : public LauncherButtonProgressIndicatorView
{
    M_VIEW(LauncherButtonProgressIndicatorModel, Ut_LauncherButtonProgressIndicatorStyle)

public:
    TestLauncherButtonProgressIndicatorView(LauncherButtonProgressIndicator *button) : LauncherButtonProgressIndicatorView(button) {}

    friend class Ut_LauncherButtonProgressIndicatorView;
};

class Ut_LauncherButtonProgressIndicatorView : public QObject
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
    void testUpdateDataForStyleModes_data();
    void testUpdateDataForStyleModes();

private:
    // MApplication
    MApplication *app;
    // The object being tested
    TestLauncherButtonProgressIndicatorView *m_subject;
    // Controller for the object
    LauncherButtonProgressIndicator *controller;
};

#endif
