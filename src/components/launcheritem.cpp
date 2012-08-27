
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
#include <mlite/mdesktopentry.h>

#ifdef HAVE_CONTENTACTION
#include <contentaction.h>
#endif

#include "launcheritem.h"

// Define this if you'd like to see debug messages from the launcher
#ifdef DEBUG_LAUNCHER
#define LAUNCHER_DEBUG(things) qDebug() << Q_FUNC_INFO << things
#else
#define LAUNCHER_DEBUG(things)
#endif

static bool iconNameSizeSortHelper(const QString &s1, const QString &s2)
{
    int x1 = s1.indexOf(QChar('x'));
    int x2 = s2.indexOf(QChar('x'));

    if (x1 == -1)
        return true;
    if (x2 == -1)
        return false;

    int ss1 = s1.left(x1).toInt();
    int ss2 = s2.left(x2).toInt();

    if (!ss1)
        return true;
    if (!ss2)
        return false;

    // The > is NOT a mistake, we need the higher res things before the lower res things
    return ss1 > ss2;
}

static QString findIconHelper(const QString &pathName, const QString &icon)
{
    QStringList extensions;
    extensions << "";
    extensions << ".png";
    extensions << ".svg";
    //    qDebug() << "Trying " << pathName << " for " << icon;

    foreach (const QString &extension, extensions) {
        if (QFile::exists(pathName + QDir::separator() + icon + extension))
            return pathName + QDir::separator() + icon + extension;
    }

    QStringList entryList = QDir(pathName).entryList(QStringList(), QDir::AllDirs | QDir::NoDotAndDotDot);
    foreach (const QString &dir, entryList) {
        QString retval = findIconHelper(QDir(pathName).absoluteFilePath(dir), icon);
        if (!retval.isNull())
            return retval;
    }

    // all else failed
    return QString();
}

static QString getIconPath(const QString &name)
{
    if (QFile::exists(name)) {
        // fast path: file given
        return name;
    }

    QSettings settings("MeeGo", "IconCache");
    QString cachedPath = settings.value(name).toString();

    if (!QFile::exists(cachedPath))
    {
        LAUNCHER_DEBUG("Negative cache hit for " << name << " to " << cachedPath);
    }
    else
    {
        return cachedPath;
    }

    QString retval;
    QStringList themes = QDir("/usr/share/themes").entryList(QStringList(), QDir::AllDirs | QDir::NoDotAndDotDot);

    if (!themes.isEmpty())
    {
        // TODO: look up active theme in gconf and set it to the first search path, don't hardcode it.
        // TODO: would be nice to investigate if our fallback behaviour is actually correct, too, but meh.

        if (themes.contains("n900de") && themes.at(0) != "n900de")
        {
            themes.removeAll("n900de");
            themes.insert(0, "n900de");
        }

        foreach (const QString &theme, themes)
        {
            retval = findIconHelper("/usr/share/themes/" + theme, name);

            if (!retval.isNull())
            {
                if (QFile::exists(retval))
                {
                    settings.setValue(name, retval);
                    return retval;
                }
            }
        }
    }

    // they also seem to get plonked here
    retval = findIconHelper("/usr/share/pixmaps/", name);

    if (!retval.isNull())
    {
        if (QFile::exists(retval))
        {
            settings.setValue(name, retval);
            return retval;
        }
    }


    // I hate all application developers

    // Going through all folders in here too
    QStringList hiColorSizes = QDir("/usr/share/icons/hicolor").entryList(QStringList(), QDir::AllDirs | QDir::NoDotAndDotDot);
    // Ordering them in a way that scalable comes first and then resolutions in descending order
    qSort(hiColorSizes.begin(), hiColorSizes.end(), iconNameSizeSortHelper);

    if (!hiColorSizes.isEmpty())
    {
        foreach (const QString &hiColorSize, hiColorSizes)
        {
            retval = findIconHelper("/usr/share/icons/hicolor/" + hiColorSize, name);

            if (!retval.isNull())
            {
                if (QFile::exists(retval))
                {
                    settings.setValue(name, retval);
                    return retval;
                }
            }
        }
    }

    // If everything fails, return an empty string
    return QString();
}

LauncherItem::LauncherItem(const QString &path, QObject *parent)
    : QObject(parent),
      _desktopEntry(new MDesktopEntry(path))
{
    emit this->itemChanged();
}

LauncherItem::~LauncherItem()
{
}

QString LauncherItem::filePath() const
{
    return _desktopEntry->fileName();
}

QString LauncherItem::title() const
{
    return _desktopEntry->name();
}

QString LauncherItem::entryType() const
{
    return _desktopEntry->type();
}

QString LauncherItem::iconFilePath() const
{
    LAUNCHER_DEBUG("icon path is:" << _desktopEntry->icon());

    if (_desktopEntry->icon().length() == 0)
    {
        LAUNCHER_DEBUG("desktop entry has empty icon setting:" << _desktopEntry->path());
        return QString();
    }

    QString path = getIconPath(_desktopEntry->icon());

    if (path.length() == 0)
    {
        LAUNCHER_DEBUG("could not find icon for iconname:" << _desktopEntry->icon());
        return QString();
    }

    LAUNCHER_DEBUG("icon file found for iconname:" << _desktopEntry->icon() << path);
    return "file://" + path;
}

QStringList LauncherItem::desktopCategories() const
{
    return _desktopEntry->categories();
}

bool LauncherItem::shouldDisplay() const
{
    return !_desktopEntry->noDisplay();
}

bool LauncherItem::isValid() const
{
    return _desktopEntry->isValid();
}

void LauncherItem::launchApplication() const
{
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
}
