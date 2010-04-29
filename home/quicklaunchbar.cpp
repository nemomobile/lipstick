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
#include "launcherdatastore.h"
#include "launcherbutton.h"
#include "launcher.h"
#include <MDesktopEntry>

const int QuickLaunchBar::NUMBER_OF_LAUNCHER_BUTTONS = 4;
const QString QuickLaunchBar::LOCATION_IDENTIFIER = "quicklaunchbar";
const char QuickLaunchBar::SECTION_SEPARATOR = '/';

QuickLaunchBar::QuickLaunchBar(LauncherDataStore *configuration, QGraphicsItem *parent) :
        MWidgetController(new QuickLaunchBarModel, parent),
        dataStore(configuration)
{
    // Fill in the model with empty widgets
    QList<MWidget *> widgets;
    for (int i = 0; i < NUMBER_OF_LAUNCHER_BUTTONS; i++) {
        widgets.append(new MWidget);
    }
    model()->setWidgets(widgets);

    // Listen to changes in data store contents
    connect(dataStore, SIGNAL(dataStoreChanged()), this, SLOT(updateWidgetList()));
}

QuickLaunchBar::~QuickLaunchBar()
{
}

void QuickLaunchBar::updateWidgetList()
{
    // Temporarily disable the listening of the change signals from the configuration to prevent a recursive call to this method
    dataStore->disconnect(this);

    // Get the old widgets so that they can be removed
    QList<MWidget *> oldWidgets(model()->widgets());

    // Construct a list of new widgets
    QList<MWidget *> newWidgets;

    // Put the desktop entries with known placements in place first
    QMapIterator<Placement, QString> iterator(createPlacementMap(dataStore->dataForAllDesktopEntries()));
    while (iterator.hasNext()) {
        iterator.next();
        Placement placement(iterator.key());
        QString desktopEntryPath(iterator.value());

        while (placement.position > newWidgets.size()) {
            newWidgets.append(new MWidget);
        }
        newWidgets.append(createLauncherButton(desktopEntryPath));
    }

    // Fill in the rest with empty buttons
    while (newWidgets.size() < NUMBER_OF_LAUNCHER_BUTTONS) {
        newWidgets.append(new MWidget);
    }

    // Set the object names
    foreach(MWidget *widget, newWidgets) {
        widget->setObjectName("QuickLaunchBarButton");
    }

    // Take the new widgets into use
    model()->setWidgets(newWidgets);

    // Delete the old widgets
    qDeleteAll(oldWidgets);

    // Reconnect signals
    connect(dataStore, SIGNAL(dataStoreChanged()), this, SLOT(updateWidgetList()));
}

void QuickLaunchBar::launchApplication(const QString &application)
{
    Launcher::startApplication(application);
}

void QuickLaunchBar::launchMApplication(const QString &serviceName)
{
    Launcher::startMApplication(serviceName);
}

LauncherButton *QuickLaunchBar::createLauncherButton(const QString &desktopEntryPath)
{
    MDesktopEntry desktopEntry(desktopEntryPath);
    LauncherButton *button = new LauncherButton(desktopEntry);
    connect(button, SIGNAL(applicationLaunched(const QString &)), this, SLOT(launchApplication(const QString &)), Qt::QueuedConnection);
    connect(button, SIGNAL(mApplicationLaunched(const QString &)), this, SLOT(launchMApplication(const QString &)), Qt::QueuedConnection);
    return button;
}

QMap<QuickLaunchBar::Placement, QString> QuickLaunchBar::createPlacementMap(const QHash<QString, QVariant> &desktopEntryPlacements)
{
    QMap<QuickLaunchBar::Placement, QString> placementMap;

    foreach (const QString &desktopEntryPath, desktopEntryPlacements.keys()) {
        Placement placement(desktopEntryPlacements.value(desktopEntryPath).toString());
        if (placement.position >= 0) {
            placementMap.insert(placement, desktopEntryPath);
        }
    }

    return placementMap;
}

QuickLaunchBar::Placement::Placement(const QString &placement) : position(-1) {
    if (placement.section(SECTION_SEPARATOR, 0, 0) == LOCATION_IDENTIFIER) {
        position = placement.section(SECTION_SEPARATOR, 1, 1).toInt();
    }
}

inline bool operator<(const QuickLaunchBar::Placement &p1, const QuickLaunchBar::Placement &p2)
{
    return p1.position < p2.position;
}
