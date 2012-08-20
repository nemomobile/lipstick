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

#include <QX11Info>

#include "mainwindow.h"
#include "homeapplication.h"

#include "components/menumodel.h"
#include "components/switchermodel.h"
#include "components/switcherpixmapitem.h"
#include "components/statusbar.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xdamage.h>

int main(int argc, char *argv[])
{
    qmlRegisterType<MenuModel>("org.nemomobile.lipstick", 0, 1, "MenuModel");
    qmlRegisterType<SwitcherModel>("org.nemomobile.lipstick", 0, 1, "SwitcherModel");
    qmlRegisterType<SwitcherPixmapItem>("org.nemomobile.lipstick", 0, 1, "WindowPixmap");
    qmlRegisterType<StatusBar>("org.nemomobile.lipstick", 0, 1, "StatusBar");

    HomeApplication app(argc, argv);

    MainWindow *mainWindow = MainWindow::instance(true);
    mainWindow->setAttribute(Qt::WA_OpaquePaintEvent);
    mainWindow->setAttribute(Qt::WA_NoSystemBackground);
    mainWindow->viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
    mainWindow->viewport()->setAttribute(Qt::WA_NoSystemBackground);
    mainWindow->show();
    QObject::connect(&app, SIGNAL(aboutToQuit()), mainWindow, SLOT(deleteLater()));

    // Tell X that changes in the properties and the substructure of the root
    // window are interesting. These are used to get the list of windows and
    // for getting window close events. Note that QWidget calls XSelectInput
    // as well so this call cannot be done in the HomeApplication constructor
    // since Qt's own call would then override this.
    XSelectInput(QX11Info::display(), DefaultRootWindow(QX11Info::display()), PropertyChangeMask | SubstructureNotifyMask);

    return app.exec();
}
