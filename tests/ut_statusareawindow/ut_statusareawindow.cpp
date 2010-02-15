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

#include <DuiDeviceProfile>
#include "ut_statusareawindow.h"
#include "statusareawindow.h"
#include "statusarea_stub.h"
#include "homeapplication_stub.h"
#include "mainwindow_stub.h"
#include "x11wrapper_stub.h"

// X stubs to avoid crashes
void XSetWMProperties(Display *, Window, XTextProperty *, XTextProperty *, char **, int, XSizeHints *, XWMHints *, XClassHint *)
{
}

// DuiWindow stubs
Dui::OrientationAngle currentOrientationAngle;
Dui::OrientationAngle DuiWindow::orientationAngle() const
{
    return currentOrientationAngle;
}

void Ut_StatusAreaWindow::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_statusareawindow";
    app = new HomeApplication(argc, &app_name);
    mainWindow = MainWindow::instance(true);
    gHomeApplicationStub->stubSetReturnValue("mainWindow", mainWindow);
}

void Ut_StatusAreaWindow::cleanupTestCase()
{
    delete mainWindow;
    delete app;
}

void Ut_StatusAreaWindow::init()
{
    m_subject = new StatusAreaWindow();
}

void Ut_StatusAreaWindow::cleanup()
{
    delete m_subject;
}

void Ut_StatusAreaWindow::testRotate()
{
    connect(this, SIGNAL(orientationChangeFinished(const Dui::Orientation &)), m_subject, SLOT(rotate(const Dui::Orientation &)));

    StatusArea *statusArea = m_subject->statusArea();
    statusArea->setPreferredSize(100, 50);

    currentOrientationAngle = Dui::Angle0;
    emit orientationChangeFinished(Dui::Landscape);
    QCOMPARE(m_subject->size(), statusArea->preferredSize().toSize());
    QCOMPARE(m_subject->pos(), QPoint());

    currentOrientationAngle = Dui::Angle180;
    emit orientationChangeFinished(Dui::Landscape);
    QCOMPARE(m_subject->size(), statusArea->preferredSize().toSize());
    QCOMPARE(m_subject->pos(), QPoint(0, DuiDeviceProfile::instance()->resolution().height() - m_subject->height()));

    currentOrientationAngle = Dui::Angle90;
    emit orientationChangeFinished(Dui::Portrait);
    QSize size = statusArea->preferredSize().toSize();
    size.transpose();
    QCOMPARE(m_subject->size(), size);
    QCOMPARE(m_subject->pos(), QPoint(DuiDeviceProfile::instance()->resolution().width() - m_subject->width(), 0));

    currentOrientationAngle = Dui::Angle270;
    emit orientationChangeFinished(Dui::Portrait);
    QCOMPARE(m_subject->size(), size);
    QCOMPARE(m_subject->pos(), QPoint());
}

QTEST_APPLESS_MAIN(Ut_StatusAreaWindow)
