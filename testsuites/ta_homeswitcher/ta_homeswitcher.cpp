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
#include <QObject>
#include <QDebug>
#include <DuiApplication>
#include <DuiApplicationWindow>
#include <DuiApplicationPage>
#include <DuiLabel>

int main(int argc, char **argv)
{
    // Get application id from the command line
    QString id("0");

    for (int i = 1; i < argc; ++i) {
        QString str(argv[i]);

        if (str == "-id") {
            if (i < argc - 1) {
                id = argv[i + 1];
                ++i;
            }
        }
    }

    // Initialize everything
    QString title = QString("Ta_HomeSwitcher-") + id;

    DuiApplication app(argc, argv, title);
    DuiApplicationWindow window;
    window.setWindowTitle(title);
    window.show();

    DuiApplicationPage page;
    page.setTitle(title);
    page.appear();

    return app.exec();
}

