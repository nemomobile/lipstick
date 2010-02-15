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
#include <DuiButton>
#include "windowpropertyhandler.h"

int main(int argc, char **argv)
{
    if (argc < 2) {
        qDebug() << "usage: ta_callui button_title <init_call_mode>";
        return 1;
    }

    // Initialize everything

    DuiApplication app(argc, argv, argv[1]);
    DuiApplicationWindow window;
    window.setWindowTitle(argv[1]);
    window.show();

    DuiApplicationPage page;
    page.setTitle(argv[1]);
    DuiButton button(argv[1]);
    page.setCentralWidget(&button);
    page.appear();
    WindowPropertyHandler propertyHandler(&page, &window, &button);
    QObject::connect(&button, SIGNAL(clicked(bool)),
                     &propertyHandler, SLOT(changeWindowProperty(bool)));
    if (argc > 2) {
        propertyHandler.changeWindowProperty(true);
    }

    return app.exec();
}

