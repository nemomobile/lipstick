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
#ifndef _UT_DESKTOP_
#define _UT_DESKTOP_

#include <QtTest/QtTest>
#include <QObject>
#include <duiapplication.h>
#include <duiwidget.h>
#include "desktop.h"

class Ut_Desktop : public QObject
{
    Q_OBJECT

public:
    static DuiWidget *widget1;
    static DuiWidget *widget2;
    static DuiWidget *widget3;

private:
    DuiApplication *app;
    Desktop *desktop;

signals:
    // Signal for adding a widget to the Desktop
    void addWidget(DuiWidget *widget);

    // Signal for removing a widget from the Desktop
    void removeWidget(DuiWidget *widget);

private slots:
    // Executed once before every test case
    void init();

    // Executed once after every test case
    void cleanup();

    // Executed once before first test case
    void initTestCase();

    // Executed once after last test case
    void cleanupTestCase();

};
#endif //_UT_DESKTOP_
