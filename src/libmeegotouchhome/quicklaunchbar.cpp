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
#include "applicationpackagemonitor.h"
#include <MDesktopEntry>
#include "launcher.h"

#include <MWidgetCreator>
M_REGISTER_WIDGET(QuickLaunchBar)

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

    if (dataStore != NULL) {
        connect(dataStore, SIGNAL(dataStoreChanged()), this, SLOT(updateButtons()));
    }

    updateButtons();
}

void QuickLaunchBar::setApplicationPackageMonitor(ApplicationPackageMonitor *packageMonitor)
{
    this->packageMonitor = packageMonitor;

    connect(packageMonitor, SIGNAL(packageStateUpdated(QSharedPointer<MDesktopEntry>, QString, QString, bool)),
        this, SLOT(updateButtonState(QSharedPointer<MDesktopEntry>, QString, QString, bool)));
}

void QuickLaunchBar::updateButtonState(const QSharedPointer<MDesktopEntry> &desktopEntry, const QString &packageName, const QString &state, bool packageRemovable)
{
    Q_UNUSED(packageName);
    Q_UNUSED(packageRemovable);
    QString desktopEntryPath(desktopEntry->fileName());
    //If there is a quicklaunch button for desktopEntryPath, update it's state
    QList<QSharedPointer<LauncherButton> > buttons = model()->buttons().values();
    QString entryFileName = QFileInfo(desktopEntryPath).fileName();
    for (int i = 0 ; i < buttons.count() ; i++) {
        if (QFileInfo(buttons.at(i)->desktopEntry()).fileName() == entryFileName) {
            buttons.at(i)->setState(Launcher::buttonStateFromPackageState(state));
            break;
        }
    }
}

void QuickLaunchBar::updateProgress(const QString& desktopEntryPath, int already, int total)
{
    //If there is a quicklaunch button for desktopEntryPath, update it's progress
    QList<QSharedPointer<LauncherButton> > buttons = model()->buttons().values();
    QString entryFileName = QFileInfo(desktopEntryPath).fileName();
    for (int i = 0 ; i < buttons.count() ; i++) {
        if (QFileInfo(buttons.at(i)->desktopEntry()).fileName() == entryFileName) {
            buttons.at(i)->setOperationProgress(already, total);
            break;
        }
    }
}

void QuickLaunchBar::updateButtons()
{
    if (dataStore == NULL) {
        model()->setButtons(QMap<int, QSharedPointer<LauncherButton> >());
        return;
    }

    // Temporarily disable the listening of the change signals from the configuration to prevent a recursive call to this method
    dataStore->disconnect(this);

    // Construct a map of buttons and their positions
    QMap<int, QSharedPointer<LauncherButton> > newButtons;

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
    QSharedPointer<MDesktopEntry> desktopEntry(new MDesktopEntry(desktopEntryPath));
    LauncherButton *button = new LauncherButton(desktopEntry);
    button->setObjectName("QuickLaunchBarButton");
    connect(button, SIGNAL(clicked()), this, SIGNAL(launcherButtonClicked()));
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
