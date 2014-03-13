
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

#ifndef LAUNCHERMODEL_H
#define LAUNCHERMODEL_H

#include <QObject>
#include <QSettings>
#include <QFileSystemWatcher>
#include <QDBusServiceWatcher>
#include <QMap>

#include "launcheritem.h"
#include "qobjectlistmodel.h"
#include "lipstickglobal.h"
#include "launchermonitor.h"
#include "launcherdbus.h"


class LIPSTICK_EXPORT LauncherModel : public QObjectListModel
{
    Q_OBJECT
    Q_DISABLE_COPY(LauncherModel)

    Q_PROPERTY(QStringList directories READ directories WRITE setDirectories NOTIFY directoriesChanged)
    Q_PROPERTY(QStringList iconDirectories READ iconDirectories WRITE setIconDirectories NOTIFY iconDirectoriesChanged)

    QFileSystemWatcher _fileSystemWatcher;
    QSettings _launcherSettings;
    QSettings _globalSettings;
    LauncherMonitor _launcherMonitor;
    LauncherDBus _launcherDBus;

    QDBusServiceWatcher _dbusWatcher;
    QMap<QString, QString> _packageNameToDBusService;
    QList<LauncherItem *> _temporaryLaunchers;

private slots:
    void monitoredFileChanged(const QString &changedPath);
    void onFilesUpdated(const QStringList &added, const QStringList &modified, const QStringList &removed);
    void onServiceUnregistered(const QString &serviceName);

public:
    explicit LauncherModel(QObject *parent = 0);
    virtual ~LauncherModel();

    QStringList directories() const;
    void setDirectories(QStringList);

    QStringList iconDirectories() const;
    void setIconDirectories(QStringList);

    void updatingStarted(const QString &packageName, const QString &label,
            const QString &iconPath, QString desktopFile, const QString &serviceName);
    void updatingProgress(const QString &packageName, int progress, const QString &serviceName);
    void updatingFinished(const QString &packageName, const QString &serviceName);

    void requestLaunch(const QString &packageName);

public slots:
    void savePositions();
    void removeTemporaryLaunchers();

signals:
    void directoriesChanged();
    void iconDirectoriesChanged();

private:
    void reorderItems(const QMap<int, LauncherItem *> &itemsWithPositions);
    void loadPositions();
    LauncherItem *itemInModel(const QString &path);
    LauncherItem *packageInModel(const QString &packageName);
    QVariant launcherPos(const QString &path);
    LauncherItem *addItemIfValid(const QString &path, QMap<int, LauncherItem *> &itemsWithPositions);
    void updateItemsWithIcon(const QString &filename, bool existing);
    void updateWatchedDBusServices();
    void setTemporary(LauncherItem *item);
    void unsetTemporary(LauncherItem *item);
    LauncherItem *temporaryItemToReplace();

    friend class Ut_LauncherModel;
};

#endif // LAUNCHERMODEL_H
