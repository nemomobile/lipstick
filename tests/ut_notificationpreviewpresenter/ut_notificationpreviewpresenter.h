/***************************************************************************
**
** Copyright (C) 2012 Jolla Ltd.
** Contact: Robin Burchell <robin.burchell@jollamobile.com>
**
** This file is part of lipstick.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#ifndef UT_NOTIFICATIONPREVIEWPRESENTER_H
#define UT_NOTIFICATIONPREVIEWPRESENTER_H

#include <QObject>

class Ut_NotificationPreviewPresenter : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanup();
    void testSignalConnections();
    void testAddNotificationWhenWindowNotOpen();
    void testAddNotificationWhenWindowAlreadyOpen();
    void testUpdateNotification();
    void testRemoveNotification();
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    void testWindowMasking_data();
    void testWindowMasking();
#endif
    void testNotificationNotShownIfNoSummaryOrBody_data();
    void testNotificationNotShownIfNoSummaryOrBody();
    void testNotificationNotShownIfHidden();
    void testShowingOnlyCriticalNotifications();
    void testUpdateNotificationRemovesNotificationFromQueueIfNotShowable();
#ifdef HAVE_QMSYSTEM
    void testNotificationNotShownIfTouchScreenIsLockedAndDisplayIsOff_data();
    void testNotificationNotShownIfTouchScreenIsLockedAndDisplayIsOff();
#endif
    void testCriticalNotificationIsClosedAfterShowing();
};

#endif
