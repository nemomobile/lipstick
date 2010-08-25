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
#include <MButton>
#include <MApplication>
#include "ut_extendedbuttonview.h"
#include "extendedbuttonview.h"

void Ut_ExtendedButtonView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_extendedbuttonview" };
    app = new MApplication(argc, app_name);
}

void Ut_ExtendedButtonView::cleanupTestCase()
{
    delete app;
}

void Ut_ExtendedButtonView::init()
{
    controller = new MButton;
    view = new ExtendedButtonView(controller);
    controller->setView(view);
}

void Ut_ExtendedButtonView::cleanup()
{
    delete controller;
}

void Ut_ExtendedButtonView::testBoundingRect()
{
    ExtendedButtonStyle *style = const_cast<ExtendedButtonStyle *>(view->style().operator ->());
    style->setExtendLeft(1);
    style->setExtendRight(2);
    style->setExtendTop(3);
    style->setExtendBottom(4);
    QRectF styledBoundingRect = view->boundingRect();
    style->setExtendLeft(0);
    style->setExtendRight(0);
    style->setExtendTop(0);
    style->setExtendBottom(0);
    QRectF normalBoundingRect = view->boundingRect();
    QCOMPARE(normalBoundingRect.adjusted(-1, -3, 2, 4), styledBoundingRect);
}

QTEST_APPLESS_MAIN(Ut_ExtendedButtonView)
