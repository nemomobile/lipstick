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

#ifndef UT_APPLICATIONPACKAGEMONITORLISTENER_H
#define UT_APPLICATIONPACKAGEMONITORLISTENER_H

#include <QObject>
#include "launcherbuttonmodel.h"

Q_DECLARE_METATYPE(LauncherButtonModel::State);

class MApplication;
class ApplicationPackageMonitorListener;

class Ut_ApplicationPackageMonitorListener : public QObject
{
    Q_OBJECT

private slots:
    // Called before the first testfunction is executed
    void initTestCase();
    // Called after the last testfunction was executed
    void cleanupTestCase();
    // Called before each testfunction is executed
    void init();
    // Called after every testfunction
    void cleanup();

    // Test cases
    void testSignalConnections();
    void testSetDownloadProgress_data();
    void testSetDownloadProgress();
    void testSetInstallProgress();
    void testSetOperationSuccess();
    void testSetOperationError();
    void testUpdatePackageStates();
    void testIsInstallerExtraEntry();

private:
    // MApplication
    MApplication *app;
    // The object being tested
    ApplicationPackageMonitorListener *m_subject;
};

#endif
