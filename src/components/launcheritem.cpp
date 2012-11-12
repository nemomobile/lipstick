
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
// Copyright (c) 2011, Robin Burchell
// Copyright (c) 2012, Timur Kristóf <venemo@fedoraproject.org>

#include <QDebug>
#include <QProcess>
#include <QFile>
#include <QDir>
#include <QSettings>
#include <QTimer>
#include <mlite/mdesktopentry.h>

#ifdef HAVE_CONTENTACTION
#include <contentaction.h>
#endif

#include "launcheritem.h"
#include "xtools/homewindowmonitor.h"

// Define this if you'd like to see debug messages from the launcher
#ifdef DEBUG_LAUNCHER
#define LAUNCHER_DEBUG(things) qDebug() << Q_FUNC_INFO << things
#else
#define LAUNCHER_DEBUG(things)
#endif

LauncherItem::LauncherItem(const QString &filePath, QObject *parent)
    : QObject(parent)
    , _isLaunching(false)
{
    if (!filePath.isEmpty()) {
        setFilePath(filePath);
    }

    // TODO: instead of this, match the PID of the window thumbnails with the launcher processes
    // Launching animation will be disabled if the window of the launched app shows up
    connect(HomeWindowMonitor::instance(), SIGNAL(isHomeWindowOnTopChanged()), this, SLOT(disableIsLaunching()));
}

LauncherItem::~LauncherItem()
{
}

void LauncherItem::setFilePath(const QString &filePath)
{
    if (!filePath.isEmpty()) {
        _desktopEntry = QSharedPointer<MDesktopEntry>(new MDesktopEntry(filePath));
    } else {
        _desktopEntry.clear();
    }

    emit this->itemChanged();
}

QString LauncherItem::filePath() const
{
    return !_desktopEntry.isNull() ? _desktopEntry->fileName() : QString();
}

QString LauncherItem::title() const
{
    return !_desktopEntry.isNull() ? _desktopEntry->name() : QString();
}

QString LauncherItem::entryType() const
{
    return !_desktopEntry.isNull() ? _desktopEntry->type() : QString();
}

QString LauncherItem::iconId() const
{
    return !_desktopEntry.isNull() ? _desktopEntry->icon() : QString();
}

QStringList LauncherItem::desktopCategories() const
{
    return !_desktopEntry.isNull() ? _desktopEntry->categories() : QStringList();
}

bool LauncherItem::shouldDisplay() const
{
    return !_desktopEntry.isNull() ? !_desktopEntry->noDisplay() : false;
}

bool LauncherItem::isValid() const
{
    return !_desktopEntry.isNull() ? _desktopEntry->isValid() : false;
}

bool LauncherItem::isLaunching() const
{
    return _isLaunching;
}

void LauncherItem::disableIsLaunching()
{
    if (!_isLaunching)
        return; // prevent spurious signals from all delegates
    _isLaunching = false;
    emit this->isLaunchingChanged();
}

void LauncherItem::launchApplication()
{
    if (_desktopEntry.isNull())
        return;

#if defined(HAVE_CONTENTACTION)
    LAUNCHER_DEBUG("launching content action for" << _desktopEntry->name());
    ContentAction::Action action = ContentAction::Action::launcherAction(_desktopEntry, QStringList());
    action.trigger();
#else
    LAUNCHER_DEBUG("launching exec line for" << _desktopEntry->name());

    // Get the command text from the desktop entry
    QString commandText = _desktopEntry->exec();

    // Take care of the freedesktop standards things

    commandText.replace(QRegExp("%k"), filePath());
    commandText.replace(QRegExp("%c"), _desktopEntry->name());
    commandText.remove(QRegExp("%[fFuU]"));

    if (!_desktopEntry->icon().isEmpty())
        commandText.replace(QRegExp("%i"), QString("--icon ") + _desktopEntry->icon());

    // DETAILS: http://standards.freedesktop.org/desktop-entry-spec/latest/index.html
    // DETAILS: http://standards.freedesktop.org/desktop-entry-spec/latest/ar01s06.html

    // Launch the application
    QProcess::startDetached(commandText);
#endif

    _isLaunching = true;
    emit this->isLaunchingChanged();

    // TODO: instead of this, match the PID of the window thumbnails with the launcher processes
    // Launching animation will stop after 5 seconds
    QTimer::singleShot(5000, this, SLOT(disableIsLaunching()));
}
