/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of systemui.
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
#ifndef UT_LOWBATTERYNOTIFIER_H
#define UT_LOWBATTERYNOTIFIER_H

#include <QObject>

class LowBatteryNotifier;

#ifdef HAVE_CONTEXTSUBSCRIBER
class ContextItem;
#endif

class Ut_LowBatteryNotifier : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();
    void testSignalConnections();
    void testSendLowBatteryAlert();
#ifdef HAVE_CONTEXTSUBSCRIBER
    void testSetNotificationInterval_data();
    void testSetNotificationInterval();
    void testSetNotificationIntervalDoesNothingWhenStateDoesNotChange();
#endif

private:
    LowBatteryNotifier *m_subject;
#ifdef HAVE_CONTEXTSUBSCRIBER
    ContextItem *contextItem;
#endif
};

#endif
