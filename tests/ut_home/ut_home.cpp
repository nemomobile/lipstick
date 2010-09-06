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

#include <MPannableViewport>
#include <MModalSceneWindow>
#include <MWidgetView>
#include <MButton>
#include <MScene>
#include <MSceneManager>
#include "ut_home.h"
#include "homeapplication_stub.h"
#include "home.h"
#include "desktop_stub.h"
#include "windowinfo_stub.h"
#include "x11wrapper_stub.h"

// X stubs to avoid crashes
void XSetWMProperties(Display *, Window, XTextProperty *, XTextProperty *, char **, int, XSizeHints *, XWMHints *, XClassHint *)
{
}

// Tests
void Ut_Home::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_home";
    app = new HomeApplication(argc, &app_name);
}

void Ut_Home::cleanupTestCase()
{
    delete app;
}

void Ut_Home::init()
{
    home = new Home();
    home->setGeometry(QRectF(0, 0, 864, 480));
}

void Ut_Home::cleanup()
{
    delete home;
}

void Ut_Home::testBoundingRect()
{
    QCOMPARE(home->boundingRect(), home->desktop->boundingRect());
}

QTEST_APPLESS_MAIN(Ut_Home)
