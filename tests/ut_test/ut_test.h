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
#ifndef _UT_TEST_
#define _UT_TEST_

#include <QtTest/QtTest>
#include <QObject>

class Ut_Test : public QObject
{
    Q_OBJECT

private slots:
    // Executed once before every test case
    void init();

    // Executed once after every test case
    void cleanup();
};
#endif //_UT_TEST_
