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

#include <MApplication>
#include <MDesktopEntry>
#include "ut_launcherpage.h"
#include "launcherpage.h"
#include "launcherbutton.h"

static QSharedPointer<LauncherButton> createLauncherButton(QString desktopFileName = QString());

// LauncherButton stubs
LauncherButton::LauncherButton(MWidget *parent) : MButton(parent, new LauncherButtonModel) {}
LauncherButton::LauncherButton(const QString&, MWidget*parent) : MButton(parent, new LauncherButtonModel) {}
LauncherButton::~LauncherButton(){}

QString LauncherButton::desktopEntry() const
{
    return objectName();
}

int updateFromDesktopEntryCallCount = 0;
void LauncherButton::updateFromDesktopEntry(const QString &)
{
    updateFromDesktopEntryCallCount++;
}

void LauncherButton::launch()
{
}

void LauncherButton::retranslateUi()
{
    return;
}

bool LauncherButton::isInProgress() const
{
    return false;
}

void LauncherButton::setProgressIndicatorTimeout(int)
{
}

void LauncherButton::hideProgressIndicator()
{
}

void LauncherButton::hideProgressIndicatorIfObscured(const QList<WindowInfo> &)
{
}

void LauncherButton::init()
{
}

void Ut_LauncherPage::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_launcherpage" };
    app = new MApplication(argc, app_name);
}

void Ut_LauncherPage::cleanupTestCase()
{
    delete app;
}

void Ut_LauncherPage::init()
{
    m_subject = new LauncherPage();
    updateFromDesktopEntryCallCount = 0;
}

void Ut_LauncherPage::cleanup()
{
    delete m_subject;
}

void Ut_LauncherPage::testAddButtonWhenPageNotFull()
{
    m_subject->model()->setMaxButtons(2);
    QSharedPointer<LauncherButton> button1 = createLauncherButton();
    bool added = m_subject->appendButton(button1);
    QVERIFY(added);
    QCOMPARE(m_subject->model()->launcherButtons().count(), 1);

    QSharedPointer<LauncherButton> button2 = createLauncherButton();
    added = m_subject->appendButton(button2);
    QVERIFY(added);
    QCOMPARE(m_subject->model()->launcherButtons().count(), 2);
}

void Ut_LauncherPage::testAddButtonWhenPageIsFull()
{
    m_subject->model()->setMaxButtons(1);
    QSharedPointer<LauncherButton> button1 = createLauncherButton();
    bool added = m_subject->appendButton(button1);
    QVERIFY(added);
    QCOMPARE(m_subject->model()->launcherButtons().count(), 1);

    QSharedPointer<LauncherButton> button2 = createLauncherButton();
    added = m_subject->appendButton(button2);
    QVERIFY(!added);
    QCOMPARE(m_subject->model()->launcherButtons().count(), 1);
}

void Ut_LauncherPage::testRemoveButton()
{
    m_subject->model()->setMaxButtons(1);
    QSharedPointer<LauncherButton> button = createLauncherButton("my-entry-name");
    bool added = m_subject->appendButton(button);
    QVERIFY(added);
    QCOMPARE(m_subject->model()->launcherButtons().count(), 1);

    m_subject->removeButton(button);
    QCOMPARE(m_subject->model()->launcherButtons().count(), 0);

    // Try to remove the same button again
    m_subject->removeButton(button);
    QCOMPARE(m_subject->model()->launcherButtons().count(), 0);
}

void Ut_LauncherPage::testUpdateButton()
{
    QSharedPointer<LauncherButton> button1 = createLauncherButton("my-entry-name-1");
    QSharedPointer<LauncherButton> button2 = createLauncherButton("my-entry-name-updated");
    m_subject->appendButton(button1);
    m_subject->appendButton(button2);
    m_subject->updateButton("my-entry-name-updated");

    QCOMPARE(updateFromDesktopEntryCallCount, 1);
}

static QSharedPointer<LauncherButton> createLauncherButton(QString desktopFileName)
{
    QSharedPointer<LauncherButton> button(new LauncherButton);
    button->setObjectName(desktopFileName);
    return button;
}

QTEST_APPLESS_MAIN(Ut_LauncherPage)
