/***************************************************************************
**
** Copyright (C) 2014 Jolla Ltd.
** Contact: Thomas Perl <thomas.perl@jolla.com>
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

#include <QtTest/QtTest>

#include "launcheritem.h"
#include "launchermodel.h"
#include "ut_launchermodel.h"
#include "mdesktopentry.h"

class MDesktopEntryPrivate {
public:
    MDesktopEntryPrivate(const QString &fileName)
        : m_fileName(fileName)
    {
    }

    QString m_fileName;
};

MDesktopEntry::MDesktopEntry(const QString &fileName)
    : d_ptr(new MDesktopEntryPrivate(fileName))
{
}

MDesktopEntry::~MDesktopEntry()
{
    delete d_ptr;
}

QString
MDesktopEntry::fileName() const
{
    return d_ptr->m_fileName;
}

QString
MDesktopEntry::exec() const
{
    return "";
}

QString
MDesktopEntry::name() const
{
    return "";
}

QString
MDesktopEntry::icon() const
{
    return "";
}

QString
MDesktopEntry::type() const
{
    return "Application";
}

QStringList
MDesktopEntry::categories() const
{
    return QStringList();
}

QString
MDesktopEntry::nameUnlocalized() const
{
    return "";
}

bool
MDesktopEntry::noDisplay() const
{
    return false;
}

bool
MDesktopEntry::isValid() const
{
    return true;
}

uint
MDesktopEntry::hash() const
{
    return 1234;
}

QString
MDesktopEntry::value(const QString &group, const QString &key) const
{
    Q_UNUSED(key)
    Q_UNUSED(group)

    return QString();
}

void QTimer::singleShot(int, const QObject *receiver, const char *member)
{
    // The "member" string is of form "1member()", so remove the trailing 1 and the ()
    int memberLength = strlen(member) - 3;
    char modifiedMember[memberLength + 1];
    strncpy(modifiedMember, member + 1, memberLength);
    modifiedMember[memberLength] = 0;
    QMetaObject::invokeMethod(const_cast<QObject *>(receiver), modifiedMember, Qt::DirectConnection);
}

bool QFile::exists() const
{
    return true;
}

void Ut_LauncherModel::init()
{
    launcherModel = new LauncherModel();
}

void Ut_LauncherModel::cleanup()
{
    delete launcherModel;
}

void Ut_LauncherModel::testUpdating()
{
    // Test if basic updating behavior works for a random package
    QVERIFY(launcherModel->packageInModel("somepackage") == NULL);

    launcherModel->updatingStarted("somepackage", "Some Package",
            "/usr/share/pixmaps/example.png", "", "org.example.caller");

    auto item = launcherModel->packageInModel("somepackage");
    QVERIFY(item != NULL);
    QVERIFY(item->updatingProgress() == -1);
    QVERIFY(item->isTemporary());
    QVERIFY(launcherModel->temporaryItemToReplace() == item);

    launcherModel->updatingProgress("somepackage", 20, "org.example.caller");

    QVERIFY(launcherModel->packageInModel("somepackage") == item);
    QVERIFY(item->updatingProgress() == 20);

    launcherModel->updatingProgress("somepackage", 40, "org.example.caller");

    QVERIFY(launcherModel->packageInModel("somepackage") == item);
    QVERIFY(item->updatingProgress() == 40);

    launcherModel->updatingFinished("somepackage", "org.example.caller");

    QVERIFY(launcherModel->packageInModel("somepackage") == NULL);
    QVERIFY(launcherModel->temporaryItemToReplace() == NULL);
}

void Ut_LauncherModel::testUpdatingFileAppears()
{
    // Test that starting an update with a non-existing item will add it to the
    // launcher list, and that when the file appears during the updating phase,
    // it will properly be transformed into a non-temporary launcher item and
    // persist even after the updating phase has finished.
    QVERIFY(launcherModel->packageInModel("somepackage") == NULL);

    const QString DESKTOPFILE("/usr/share/applications/lipstick_ut_launchermodel.desktop");

    launcherModel->updatingStarted("somepackage", "Some Package",
            "/usr/share/pixmaps/example.png", DESKTOPFILE,
            "org.example.caller");

    auto item = launcherModel->packageInModel("somepackage");
    QVERIFY(item != NULL);
    QVERIFY(item->updatingProgress() == -1);
    QVERIFY(item->isTemporary());
    QVERIFY(launcherModel->temporaryItemToReplace() == item);

    QVERIFY(launcherModel->itemInModel(DESKTOPFILE) == item);

    QStringList added, modified, removed;
    added << DESKTOPFILE;
    launcherModel->onFilesUpdated(added, modified, removed);

    QVERIFY(launcherModel->itemInModel(DESKTOPFILE) == item);
    QVERIFY(!item->isTemporary());

    launcherModel->updatingFinished("somepackage", "org.example.caller");

    QVERIFY(launcherModel->itemInModel(DESKTOPFILE) == item);
    QVERIFY(!item->isUpdating());

    QVERIFY(launcherModel->packageInModel("somepackage") == NULL);
    QVERIFY(launcherModel->temporaryItemToReplace() == NULL);
}

QTEST_MAIN(Ut_LauncherModel)
