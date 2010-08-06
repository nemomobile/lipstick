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

#ifndef UT_APPLICATIONPACKAGEMONITOR_H
#define UT_APPLICATIONPACKAGEMONITOR_H

#include <QObject>

class MApplication;
class ApplicationPackageMonitor;

class Ut_ApplicationPackageMonitor : public QObject
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
private:
    // MApplication
    MApplication *app;
    // The object being tested
    ApplicationPackageMonitor *m_subject;

    //Simulates slots called during unsuccessfull package install
    void installUnsuccessfully(const QString&);
    //Simulates slots called during succesfull package install
    void installSuccessfully(const QString&);
    //There is a possibility that operation complete signal is emitted when download
    //progress is finished. Simulates that possibility.
    void installSuccessfullyWithOperationCompleteAfterDownload(const QString&);
    //Simulates slots called when uninstalling
    void uninstall(const QString&);

private slots:
    //Tests created dbus connections.
    void testConstruction();
    //Tests successfull install with multiple packages.
    void testSuccessfullInstall();
    //Tests unsuccessfull install with multiple packages
    void testUnsuccessfullInstall();
    //Tests uninstall.
    void testUninstallSignal();
    //Tests installing package that first fails and then succeeds.
    void testUnsuccesfullAndSuccesfullInstall();
    //Tests installing multiple packages successfully with operation
    //complete signal received after download progress.
    void testSuccessfullInstallWithOperationCompletedAfterDownload();


};

#endif
