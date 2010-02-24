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

#include "quicklaunchbar.h"
#include "launcherbutton.h"
#include "launcher.h"
#include <DuiDesktopEntry>
#include <duifiledatastore.h>
#include <QDir>

const int QuickLaunchBar::NUMBER_OF_LAUNCHER_BUTTONS = 4;

QuickLaunchBar::QuickLaunchBar(QGraphicsItem *parent) : DuiWidgetController(new QuickLaunchBarModel, parent),
    fileDataStore(NULL)
{
    initializeDataStore();
    updateWidgetList();

    // Start watching the applications directory for changes
    connect(&desktopDirectoryWatcher, SIGNAL(directoryChanged(const QString &)), this, SLOT(updateWidgetList()));
    desktopDirectoryWatcher.addPath(APPLICATIONS_DIRECTORY);
}

QuickLaunchBar::~QuickLaunchBar()
{
    delete fileDataStore;
}

void QuickLaunchBar::initializeDataStore()
{
    if (!QDir::root().exists(QDir::homePath() + "/.config/duihome")) {
        QDir::root().mkpath(QDir::homePath() + "/.config/duihome");
    }

    fileDataStore = new DuiFileDataStore(QDir::homePath() + "/.config/duihome/quicklaunchbar.data");
}

void QuickLaunchBar::updateWidgetList()
{
    // Get the old widgets so that they can be removed
    QList<DuiWidget *> oldWidgets(model()->widgets());

    // Construct a list of new widgets
    QList<DuiWidget *> newWidgets;
    for (int i = 1; i <= NUMBER_OF_LAUNCHER_BUTTONS; i++) {
        DuiWidget *widget = NULL;

        QString key;
        key.sprintf("%d/desktopFile", i);
        if (fileDataStore->contains(key)) {
            DuiDesktopEntry desktopEntry(fileDataStore->value(key).toString());
            if (desktopEntry.isValid()) {
                widget = new LauncherButton(desktopEntry);
                widget->setObjectName("QuickLaunchBarButton");

                connect(widget, SIGNAL(applicationLaunched(const QString &)), this, SLOT(launchApplication(const QString &)), Qt::QueuedConnection);
                connect(widget, SIGNAL(duiApplicationLaunched(const QString &)), this, SLOT(launchDuiApplication(const QString &)), Qt::QueuedConnection);
            }
        }

        if (widget == NULL) {
            // Use an empty widget if the entry was not valid
            widget = new DuiWidget;
        }

        newWidgets.append(widget);
    }

    // Take the new widgets into use
    model()->setWidgets(newWidgets);

    // Delete the old widgets
    qDeleteAll(oldWidgets);
}

void QuickLaunchBar::launchApplication(const QString &application)
{
    Launcher::startApplication(application);
}

void QuickLaunchBar::launchDuiApplication(const QString &serviceName)
{
    Launcher::startDuiApplication(serviceName);
}
