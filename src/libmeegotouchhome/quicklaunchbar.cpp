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
#include "applicationpackagemonitorlistener.h"
#include <MDesktopEntry>

#include <MWidgetCreator>
M_REGISTER_WIDGET(QuickLaunchBar)

const int QuickLaunchBar::NUMBER_OF_LAUNCHER_BUTTONS = 4;
const QString QuickLaunchBar::LOCATION_IDENTIFIER = "quicklaunchbar";
const char QuickLaunchBar::SECTION_SEPARATOR = '/';

QuickLaunchBar::QuickLaunchBar(QGraphicsItem *parent) :
        MWidgetController(new QuickLaunchBarModel, parent),
        dataStore(NULL)
{
}

QuickLaunchBar::~QuickLaunchBar()
{
}

void QuickLaunchBar::setLauncherDataStore(LauncherDataStore *dataStore)
{
    if (this->dataStore != NULL) {
        disconnect(dataStore, SIGNAL(dataStoreChanged()), this, SLOT(updateButtons()));
    }
    this->dataStore = dataStore;
    connect(dataStore, SIGNAL(dataStoreChanged()), this, SLOT(updateButtons()));
}

void QuickLaunchBar::setApplicationPackageMonitorListener(ApplicationPackageMonitorListener *packageMonitorListener)
{
    this->packageMonitorListener = packageMonitorListener;

    connect(packageMonitorListener, SIGNAL(packageStateChanged(QString, LauncherButtonModel::State, int)),
        this, SLOT(updateButtonState(QString, LauncherButtonModel::State, int)));
}

void QuickLaunchBar::updateButtonState(const QString &desktopEntryPath, LauncherButtonModel::State state, int progress)
{
    //If there is a quicklaunch button for desktopEntryPath, update it's state
    QList<QSharedPointer<LauncherButton> > buttons = model()->buttons().values();
    QString entryFileName = QFileInfo(desktopEntryPath).fileName();
    for (int i = 0 ; i < buttons.count() ; i++) {
        if (QFileInfo(buttons.at(i)->desktopEntry()).fileName() == entryFileName) {
            buttons.at(i)->setState(state, progress);
        }
    }
}

void QuickLaunchBar::updateButtons()
{
    if (dataStore == NULL) {
        return;
    }

    // Temporarily disable the listening of the change signals from the configuration to prevent a recursive call to this method
    dataStore->disconnect(this);

    // Construct a map of buttons and their positions
    QMap< int, QSharedPointer<LauncherButton> > newButtons;

    // Add the desktop entries with known placements to the map
    QMapIterator<Placement, QString> iterator(createPlacementMap(dataStore->dataForAllDesktopEntries()));
    while (iterator.hasNext()) {
        iterator.next();
        Placement placement(iterator.key());
        QString desktopEntryPath(iterator.value());
        newButtons.insert(placement.position, QSharedPointer<LauncherButton>(createLauncherButton(desktopEntryPath)));
    }

    // Take the new buttons into use
    model()->setButtons(newButtons);

    // Reconnect signals
    connect(dataStore, SIGNAL(dataStoreChanged()), this, SLOT(updateButtons()));
}

LauncherButton *QuickLaunchBar::createLauncherButton(const QString &desktopEntryPath)
{
    LauncherButton *button = new LauncherButton(desktopEntryPath);
    button->setObjectName("QuickLaunchBarButton");
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
