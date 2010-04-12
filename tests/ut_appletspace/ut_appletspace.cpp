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

#include <MApplication>
#include "ut_appletspace.h"
#include "appletspace.h"

void Ut_AppletSpace::initTestCase()
{
}

void Ut_AppletSpace::cleanupTestCase()
{
}

void Ut_AppletSpace::init()
{
    m_subject = new AppletSpace();
}

void Ut_AppletSpace::cleanup()
{
    delete m_subject;
}

void Ut_AppletSpace::testEnabled()
{
    // Test that close button visibility follows enabledness
    m_subject->setEnabled(true);
    QCOMPARE(m_subject->model()->closeButtonVisible(), true);

    m_subject->setEnabled(false);
    QCOMPARE(m_subject->model()->closeButtonVisible(), false);
}

QTEST_APPLESS_MAIN(Ut_AppletSpace)
