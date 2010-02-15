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

#ifndef UT_X11HELPER_H
#define UT_X11HELPER_H

#include <QObject>

class Ut_X11Helper : public QObject
{
    Q_OBJECT

public:
    static bool propertyQuerySucceeds;
    static bool propertySizeQuerySucceeds;

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
    void testGetNetWmState();
    void testGetNetWmStatePropertyQueryFails();
    void testGetNetWmStateProperySizeQueryFails();


};

#endif
