
// This file is part of lipstick, a QML desktop library
//
// Copyright (c) 2012 Jolla Ltd.
// Contact: Thomas Perl <thomas.perl@jolla.com>
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

#include "launchermonitor.h"

#include "launcheritem.h"

#include <QDir>

/**
 * Timeout (in milliseconds) to hold back sending updates, so that we can
 * combine multiple updates to icons and desktop file in one go. Also to
 * avoid doing extraneous updates in case files get added/changed/removed
 * in quick succession.
 **/
#define LAUNCHER_MONITOR_HOLDBACK_TIMEOUT_MS 2000


LauncherMonitor::LauncherMonitor(const QString &desktopFilesPath,
        const QString &iconFilesPath)
    : QObject()
    , m_watcher()
    , m_holdbackTimer()
    , m_knownFiles()
    , m_addedFiles()
    , m_modifiedFiles()
    , m_removedFiles()
    , m_desktopFilesPath(desktopFilesPath)
{
    m_holdbackTimer.setSingleShot(true);

    QObject::connect(&m_watcher, SIGNAL(directoryChanged(const QString &)),
            this, SLOT(onDirectoryChanged(const QString &)));
    QObject::connect(&m_watcher, SIGNAL(fileChanged(const QString &)),
            this, SLOT(onFileChanged(const QString &)));
    QObject::connect(&m_holdbackTimer, SIGNAL(timeout()),
            this, SLOT(onHoldbackTimerTimeout()));

    m_iconFilesPaths << iconFilesPath;

    m_watcher.addPaths(m_iconFilesPaths);
    m_watcher.addPath(m_desktopFilesPath);

    // Force initial scan of directories
    // Scan the desktop files first, so that the launcher items are already
    // available by the time the icons will be processed
    onDirectoryChanged(m_desktopFilesPath);
    onDirectoryChanged(iconFilesPath);
}

LauncherMonitor::~LauncherMonitor()
{
}

void LauncherMonitor::start()
{
    // Manually force sending an update (this should be called when signals are
    // hooked up and the model is ready to receive update events)
    m_holdbackTimer.stop();
    onHoldbackTimerTimeout();
}

QStringList LauncherMonitor::directories() const
{
    QStringList result;
    // Right now, we only monitor one directory for .desktop files
    result.append(m_desktopFilesPath);
    return result;
}

QStringList LauncherMonitor::iconDirectories() const
{
    return m_iconFilesPaths;
}

void LauncherMonitor::setIconDirectories(const QStringList &dirs)
{
    QStringList newPaths;
    QStringList::ConstIterator it = dirs.begin();
    while (it != dirs.end()) {
        if (!m_iconFilesPaths.contains(*it)) {
            newPaths << *it;
        } else {
            m_iconFilesPaths.removeAll(*it);
        }
        ++it;
    }

    m_watcher.removePaths(m_iconFilesPaths);

    m_iconFilesPaths = dirs;
    m_watcher.addPaths(newPaths);
    foreach (QString path, newPaths)
        onDirectoryChanged(path);
}

void LauncherMonitor::onDirectoryChanged(const QString &path)
{
    QDir dir(path);
    QStringList seen = dir.entryList();
    QStringList added;
    QStringList removed;
    QStringList &knownFiles = m_knownFiles[path];
    
    // Calculate added and removed files
    foreach (const QString &filename, knownFiles) {
        if (filename.startsWith(".")) {
            continue;
        }

        if (!seen.contains(filename)) {
            removed.append(dir.filePath(filename));
        }
    }
    foreach (const QString &filename, seen) {
        if (filename.startsWith(".")) {
            continue;
        }

        if (!knownFiles.contains(filename)) {
            added.append(dir.filePath(filename));
        }
    }

    // First, stop monitoring all removed files and then tell interested
    // parties that these files have gone.
    // After that, do the same thing for added files.
    if (!removed.isEmpty()) {
        m_watcher.removePaths(removed);

        foreach (const QString &filename, removed) {
            m_modifiedFiles.removeAll(filename);
            if (m_addedFiles.contains(filename)) {
                // We have an "added" notification that's not sent out yet
                // Just remove this notification, as if nothing happened
                //
                // (=> the file has been added and quickly removed again)
                m_addedFiles.removeAll(filename);
            } else {
                m_removedFiles.append(filename);
            }
        }
    }

    if (!added.isEmpty()) {
        m_watcher.addPaths(added);

        foreach (const QString &filename, added) {
            m_modifiedFiles.removeAll(filename);
            if (m_removedFiles.contains(filename)) {
                // We have a "removed" notification that's not sent out yet
                // Just remove this notification, as if nothing happened
                //
                // (=> the file has vanished and re-appeared quickly)
                m_removedFiles.removeAll(filename);
            } else {
                m_addedFiles.append(filename);
            }
        }
    }

    // Schedule updating the launcher icons
    m_holdbackTimer.start(LAUNCHER_MONITOR_HOLDBACK_TIMEOUT_MS);

    knownFiles = seen;
}

void LauncherMonitor::onFileChanged(const QString &path)
{
    m_modifiedFiles.append(path);
    // Schedule updating the launcher icons
    m_holdbackTimer.start(LAUNCHER_MONITOR_HOLDBACK_TIMEOUT_MS);
}

void LauncherMonitor::onHoldbackTimerTimeout()
{
    QStringList modifiedCandidates = m_modifiedFiles;
    foreach (const QString &filename, modifiedCandidates) {
        // This takes care of avoiding sending modifications for two cases:
        //   1. The file was added and then modified
        //   2. The file was modified and then removed
        if (m_addedFiles.contains(filename) || m_removedFiles.contains(filename)) {
            m_modifiedFiles.removeOne(filename);
        }
    }

    if (m_addedFiles.isEmpty() && m_modifiedFiles.isEmpty() && m_removedFiles.isEmpty()) {
        // Nothing to update
        return;
    }

    LAUNCHER_DEBUG("=========");
    LAUNCHER_DEBUG("Added:" << m_addedFiles);
    LAUNCHER_DEBUG("Modified:" << m_modifiedFiles);
    LAUNCHER_DEBUG("Removed:" << m_removedFiles);
    LAUNCHER_DEBUG("=========");

    emit filesUpdated(m_addedFiles, m_modifiedFiles, m_removedFiles);
    m_addedFiles.clear();
    m_modifiedFiles.clear();
    m_removedFiles.clear();
}
