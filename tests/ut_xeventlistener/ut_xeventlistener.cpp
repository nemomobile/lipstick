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

#include <QtTest/QtTest>
#include "ut_xeventlistener.h"
#include "xeventlistener.h"
#include "homeapplication_stub.h"


class TestXEventListener : public XEventListener
{
public:
    virtual bool handleXEvent(const XEvent &event) {
        Q_UNUSED(event);
        return true;
    }
};


void Ut_XEventListener::initTestCase()
{
    static char *argv[] = { (char *)"./ut_switcher" };
    static int argc = sizeof(argv) / sizeof(char*);
    app = new HomeApplication(argc, argv);
}

void Ut_XEventListener::cleanupTestCase()
{
    delete app;
}

void Ut_XEventListener::init()
{
    m_subject = new TestXEventListener();
}

void Ut_XEventListener::cleanup()
{
    delete m_subject;
    gHomeApplicationStub->stubReset();
}

void Ut_XEventListener::testListenerAutoRegistersWhenConstructed()
{
    QCOMPARE(gHomeApplicationStub->stubCallCount("addXEventListener"), 1);
}

void Ut_XEventListener::testListenerUnregistersWhenDestructed()
{
    delete m_subject;
    m_subject = NULL;
    QCOMPARE(gHomeApplicationStub->stubCallCount("removeXEventListener"), 1);
}

QTEST_APPLESS_MAIN(Ut_XEventListener)
