
// This file is part of lipstick, a QML desktop library
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License version 2.1 as published by the Free Software Foundation
// and appearing in the file LICENSE.LGPL included in the packaging
// of this file.
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// Copyright (c) 2012, Timur Kristóf <venemo@fedoraproject.org>

#include <QDir>
#include <QFileSystemWatcher>
#include <QDebug>
#include <QSettings>

#include "launchermodel.h"

// Define this if you'd like to see debug messages from the launcher
#ifdef DEBUG_LAUNCHER
#define LAUNCHER_DEBUG(things) qDebug() << Q_FUNC_INFO << things
#else
#define LAUNCHER_DEBUG(things)
#endif

LauncherModel::LauncherModel(QObject *parent) :
    QObjectListModel(parent),
    _fileSystemWatcher(new QFileSystemWatcher(this))
{
    // This is the most common path for .desktop files in most distributions
    QString defaultAppsPath("/usr/share/applications");

    // Setting up the file system wacher
    _fileSystemWatcher->addPath(defaultAppsPath);
    monitoredDirectoryChanged(defaultAppsPath);
    connect(_fileSystemWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(monitoredDirectoryChanged(QString)));
    connect(this, SIGNAL(rowsMoved(const QModelIndex&,int,int,const QModelIndex&,int)), this, SLOT(savePositions()));
    // watch for changes to order
    QSettings launcherSettings("nemomobile", "lipstick");
    _settingsPath = launcherSettings.fileName();
    _fileSystemWatcher->addPath(_settingsPath);
    connect(_fileSystemWatcher, SIGNAL(fileChanged(QString)), this, SLOT(monitoredFileChanged(QString)));
}

LauncherModel::~LauncherModel()
{
}

void LauncherModel::monitoredDirectoryChanged(QString changedPath)
{
    QDir directory(changedPath);
    directory.setFilter(QDir::Files);
    QFileInfoList fileInfoList = directory.entryInfoList();
    QList<LauncherItem *> *currentLauncherList = getList<LauncherItem>();

    // Finding removed desktop entries
    foreach (LauncherItem *item, *currentLauncherList) {
        if (!item->filePath().startsWith(changedPath))
            continue;

        bool foundOnDisk = false;
        foreach (const QFileInfo &fileInfo, fileInfoList) {
            if (fileInfo.absoluteFilePath() == item->filePath())
                foundOnDisk = true;
        }

        if (!foundOnDisk) {
            LAUNCHER_DEBUG(item->filePath() << " removed from disk");
            removeItem(item);
        }
    }

    QMap<int, LauncherItem *> itemsWithPositions;
    QSettings launcherSettings("nemomobile", "lipstick");
    QSettings globalSettings("/usr/share/lipstick/lipstick.conf", QSettings::IniFormat);

    // Finding newly added desktop entries
    foreach (const QFileInfo &fileInfo, fileInfoList) {
        // Skip files which are not desktop entries
        if (!fileInfo.fileName().endsWith(".desktop"))
            continue;

        bool foundInModel = false;
        foreach (LauncherItem *item, *currentLauncherList) {
            if (fileInfo.absoluteFilePath() == item->filePath()) {
                foundInModel = true;
                break;
            }
        }

        if (!foundInModel) {
            LAUNCHER_DEBUG("Creating LauncherItem for desktop entry" << fileInfo.absoluteFilePath());
            LauncherItem *item = new LauncherItem(fileInfo.absoluteFilePath(), this);

            if (!item->isValid()) {
                LAUNCHER_DEBUG("Item " << fileInfo.absoluteFilePath() << " is not valid");
                delete item;
                continue;
            }

            if (!item->shouldDisplay()) {
                LAUNCHER_DEBUG("Item " << fileInfo.absoluteFilePath() << " should not be displayed");
                delete item;
                continue;
            }

            this->addItem(item);

            QVariant pos = launcherSettings.value("LauncherOrder/" + item->filePath());

            // fall back to vendor configuration if the user hasn't specified a location
            if (!pos.isValid())
                pos = globalSettings.value("LauncherOrder/" + item->filePath());

            if (!pos.isValid())
                continue;

            int gridPos = pos.toInt();
            itemsWithPositions.insert(gridPos, item);
            LAUNCHER_DEBUG("Planned move of " << item->filePath() << " to " << gridPos);
        }
    }

    reorderItems(itemsWithPositions);

    savePositions();
}

void LauncherModel::monitoredFileChanged(QString changedPath)
{
    if (changedPath != _settingsPath)
        return;

    // Settings file changed - update positions.
    QMap<int, LauncherItem *> itemsWithPositions;
    QSettings launcherSettings("nemomobile", "lipstick");
    QSettings globalSettings("/usr/share/lipstick/lipstick.conf", QSettings::IniFormat);

    QList<LauncherItem *> *currentLauncherList = getList<LauncherItem>();
    foreach (LauncherItem *item, *currentLauncherList) {
        QVariant pos = launcherSettings.value("LauncherOrder/" + item->filePath());

        // fall back to vendor configuration if the user hasn't specified a location
        if (!pos.isValid())
            pos = globalSettings.value("LauncherOrder/" + item->filePath());

        if (!pos.isValid())
            continue;

        int gridPos = pos.toInt();
        itemsWithPositions.insert(gridPos, item);
    }

    reorderItems(itemsWithPositions);
}

void LauncherModel::reorderItems(const QMap<int, LauncherItem *> &itemsWithPositions)
{
    // QMap is key-ordered, the int here is the desired position in the launcher we want the item to appear
    // so, we'll iterate from the lowest desired position to the highest, and move the items there.
    for (QMap<int, LauncherItem *>::ConstIterator it = itemsWithPositions.constBegin();
         it != itemsWithPositions.constEnd(); ++it) {
        LauncherItem *item = it.value();
        int gridPos = it.key();
        LAUNCHER_DEBUG("Moving " << item->filePath() << " to " << gridPos);

        if (gridPos < 0 || gridPos >= itemCount()) {
            qWarning() << Q_FUNC_INFO << "Invalid planned position for " << item->filePath();
            continue;
        }

        int currentPos = indexOf(item);
        Q_ASSERT(currentPos >= 0);
        if (currentPos == -1)
            continue;

        if (gridPos == currentPos)
            continue;

        move(currentPos, gridPos);
    }
}

QStringList LauncherModel::directories() const
{
    return _fileSystemWatcher->directories();
}

void LauncherModel::setDirectories(QStringList newDirectories)
{
    _fileSystemWatcher->removePaths(_fileSystemWatcher->directories());

    foreach (const QString &path, newDirectories) {
        if (!path.startsWith('/')) {
            LAUNCHER_DEBUG(Q_FUNC_INFO << "Not an absolute path, not adding" << path);
            continue;
        }

        _fileSystemWatcher->addPath(path);
        monitoredDirectoryChanged(path);
    }

    emit this->directoriesChanged();
}

void LauncherModel::savePositions()
{
    QSettings launcherSettings("nemomobile", "lipstick");
    _fileSystemWatcher->removePath(launcherSettings.fileName());
    QList<LauncherItem *> *currentLauncherList = getList<LauncherItem>();

    int pos = 0;
    foreach (LauncherItem *item, *currentLauncherList) {
        launcherSettings.setValue("LauncherOrder/" + item->filePath(), pos);
        ++pos;
    }

    launcherSettings.sync();
    _fileSystemWatcher->addPath(launcherSettings.fileName());
}

