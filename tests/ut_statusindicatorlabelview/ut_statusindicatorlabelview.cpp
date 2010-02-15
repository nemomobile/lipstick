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

#include "ut_statusindicatorlabelview.h"
#include "statusindicatorlabelview.h"

#include <DuiApplication>
#include <DuiLabel>

TestStatusIndicatorLabelView::TestStatusIndicatorLabelView(StatusIndicator *controller) :
    StatusIndicatorLabelView(controller)
{
}

StatusIndicatorModel *TestStatusIndicatorLabelView::getModel()
{
    return model();
}

QString gDuiLabelText;
void DuiLabel::setText(const QString &text)
{
    gDuiLabelText = text;
}

void Ut_StatusIndicatorLabelView::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_statusindicatorlabelview";
    app = new DuiApplication(argc, &app_name);
}

void Ut_StatusIndicatorLabelView::cleanupTestCase()
{
    delete app;
}

void Ut_StatusIndicatorLabelView::init()
{
    controller = new StatusIndicator;
    m_subject = new TestStatusIndicatorLabelView(controller);
    controller->setView(m_subject);

    gDuiLabelText.clear();
}

void Ut_StatusIndicatorLabelView::cleanup()
{
    delete m_subject;
}

void Ut_StatusIndicatorLabelView::testLabelChanged()
{
    m_subject->getModel()->setValue("newValue");
    QCOMPARE(gDuiLabelText, QString("newValue"));
}

void Ut_StatusIndicatorLabelView::testModelChanged()
{
    StatusIndicatorModel *m = new StatusIndicatorModel;
    m->setValue("newValue");
    m_subject->setModel(m);
    QCOMPARE(gDuiLabelText, QString("newValue"));
}


QTEST_APPLESS_MAIN(Ut_StatusIndicatorLabelView)
