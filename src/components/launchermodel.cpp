
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

        if (fileInfoList.end() == std::find_if(
                fileInfoList.begin(),
                fileInfoList.end(),
                [item](QFileInfo fileInfo) -> bool { return item->filePath() == fileInfo.fileName(); }))
            removeItem(item);
    }

    // Finding newly added desktop entries
    foreach (const QFileInfo &fileInfo, fileInfoList) {
        // Skip files which are not desktop entries
        if (!fileInfo.fileName().endsWith(".desktop"))
            continue;

        if (currentLauncherList->end() == std::find_if(
            currentLauncherList->begin(),
            currentLauncherList->end(),
            [fileInfo](LauncherItem* item) -> bool { return item->filePath() == fileInfo.fileName(); })) {

            LAUNCHER_DEBUG("Creating LauncherItem for desktop entry" << fileInfo.absoluteFilePath());
            LauncherItem *item = new LauncherItem(fileInfo.absoluteFilePath(), this);

            if (item->isValid())
                this->addItem(item);
            else
                delete item;
        }
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
