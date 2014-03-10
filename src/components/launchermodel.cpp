
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
#include <QDBusConnection>
#include <QDebug>
#include <QFile>
#include <QSettings>

#include "launchermodel.h"


#define LAUNCHER_APPS_PATH "/usr/share/applications/"

// Make sure to also update this in the .spec file, so it gets
// created whenever lipstick is installed, otherwise monitoring
// will fail and newly-installed icons will not be detected
#define LAUNCHER_ICONS_PATH "/usr/share/icons/hicolor/86x86/apps/"

#define LAUNCHER_KEY_FOR_PATH(path) ("LauncherOrder/" + path)

// Time in millseconds to wait before removing temporary launchers
#define LAUNCHER_UPDATING_REMOVAL_HOLDBACK_MS 3000

static inline bool isDesktopFile(const QString &filename)
{
    return filename.startsWith(LAUNCHER_APPS_PATH) && filename.endsWith(".desktop");
}

static inline bool isIconFile(const QString &filename)
{
    // TODO: Possibly support other file types
    return filename.startsWith(QLatin1Char('/')) && filename.endsWith(".png");
}

static inline QString iconIdFromFilename(const QString &filename)
{
    int start = filename.lastIndexOf('/') + 1;
    int end = filename.lastIndexOf('.');

    if (start == -1 || end == -1) {
        // something's fishy..
        return QString();
    }

    return filename.mid(start, end - start);
}

static inline QString filenameFromIconId(const QString &filename, const QString &path)
{
    return QString("%1%2%3").arg(path).arg(filename).arg(".png");
}

static inline bool isVisibleDesktopFile(const QString &filename)
{
    LauncherItem item(filename);

    return item.isValid() && item.shouldDisplay();
}

LauncherModel::LauncherModel(QObject *parent) :
    QObjectListModel(parent),
    _fileSystemWatcher(),
    _launcherSettings("nemomobile", "lipstick"),
    _globalSettings("/usr/share/lipstick/lipstick.conf", QSettings::IniFormat),
    _launcherMonitor(LAUNCHER_APPS_PATH, LAUNCHER_ICONS_PATH),
    _launcherDBus(this),
    _dbusWatcher(this),
    _packageNameToDBusService(),
    _temporaryLaunchers()
{
    // Set up the monitor for icon and desktop file changes
    connect(&_launcherMonitor, SIGNAL(filesUpdated(const QStringList &, const QStringList &, const QStringList &)),
            this, SLOT(onFilesUpdated(const QStringList &, const QStringList &, const QStringList &)));

    // Start monitoring
    _launcherMonitor.start();

    // Save order of icons when model is changed
    connect(this, SIGNAL(rowsMoved(const QModelIndex&,int,int,const QModelIndex&,int)), this, SLOT(savePositions()));

    // Watch for changes to the item order settings file
    _fileSystemWatcher.addPath(_launcherSettings.fileName());
    connect(&_fileSystemWatcher, SIGNAL(fileChanged(QString)), this, SLOT(monitoredFileChanged(QString)));

    // Used to watch for owner changes during installation progress
    _dbusWatcher.setConnection(QDBusConnection::sessionBus());
    _dbusWatcher.setWatchMode(QDBusServiceWatcher::WatchForUnregistration);

    connect(&_dbusWatcher, SIGNAL(serviceUnregistered(const QString &)),
            this, SLOT(onServiceUnregistered(const QString &)));
}

LauncherModel::~LauncherModel()
{
}

void LauncherModel::onFilesUpdated(const QStringList &added,
        const QStringList &modified, const QStringList &removed)
{
    QMap<int, LauncherItem *> itemsWithPositions;
    QStringList modded = modified;

    // First, remove all removed launcher items before adding new ones
    foreach (const QString &filename, removed) {
        if (isDesktopFile(filename)) {
            // Desktop file has been removed - remove launcher
            LauncherItem *item = itemInModel(filename);
            if (item != NULL) {
                LAUNCHER_DEBUG("Removing launcher item:" << filename);
                removeItem(item);
            }
        } else if (isIconFile(filename)) {
            // Icons has been removed - find item and clear its icon path
            updateItemsWithIcon(filename, false);
        }
    }

    foreach (const QString &filename, added) {
        if (isDesktopFile(filename)) {
            // New desktop file appeared - add launcher
            LauncherItem *item = itemInModel(filename);

            // Check if there is a temporary launcher item, and if so, assume that
            // the newly-appeared file (if it is visible) will replace the temporary
            // launcher. In general, this should not happen if the app is properly
            // packaged (desktop file shares basename with packagename), but in some
            // cases, this is better than having the temporary and non-temporary in
            // place at the same time.
            if (item == NULL && _temporaryLaunchers.length() == 1 &&
                    isVisibleDesktopFile(filename)) {
                // Replace the single temporary launcher with the newly-added icon
                item = _temporaryLaunchers.first();

                qWarning() << "Applying heuristics:" << filename <<
                    "is the launcher item for" << item->packageName();
                item->setFilePath(filename);
            }

            if (item == NULL) {
                LAUNCHER_DEBUG("Trying to add launcher item:" << filename);
                item = addItemIfValid(filename, itemsWithPositions);

                if (item != NULL) {
                    // Try to look up an already-installed icon in the icons directory
                    foreach (const QString &iconPath, _launcherMonitor.iconDirectories()) {
                        QString iconname = filenameFromIconId(item->getOriginalIconId(), iconPath);
                        if (QFile(iconname).exists()) {
                            LAUNCHER_DEBUG("Loading existing icon:" << iconname);
                            updateItemsWithIcon(iconname, true);
                            break;
                        }
                    }
                }
            } else {
                // This case happens if a .desktop file is found as new, but we
                // already have an entry for it, which usually means it was a
                // temporary launcher that we now successfully can replace.
                qWarning() << "Expected file arrives:" << filename;
                unsetTemporary(item);

                // Act as if this filename has been modified, so we can update
                // it below (e.g. turn a temporary item into a permanent one)
                modded << filename;
            }
        } else if (isIconFile(filename)) {
            // Icons has been added - find item and update its icon path
            updateItemsWithIcon(filename, true);
        }
    }

    foreach (const QString &filename, modded) {
        if (isDesktopFile(filename)) {
            // Desktop file has been updated - update launcher
            LauncherItem *item = itemInModel(filename);
            if (item != NULL) {
                bool isValid = item->isStillValid() && item->shouldDisplay();
                if (!isValid) {
                    // File has changed in such a way (e.g. Hidden=true) that
                    // it now should become invisible again
                    removeItem(item);
                } else {
                    // File has been updated and is still valid; check if we
                    // might need to auto-update the icon file
                    if (item->iconFilename().isEmpty()) {
                        foreach (const QString &iconPath, _launcherMonitor.iconDirectories()) {
                            QString filename = filenameFromIconId(item->getOriginalIconId(), iconPath);
                            LAUNCHER_DEBUG("Desktop file changed, checking for:" << filename);
                            if (QFile(filename).exists()) {
                                updateItemsWithIcon(filename, true);
                                break;
                            }
                        }
                    }
                }
            } else {
                // No item yet (maybe it had Hidden=true before), try to see if
                // we should show the item now
                addItemIfValid(filename, itemsWithPositions);
            }
        } else if (isIconFile(filename)) {
            // Icons has been updated - find item and update its icon path
            updateItemsWithIcon(filename, true);
        }
    }

    reorderItems(itemsWithPositions);
    savePositions();
}

void LauncherModel::updateItemsWithIcon(const QString &filename, bool existing)
{
    QString iconId = iconIdFromFilename(filename);

    LAUNCHER_DEBUG("updateItemsWithIcon: filename=" << filename << ", existing=" << existing << ", id=" << iconId);

    foreach (LauncherItem *item, *getList<LauncherItem>()) {
        const QString &currentId = item->getOriginalIconId();
        if (currentId.isEmpty()) {
            continue;
        }

        if (!existing && filename == item->iconFilename()) {
            // File is currently used as icon, but has been removed
            LAUNCHER_DEBUG("Icon vanished, removing:" << filename);
            item->setIconFilename("");
        } else if (existing) {
            if ((filename == currentId) /* absolute file path in .desktop file */ ||
                    (iconId == currentId) /* icon id matches */) {
                LAUNCHER_DEBUG("Icon was added or updated:" << filename);
                item->setIconFilename(filename);
            }
        }
    }
}

void LauncherModel::monitoredFileChanged(const QString &changedPath)
{
    if (changedPath == _launcherSettings.fileName()) {
        loadPositions();
    } else {
        qWarning() << "Unknown monitored file in LauncherModel:" << changedPath;
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
    return _launcherMonitor.directories();
}

void LauncherModel::setDirectories(QStringList newDirectories)
{
    Q_UNUSED(newDirectories);
    qWarning() << "Changing the directories of desktop files to watch not supported";
    // TODO: Maybe add support for this to LauncherMonitor and call from here
    //emit this->directoriesChanged();
}

QStringList LauncherModel::iconDirectories() const
{
    return _launcherMonitor.iconDirectories();
}

void LauncherModel::setIconDirectories(QStringList newDirectories)
{
    if (!newDirectories.contains(LAUNCHER_ICONS_PATH))
        newDirectories << LAUNCHER_ICONS_PATH;
    _launcherMonitor.setIconDirectories(newDirectories);
    emit iconDirectoriesChanged();
}

static QString desktopFileFromPackageName(const QString &packageName)
{
    // Using the package name as base name for the desktop file is a good
    // heuristic, and usually works fine.
    return QString(LAUNCHER_APPS_PATH) + packageName + ".desktop";
}

void LauncherModel::installStarted(const QString &packageName, const QString &label,
        const QString &iconPath, QString desktopFile, const QString &serviceName)
{
    LAUNCHER_DEBUG("Installation started:" << packageName << label
            << iconPath << desktopFile);

    // Remember which service notified us about this package, so we can
    // clean up existing updates when the service vanishes from D-Bus.
    _packageNameToDBusService[packageName] = serviceName;
    _dbusWatcher.addWatchedService(serviceName);

    if (desktopFile.isEmpty()) {
        desktopFile = desktopFileFromPackageName(packageName);
    }

    LauncherItem *item = itemInModel(desktopFile);

    if (!item) {
        item = packageInModel(packageName);
    }

    // Calling installStarted on an existing temporary icon should
    // update the internal state of the temporary icon (and if the
    // .desktop file exists, make the icon non-temporary).
    if (item && item->isTemporary()) {
        if (!label.isEmpty()) {
            item->setCustomTitle(label);
        }

        if (!iconPath.isEmpty()) {
            item->setIconFilename(iconPath);
        }

        if (!desktopFile.isEmpty()) {
            item->setFilePath(desktopFile);
        }

        if (QFile(desktopFile).exists()) {
            // The file has appeared - remove temporary flag
            unsetTemporary(item);
        }
    }

    if (!item) {
        // Newly-installed package: Create temporary icon with label and icon
        item = new LauncherItem(packageName, label, iconPath, desktopFile, this);
        setTemporary(item);
        addItem(item);
    }

    item->setUpdatingProgress(-1);
    item->setIsUpdating(true);
    item->setPackageName(packageName);
}

void LauncherModel::installProgress(const QString &packageName, int progress,
        const QString &serviceName)
{
    LAUNCHER_DEBUG("Installation progress:" << packageName << progress);

    QString expectedServiceName = _packageNameToDBusService[packageName];
    if (expectedServiceName != serviceName) {
        qWarning() << "Got update from" << serviceName <<
                      "but expected update from" << expectedServiceName;
    }

    LauncherItem *item = packageInModel(packageName);

    if (!item) {
        qWarning() << "Package not found in model:" << packageName;
    }

    item->setUpdatingProgress(progress);
    item->setIsUpdating(true);
}

void LauncherModel::installFinished(const QString &packageName,
        const QString &serviceName)
{
    LAUNCHER_DEBUG("Installation finished:" << packageName);

    QString expectedServiceName = _packageNameToDBusService[packageName];
    if (expectedServiceName != serviceName) {
        qWarning() << "Got update from" << serviceName <<
                      "but expected update from" << expectedServiceName;
    }

    _packageNameToDBusService.remove(packageName);
    updateWatchedDBusServices();

    LauncherItem *item = packageInModel(packageName);

    if (!item) {
        qWarning() << "Package not found in model:" << packageName;
        return;
    }

    item->setIsUpdating(false);
    item->setUpdatingProgress(-1);
    if (item->isTemporary()) {
        // Schedule removal of temporary icons
        QTimer::singleShot(LAUNCHER_UPDATING_REMOVAL_HOLDBACK_MS,
                this, SLOT(removeTemporaryLaunchers()));
    }
}

void LauncherModel::updateWatchedDBusServices()
{
    QStringList requiredServices = _packageNameToDBusService.values();

    foreach (const QString &service, _dbusWatcher.watchedServices()) {
        if (!requiredServices.contains(service)) {
            LAUNCHER_DEBUG("Don't need to watch service anymore:" << service);
            _dbusWatcher.removeWatchedService(service);
        }
    }
}

void LauncherModel::onServiceUnregistered(const QString &serviceName)
{
    qWarning() << "Service" << serviceName << "vanished";
    _dbusWatcher.removeWatchedService(serviceName);

    QStringList packagesToRemove;
    QMap<QString, QString>::iterator it;
    for (it = _packageNameToDBusService.begin(); it != _packageNameToDBusService.end(); ++it) {
        if (it.value() == serviceName) {
            qWarning() << "Service" << serviceName << "was active for" << it.key();
            packagesToRemove << it.key();
        }
    }

    foreach (const QString &packageName, packagesToRemove) {
        LAUNCHER_DEBUG("Fabricating installFinished for" << packageName);
        installFinished(packageName, serviceName);
    }
}

void LauncherModel::removeTemporaryLaunchers()
{
    QList<LauncherItem *> iterationCopy = _temporaryLaunchers;
    foreach (LauncherItem *item, iterationCopy) {
        if (!item->isUpdating()) {
            // Temporary item that is not updating at the moment
            LAUNCHER_DEBUG("Removing temporary launcher");
            // Will remove it from _temporaryLaunchers
            unsetTemporary(item);
            removeItem(item);
        }
    }
}

void LauncherModel::requestLaunch(const QString &packageName)
{
    // Send launch request via D-Bus, so interested parties can act upon it
    _launcherDBus.requestLaunch(packageName);
}

void LauncherModel::savePositions()
{
    _fileSystemWatcher.removePath(_launcherSettings.fileName());
    _launcherSettings.remove("LauncherOrder");
    QList<LauncherItem *> *currentLauncherList = getList<LauncherItem>();

    int pos = 0;
    foreach (LauncherItem *item, *currentLauncherList) {
        _launcherSettings.setValue(LAUNCHER_KEY_FOR_PATH(item->filePath()), pos);
        ++pos;
    }

    _launcherSettings.sync();
    _fileSystemWatcher.addPath(_launcherSettings.fileName());
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

LauncherItem *LauncherModel::packageInModel(const QString &packageName)
{
    QList<LauncherItem *> *list = getList<LauncherItem>();

    QList<LauncherItem *>::const_iterator it = list->constEnd();
    while (it != list->constBegin()) {
        --it;

        if ((*it)->packageName() == packageName) {
            return *it;
        }
    }

    // Fall back to trying to find the launcher via the .desktop file
    return itemInModel(desktopFileFromPackageName(packageName));
}

QVariant LauncherModel::launcherPos(const QString &path)
{
    QString key = LAUNCHER_KEY_FOR_PATH(path);

    if (_launcherSettings.contains(key)) {
        return _launcherSettings.value(key);
    }

    // fall back to vendor configuration if the user hasn't specified a location
    return _globalSettings.value(key);
}

LauncherItem *LauncherModel::addItemIfValid(const QString &path, QMap<int, LauncherItem *> &itemsWithPositions)
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
        item = NULL;
    }

    return item;
}

void LauncherModel::setTemporary(LauncherItem *item)
{
    if (!item->isTemporary()) {
        item->setIsTemporary(true);
        _temporaryLaunchers.append(item);
    }
}

void LauncherModel::unsetTemporary(LauncherItem *item)
{
    if (item->isTemporary()) {
        item->setIsTemporary(false);
        _temporaryLaunchers.removeOne(item);
    }
}
