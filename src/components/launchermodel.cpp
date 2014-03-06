
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
#include <QFile>
#include <QSettings>

#include "launchermodel.h"


#define LAUNCHER_APPS_PATH "/usr/share/applications/"

// Make sure to also update this in the .spec file, so it gets
// created whenever lipstick is installed, otherwise monitoring
// will fail and newly-installed icons will not be detected
#define LAUNCHER_ICONS_PATH "/usr/share/icons/hicolor/86x86/apps/"

#define LAUNCHER_KEY_FOR_PATH(path) ("LauncherOrder/" + path)

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

LauncherModel::LauncherModel(QObject *parent) :
    QObjectListModel(parent),
    _fileSystemWatcher(),
    _launcherSettings("nemomobile", "lipstick"),
    _globalSettings("/usr/share/lipstick/lipstick.conf", QSettings::IniFormat),
    _launcherMonitor(LAUNCHER_APPS_PATH, LAUNCHER_ICONS_PATH),
    _launcherDBus(this)
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
}

LauncherModel::~LauncherModel()
{
}

void LauncherModel::onFilesUpdated(const QStringList &added,
        const QStringList &modified, const QStringList &removed)
{
    QMap<int, LauncherItem *> itemsWithPositions;

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
            if (itemInModel(filename) == NULL) {
                LAUNCHER_DEBUG("Trying to add launcher item:" << filename);
                LauncherItem *item = addItemIfValid(filename, itemsWithPositions);

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
                // This "should not" happen...
                qWarning() << "New file already in model:" << filename;
            }
        } else if (isIconFile(filename)) {
            // Icons has been added - find item and update its icon path
            updateItemsWithIcon(filename, true);
        }
    }

    foreach (const QString &filename, modified) {
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

void LauncherModel::installStarted(const QString &packageName, const QString &label,
        const QString &iconPath, const QString &desktopFile)
{
    LauncherItem *item = itemInModel(desktopFile);

    if (!item) {
        item = packageInModel(packageName);
    }

    if (!item) {
        Q_UNUSED(label);
        Q_UNUSED(iconPath);
        // TODO: Create new, temporary icon with label, icon and packagename
        qDebug() << __func__ << "No item found:" << packageName << desktopFile;
        return;
    }

    item->setIsUpdating(true);
    item->setPackageName(packageName);
}

void LauncherModel::installProgress(const QString &packageName, int progress)
{
    LauncherItem *item = packageInModel(packageName);
    if (item) {
        item->setIsUpdating(true);
        item->setUpdatingProgress(progress);
    } else {
        qDebug() << "WARNING:" << __func__ << "package not found:" << packageName;
    }
}

void LauncherModel::installFinished(const QString &packageName)
{
    LauncherItem *item = packageInModel(packageName);
    if (item) {
        item->setIsUpdating(false);
        // TODO: If this was a temporary icon, remove it
    } else {
        qDebug() << "WARNING:" << __func__ << "package not found in model:" << packageName;
        // XXX: Failure?
    }
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
    foreach (LauncherItem *item, *getList<LauncherItem>()) {
        if (item->packageName() == packageName) {
            return item;
        }
    }
    return 0;
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
