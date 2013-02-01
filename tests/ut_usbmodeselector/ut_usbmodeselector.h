/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
#ifndef _UT_USBMODESELECTOR_H
#define _UT_USBMODESELECTOR_H

#include <QObject>

class USBModeSelector;

class Ut_USBModeSelector : public QObject {
    Q_OBJECT

private slots:
    // Executed once before every test case
    void init();

    // Executed once after every test case
    void cleanup();

    // Executed once before first test case
    void initTestCase();

    // Executed once after last test case
    void cleanupTestCase();

    // Test cases
    void testConnections();
    void testShowDialog_data();
    void testShowDialog();
    void testHideDialog_data();
    void testHideDialog();
    void testUSBNotifications_data();
    void testUSBNotifications();
    void testConnectingUSBWhenDeviceIsLockedEmitsDialogShown_data();
    void testConnectingUSBWhenDeviceIsLockedEmitsDialogShown();
    void testShowError();
    void testSetUSBMode();

private:
    USBModeSelector *usbModeSelector;
};

#endif
