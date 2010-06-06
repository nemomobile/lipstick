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
#include <MApplication>
#include <MDesktopEntry>
#include "ut_launcherbutton.h"
#include "launcherbutton.h"
#include "launcher_stub.h"

// MDesktopEntry stubs (used by LauncherButton)
QMap<const MDesktopEntry *, QString> desktopEntryFileName;
QMap<QString, QString> desktopEntryType;
QMap<QString, QString> desktopEntryXMaemoService;
QMap<QString, QString> desktopEntryName;
QMap<QString, QString> desktopEntryIcon;
QMap<QString, QString> desktopEntryExec;

MDesktopEntry::MDesktopEntry(const QString &fileName) :
    d_ptr(NULL)
{
    desktopEntryFileName.insert(this, fileName);
}

QString MDesktopEntry::fileName() const
{
    return desktopEntryFileName[this];
}


bool MDesktopEntry::isValid() const
{
    return true;
}

QString MDesktopEntry::type() const
{
    return desktopEntryType.value(desktopEntryFileName.value(this));
}

QString MDesktopEntry::xMaemoService() const
{
    return desktopEntryXMaemoService.value(desktopEntryFileName.value(this));
}

QString MDesktopEntry::name() const
{
    return desktopEntryName.value(desktopEntryFileName.value(this));
}

QString MDesktopEntry::icon() const
{
    return desktopEntryIcon.value(desktopEntryFileName.value(this));
}

QString MDesktopEntry::exec() const
{
    return desktopEntryExec.value(desktopEntryFileName.value(this));
}

// QIcon stubs
QString iconFileName;
QIcon::QIcon(const QString &fileName)
{
    iconFileName = fileName;
}

QIcon::~QIcon()
{
}

void Ut_LauncherButton::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_launcherbutton" };
    app = new MApplication(argc, app_name);
}

void Ut_LauncherButton::cleanupTestCase()
{
    delete app;
}

void Ut_LauncherButton::init()
{
    desktopEntryFileName.clear();
    desktopEntryType.clear();
    desktopEntryXMaemoService.clear();
    desktopEntryName.clear();
    desktopEntryIcon.clear();
    desktopEntryExec.clear();
    iconFileName.clear();
    m_subject = new LauncherButton();
    connect(this, SIGNAL(clicked()), m_subject, SLOT(launch()));
}

void Ut_LauncherButton::cleanup()
{
    delete m_subject;
}

void Ut_LauncherButton::testInitialization()
{
    delete m_subject;

    desktopEntryType.insert("/dev/null", "Application");
    desktopEntryName.insert("/dev/null", "name");
    desktopEntryIcon.insert("/dev/null", "icon");
    desktopEntryExec.insert("/dev/null", "exec");

    m_subject = new LauncherButton("/dev/null");
    QCOMPARE(m_subject->targetType(), QString("Application"));
    QCOMPARE(m_subject->text(), QString("name"));
    QCOMPARE(m_subject->iconID(), QString("icon"));
    QCOMPARE(m_subject->target(), QString("exec"));
}

void Ut_LauncherButton::testInitializationXMaemoService()
{
    delete m_subject;

    desktopEntryType.insert("/dev/null", "Application");
    desktopEntryXMaemoService.insert("/dev/null", "service");

    m_subject = new LauncherButton("/dev/null");
    QCOMPARE(m_subject->targetType(), QString("Service"));
    QCOMPARE(m_subject->target(), QString("service"));
}

void Ut_LauncherButton::testInitializationAbsoluteIcon()
{
    delete m_subject;

    desktopEntryIcon.insert("/dev/null", "/icon");

    m_subject = new LauncherButton("/dev/null");
    QCOMPARE(m_subject->iconID(), QString());
    QCOMPARE(iconFileName, QString("/icon"));
}

void Ut_LauncherButton::testLaunchApplication()
{
    m_subject->setTargetType("Application");
    m_subject->setTarget("testApplication");
    emit clicked();
    QCOMPARE(gLauncherStub->stubCallCount("startApplication"), 1);
    QCOMPARE(gLauncherStub->stubLastCallTo("startApplication").parameter<QString>(0), m_subject->target());
}

void Ut_LauncherButton::testLaunchMApplication()
{
    m_subject->setTargetType("Service");
    m_subject->setTarget("testService");
    emit clicked();
    QCOMPARE(gLauncherStub->stubCallCount("startMApplication"), 1);
    QCOMPARE(gLauncherStub->stubLastCallTo("startMApplication").parameter<QString>(0), m_subject->target());
}

QTEST_APPLESS_MAIN(Ut_LauncherButton)
