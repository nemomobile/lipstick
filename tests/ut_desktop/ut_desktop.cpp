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
#include "ut_desktop.h"
#include "desktop.h"

void Ut_Desktop::init()
{
    // Creating a desktop also creates the desktop view
    desktop = new Desktop;
}

void Ut_Desktop::cleanup()
{
    // Destroy the desktop (and the associated view)
    delete desktop;
}

void Ut_Desktop::initTestCase()
{
    // MApplications must be created manually these days due to theme system changes
    static int argc = 1;
    static char *app_name = (char *)"./ut_desktop";
    app = new MApplication(argc, &app_name);
}

void Ut_Desktop::cleanupTestCase()
{
    // Destroy the MApplication
    delete app;
}

QTEST_APPLESS_MAIN(Ut_Desktop)
