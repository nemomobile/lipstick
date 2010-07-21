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

#ifndef UT_PAGEDVIEWPORT_H
#define UT_PAGEDVIEWPORT_H

#include <QtTest/QtTest>
#include <QObject>

class MApplication;
class PagedViewport;

class Ut_PagedViewport : public QObject
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

    // Tests updatePageWidth method
    void test_updatePageCount();
    // Tests that pageChanged signal is emitted when panToPage
    // is called
    void test_panToPage();
    // Tests setFirstPagePosition method
    void test_focusFirstPage();
    // Tests setPage method
    void test_setPage();

private:
    // MApplication
    MApplication *app;
    // The object being tested
    PagedViewport *m_subject;
};

#endif
