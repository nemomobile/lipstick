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
//============================================================================
// Name        : DuiHomeShortcutCreator.cpp
// Author      : Vesa Halttunen
// Version     : 0.0
// Description : Creates a shortcut into home
//============================================================================

#include <iostream>
#include <QApplication>
#include <QPixmap>
#include <QUrl>
#include <QDir>
#include <duihomeshortcutmanager.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    if (argc != 3) {
        std::cerr << "USAGE: duihomeshortcutcreator FILENAME NAME\n";
        return -1;
    }

    // Load the image
    QPixmap pixmap;
    pixmap.load(argv[1]);
    if (pixmap.isNull()) {
        std::cerr << "Error: unable to load image\n";
        return -1;
    }

    // Create a shortcut from the image
    DuiHomeShortcutManager::createShortcut(QUrl::fromLocalFile(QDir::current().absoluteFilePath(argv[1])),
                                           argv[2], pixmap);

    return 0;
}
