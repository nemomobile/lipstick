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

#include "mainwindow.h"
#include "homeapplication.h"
#include "x11wrapper.h"

int main(int argc, char *argv[])
{
    HomeApplication app(argc, argv, QString("meegotouchhome"));

    MainWindow *mainWindow = MainWindow::instance(true);
    mainWindow->show();
    QObject::connect(&app, SIGNAL(aboutToQuit()), mainWindow, SLOT(deleteLater()));

    // Tell X that changes in the properties and the substructure of the root
    // window are interesting. These are used to get the list of windows and
    // for getting window close events. Note that QWidget calls XSelectInput
    // as well so this call cannot be done in the HomeApplication constructor
    // since Qt's own call would then override this.
    X11Wrapper::XSelectInput(QX11Info::display(), DefaultRootWindow(QX11Info::display()), PropertyChangeMask | SubstructureNotifyMask);

    return app.exec();
}
