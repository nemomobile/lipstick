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

#include <QtGui>
#include "duihomeshortcutmanager.h"

#ifdef ENABLE_QTTRACKER
#include <QtTracker/Tracker>
#endif

DuiHomeShortcutManager::DuiHomeShortcutManager()
{
}

DuiHomeShortcutManager::~DuiHomeShortcutManager()
{
}

bool DuiHomeShortcutManager::createShortcut(const QUrl &iri, const QString &name, const QPixmap &thumbnail)
{
    // Create the thumbnail directory if it doesn't exist
    QString thumbnailFileName(QDir::homePath() + "/.thumbnails/normal/");
    if (!QDir::root().exists(thumbnailFileName))
        QDir::root().mkpath(thumbnailFileName);

    // Create the thumbnail filename from the IRI
    QByteArray const digest = QCryptographicHash::hash(iri.toEncoded(), QCryptographicHash::Md5);
    thumbnailFileName.append(QString::fromLatin1(digest.toHex()).rightJustified(32, '0'));
    thumbnailFileName.append(".png");

    // Write the thumbnail into a PNG file
    thumbnail.scaled(256, 256, Qt::KeepAspectRatio, Qt::SmoothTransformation).save(thumbnailFileName, "PNG");

    // Create the SPARQL INSERT statement
    QString query("INSERT { ");
    query.append("<maemo:DesktopBookmarkFolder> nfo:containsBookmark _:x . ");
    query.append("_:x a nfo:Bookmark ; ");
    query.append("nfo:bookmarks <");
    query.append(iri.toString());
    query.append("> ; ");
    query.append("maemo:bookmarkThumbnail \"file://");
    query.append(thumbnailFileName);
    query.append("\" ; ");
    query.append("nie:title \"");
    query.append(name);
    query.append("\" . ");
    query.append("}");

#ifdef ENABLE_QTTRACKER
    // Run the statement
    tracker()->rawSparqlUpdateQuery(query);
#endif

    // Always return true for now since there is no way to catch errors
    return true;
}
