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

#ifdef ENABLE_QTTRACKER
#include <QtTracker/RDFService>
#endif

#include "ut_duihomeshortcutmanager.h"
#include "duihomeshortcutmanager.h"

QString Ut_DuiHomeShortcutManager::query;
QString Ut_DuiHomeShortcutManager::thumbnailFileName;

// QPixmap stubs (used by DuiHomeShortcutManager)
bool QPixmap::save(const QString &fileName, const char *format, int quality) const
{
    Q_UNUSED(format);
    Q_UNUSED(quality);

    Ut_DuiHomeShortcutManager::thumbnailFileName = fileName;

    return true;
}

#ifdef ENABLE_QTTRACKER
// LibQtTracker stubs (used by DuiHomeShortcutManager)
namespace SopranoLive
{
    struct RDFGraphBridge {
        QVector<QStringList> rawExecuteQuery(QString const &query, int type = RDFQuery::SPARQL);
    };
}

QVector<QStringList> SopranoLive::RDFGraphBridge::rawExecuteQuery(QString const &query, int type)
{
    Q_UNUSED(type);

    Ut_DuiHomeShortcutManager::query = query;

    return QVector<QStringList>();
}
#endif

void Ut_DuiHomeShortcutManager::init()
{
}

void Ut_DuiHomeShortcutManager::cleanup()
{
}

void Ut_DuiHomeShortcutManager::initTestCase()
{
}

void Ut_DuiHomeShortcutManager::cleanupTestCase()
{
}

void Ut_DuiHomeShortcutManager::testShortcutCreation()
{
    // Create a shortcut
    QPixmap thumbnail(48, 48);
    QVERIFY(DuiHomeShortcutManager::createShortcut(QUrl("file:///home/user/image.jpeg"), "Name", thumbnail));

#ifdef ENABLE_QTTRACKER
    // This should create a certain SPARQL query
    QCOMPARE(query, QString("INSERT { <maemo:DesktopBookmarkFolder> nfo:containsBookmark _:x . _:x a nfo:Bookmark ; nfo:bookmarks <file:///home/user/image.jpeg> ; maemo:bookmarkThumbnail \"file://" + thumbnailFileName + "\" ; nie:title \"Name\" . }"));
#endif
}

QTEST_MAIN(Ut_DuiHomeShortcutManager)
