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

#ifndef DUIHOMESHORTCUTMANAGER_H_
#define DUIHOMESHORTCUTMANAGER_H_

class QString;
class QPixmap;
class QUrl;

class DuiHomeShortcutManager
{
private:
    /*!
     * Constructs a shortcut manager.
     */
    DuiHomeShortcutManager();

    /*!
     * Destroys a shortcut manager.
     */
    virtual ~DuiHomeShortcutManager();

public:
    /*!
     * Inserts the shortcut into Tracker and stores the thumbnail into ~/.thumbnails/normal/
     *
     * NOTE: there is currently no way to catch errors so this function always returns true!
     *
     * \param iri the IRI of the content to create a shortcut to
     * \param name the name of the shortcut
     * \param thumbnail the thumbnail image for the shortcut
     * \return true if the shortcut was successfully created, false otherwise
     */
    static bool createShortcut(const QUrl &iri, const QString &name, const QPixmap &thumbnail);
};

#endif /* DUIHOMESHORTCUTMANAGER_H_ */
