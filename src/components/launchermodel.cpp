
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


#define LAUNCHER_KEY_FOR_PATH(path) ("LauncherOrder/" + path)

LauncherModel::LauncherModel(QObject *parent) :
    QObjectListModel(parent),
    _fileSystemWatcher(new QFileSystemWatcher(this)),
    _launcherSettings("nemomobile", "lipstick"),
    _globalSettings("/usr/share/lipstick/lipstick.conf", QSettings::IniFormat)
{
    // This is the most common path for .desktop files in most distributions
    QString defaultAppsPath("/usr/share/applications");

    // Setting up the file system wacher
    _fileSystemWatcher->addPath(defaultAppsPath);
    monitoredDirectoryChanged(defaultAppsPath);
    connect(_fileSystemWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(monitoredDirectoryChanged(QString)));
    connect(this, SIGNAL(rowsMoved(const QModelIndex&,int,int,const QModelIndex&,int)), this, SLOT(savePositions()));
    // watch for changes to order
    _fileSystemWatcher->addPath(_launcherSettings.fileName());
    connect(_fileSystemWatcher, SIGNAL(fileChanged(QString)), this, SLOT(monitoredFileChanged(QString)));
}

LauncherModel::~LauncherModel()
{
}

void LauncherModel::monitoredDirectoryChanged(const QString &changedPath)
{
    QFileInfoList fileInfoList = QDir(changedPath).entryInfoList(QStringList() << "*.desktop", QDir::Files);

    // Find removed and invalidated desktop entries
    foreach (LauncherItem *item, *getList<LauncherItem>()) {
        bool isValid = false;
        foreach (const QFileInfo &fileInfo, fileInfoList) {
            if (fileInfo.absoluteFilePath() == item->filePath()) {
                isValid = item->isStillValid() && item->shouldDisplay();
                break;
            }
        }

        if (!isValid) {
            LAUNCHER_DEBUG(item->filePath() << "no longer a valid .desktop entry");
            removeItem(item);
        }
    }

    QMap<int, LauncherItem *> itemsWithPositions;

    // Finding newly added desktop entries
    foreach (const QFileInfo &fileInfo, fileInfoList) {
        QString filePath = fileInfo.absoluteFilePath();
        _fileSystemWatcher->addPath(filePath);

        if (itemInModel(filePath) == 0) {
            addItemIfValid(filePath, itemsWithPositions);
        }
    }

    reorderItems(itemsWithPositions);

    savePositions();
}

void LauncherModel::monitoredFileChanged(const QString &changedPath)
{
    if (changedPath == _launcherSettings.fileName()) {
        loadPositions();
    } else {
        LauncherItem *item = itemInModel(changedPath);
        if (item == 0) {
            QMap<int, LauncherItem *> itemsWithPositions;
            addItemIfValid(changedPath, itemsWithPositions);
            reorderItems(itemsWithPositions);
            savePositions();
        } else if (!(item->isStillValid() && item->shouldDisplay())) {
            LAUNCHER_DEBUG(item->filePath() << "no longer a valid .desktop entry");
            removeItem(item);
            savePositions();
        }
    }
}

void LauncherModel::loadPositions()
{
    QMap<int, LauncherItem *> itemsWithPositions;

    _launcherSettings.sync();

    QList<LauncherItem *> *currentLauncherList = getList<LauncherItem>();
    foreach (LauncherItem *item, *currentLauncherList) {
        QVariant pos = launcherPos(item->filePath());

        if (pos.isValid()) {
            int gridPos = pos.toInt();
            itemsWithPositions.insert(gridPos, item);
        }
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
        LAUNCHER_DEBUG("Moving" << item->filePath() << "to" << gridPos);

        if (gridPos < 0 || gridPos >= itemCount()) {
            LAUNCHER_DEBUG("Invalid planned position for" << item->filePath());
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
    _fileSystemWatcher->removePath(_launcherSettings.fileName());
    _launcherSettings.clear();
    QList<LauncherItem *> *currentLauncherList = getList<LauncherItem>();

    int pos = 0;
    foreach (LauncherItem *item, *currentLauncherList) {
        _launcherSettings.setValue(LAUNCHER_KEY_FOR_PATH(item->filePath()), pos);
        ++pos;
    }

    _launcherSettings.sync();
    _fileSystemWatcher->addPath(_launcherSettings.fileName());
}

LauncherItem *LauncherModel::itemInModel(const QString &path)
{
    foreach (LauncherItem *item, *getList<LauncherItem>()) {
        if (item->filePath() == path) {
            return item;
        }
    }
    return 0;
}

QVariant LauncherModel::launcherPos(const QString &path)
{
    QString key = LAUNCHER_KEY_FOR_PATH(path);

    if (_launcherSettings.contains(key)) {
        _launcherSettings.value(key);
    }

    // fall back to vendor configuration if the user hasn't specified a location
    return _globalSettings.value(key);
}

void LauncherModel::addItemIfValid(const QString &path, QMap<int, LauncherItem *> &itemsWithPositions)
{
    LAUNCHER_DEBUG("Creating LauncherItem for desktop entry" << path);
    LauncherItem *item = new LauncherItem(path, this);

    bool isValid = item->isValid();
    bool shouldDisplay = item->shouldDisplay();
    if (isValid && shouldDisplay) {
        addItem(item);

        QVariant pos = launcherPos(item->filePath());

        if (pos.isValid()) {
            int gridPos = pos.toInt();
            itemsWithPositions.insert(gridPos, item);
            LAUNCHER_DEBUG("Planned move of" << item->filePath() << "to" << gridPos);
        }
    } else {
        LAUNCHER_DEBUG("Item" << path << (!isValid ? "is not valid" : "should not be displayed"));
        delete item;
    }
}
