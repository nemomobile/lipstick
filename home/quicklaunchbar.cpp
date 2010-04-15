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

#include "quicklaunchbar.h"
#include "launcherbutton.h"
#include "launcher.h"
#include <MDesktopEntry>
#include <mfiledatastore.h>
#include "launcherdatastore.h"
#include <QDir>

const int QuickLaunchBar::NUMBER_OF_LAUNCHER_BUTTONS = 4;

QuickLaunchBar::QuickLaunchBar(QGraphicsItem *parent) : MWidgetController(new QuickLaunchBarModel, parent),
    configurationDataStore(NULL)
{
    init();
}

QuickLaunchBar::QuickLaunchBar(LauncherDataStore *configuration, QGraphicsItem *parent) :
        MWidgetController(new QuickLaunchBarModel, parent),
        configurationDataStore(configuration)
{
    init();
}

void QuickLaunchBar::init()
{
    initializeDataStore();
    updateWidgetList();

    // Start watching the applications directory for changes
    connect(&desktopDirectoryWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(updateWidgetList()));
    desktopDirectoryWatcher.addPath(APPLICATIONS_DIRECTORY);
}

QuickLaunchBar::~QuickLaunchBar()
{
}

void QuickLaunchBar::initializeDataStore()
{
    connect(configurationDataStore, SIGNAL(dataStoreChanged()), this, SLOT(updateWidgetList()));
}

void QuickLaunchBar::updateWidgetList()
{
    // Get the old widgets so that they can be removed
    QList<MWidget *> oldWidgets(model()->widgets());

    // Construct a list of new widgets
    QList<MWidget *> newWidgets;

    // Temporarily disable the listening of the change signals from the configuration to prevent a recursive call to this method
    configurationDataStore->disconnect(this);
    QList<LauncherButton*> buttons = configurationDataStore->quickLaunchBarButtons();
    connect(configurationDataStore, SIGNAL(dataStoreChanged()), this, SLOT(updateWidgetList()));

    for(int i = 0; i < NUMBER_OF_LAUNCHER_BUTTONS; i++) {
        MWidget *widget = NULL;
        if(i >= buttons.size() || buttons[i] == NULL) {
            widget = new MWidget;
        } else {
            widget = buttons[i];
            connect(widget, SIGNAL(applicationLaunched(const QString &)), this, SLOT(launchApplication(const QString &)), Qt::QueuedConnection);
            connect(widget, SIGNAL(mApplicationLaunched(const QString &)), this, SLOT(launchMApplication(const QString &)), Qt::QueuedConnection);
        }

        widget->setObjectName("QuickLaunchBarButton");
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

void QuickLaunchBar::launchMApplication(const QString &serviceName)
{
    Launcher::startMApplication(serviceName);
}
