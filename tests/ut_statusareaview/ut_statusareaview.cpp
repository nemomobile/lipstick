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
#include "ut_statusareaview.h"
#include "statusareaview.h"
#include "statusarea.h"
#include "clock_stub.h"
#include "notifier_stub.h"
#include "contextframeworkcontext_stub.h"
#include "clockalarmstatusindicator_stub.h"
#include "batterystatusindicator_stub.h"
#include "phonenetworksignalstrengthstatusindicator_stub.h"
#include "internetconnectionstatusindicator_stub.h"
#include "bluetoothstatusindicator_stub.h"
#include "statusindicator_stub.h"
#include "phonenetworkstatusindicator_stub.h"
#include <DuiSceneWindow>
#include <QGraphicsLayout>

int Ut_StatusAreaView::windowExecutionCount;
int Ut_StatusAreaView::windowRejectionCount;

// SceneWindow stubs
void DuiSceneWindow::appear(DuiSceneWindow::DeletionPolicy)
{
    Ut_StatusAreaView::windowExecutionCount++;
}

void DuiSceneWindow::disappear()
{
    Ut_StatusAreaView::windowRejectionCount++;
}

// Called before the first testfunction is executed
void Ut_StatusAreaView::initTestCase()
{
}

// Called after the last testfunction was executed
void Ut_StatusAreaView::cleanupTestCase()
{
}

// Called before each testfunction is executed
void Ut_StatusAreaView::init()
{
    statusArea = new StatusArea;
    m_subject = new StatusAreaView(statusArea);
    statusArea->setView(m_subject);
}

// Called after every testfunction
void Ut_StatusAreaView::cleanup()
{
    delete statusArea;
    statusArea = NULL;
}

QTEST_APPLESS_MAIN(Ut_StatusAreaView)
