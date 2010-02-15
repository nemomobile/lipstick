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

#include "ut_windowinfo.h"
#include "windowinfo.h"


void Ut_WindowInfo::initTestCase()
{
}

void Ut_WindowInfo::cleanupTestCase()
{
}

void Ut_WindowInfo::init()
{
}

void Ut_WindowInfo::cleanup()
{
}

void Ut_WindowInfo::testGetters()
{
    // Title of the window
    QString title = "Title";
    // Window ID
    Window window = 1;
    // Window attributes
    XWindowAttributes attributes;
    memset(&attributes, 0, sizeof(XWindowAttributes));
    attributes.depth = 16;
    attributes.width = 320;
    attributes.height = 256;
    // Window pixmap ID
    Pixmap pixmap = 3;

    WindowInfo *info = new WindowInfo(title, window, attributes, pixmap);
    QCOMPARE(info->title(), title);
    QCOMPARE(info->window(), window);
    QCOMPARE(info->windowAttributes().depth, 16);
    QCOMPARE(info->windowAttributes().width, 320);
    QCOMPARE(info->windowAttributes().height, 256);
    QCOMPARE(info->icon(), pixmap);

    delete info;
}

QTEST_MAIN(Ut_WindowInfo)
