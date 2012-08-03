/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <QProcess>
#include <QtDeclarative>
#include <QCryptographicHash>
#include <QFile>
#include <QDir>
#include <QSettings>

#include <mdesktopentry.h>

#ifdef HAS_CONTENTACTION
#include <contentaction.h>
#endif

#include "launcheritem.h"

#define WIDTH_KEY "Desktop Entry/X-MEEGO-APP-HOME-WIDTH"
#define HEIGHT_KEY "Desktop Entry/X-MEEGO-APP-HOME-HEIGHT"
#define PRIORITY_KEY "Desktop Entry/X-MEEGO-APP-HOME-PRIORITY"
#define ROW_KEY "Desktop Entry/X-MEEGO-APP-HOME-ROW"
#define COLUMN_KEY "Desktop Entry/X-MEEGO-APP-HOME-COLUMN"
#define PAGE_KEY "Desktop Entry/X-MEEGO-APP-HOME-PAGE"

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

    if (!QFile::exists(cachedPath)) {
        qDebug() << Q_FUNC_INFO << "Negative cache hit for " << name << " to " << cachedPath;
    } else {
        return cachedPath;
    }

    QStringList themes = QDir("/usr/share/themes").entryList(QStringList(), QDir::AllDirs | QDir::NoDotAndDotDot);
    if (themes.isEmpty())
        return QString();

    // TODO: look up active theme in gconf and set it to the first search path, don't hardcode it.
    // TODO: would be nice to investigate if our fallback behaviour is actually correct, too, but meh.
    if (themes.contains("n900de") &&
        themes.at(0) != "n900de") {
        themes.removeAll("n900de");
        themes.insert(0, "n900de");
    }

    foreach (const QString &theme, themes) {
        QString retval = findIconHelper("/usr/share/themes/" + theme, name);
        if (!retval.isNull()) {
            settings.setValue(name, retval);
            return retval;
        }
    }

    // they also seem to get plonked here
    QString retval = findIconHelper("/usr/share/pixmaps/", name);
    if (!retval.isNull()) {
        settings.setValue(name, retval);
        return retval;
    }

    // I hate all application developers
    retval = findIconHelper("/usr/share/icons/hicolor/64x64/", name);
    if (!retval.isNull()) {
        settings.setValue(name, retval);
        return retval;
    }

    return QString();
}

LauncherItem::LauncherItem(const QString &fileName, QObject *parent)
    : QObject(parent)
    , m_filename(fileName)
    , m_entry(QSharedPointer<MDesktopEntry>(new MDesktopEntry(fileName)))
    , m_pid(0)
    , m_wid(0)
    , m_assigned(false)
{
    ///Set the id:
    QFile file(m_filename);
    file.open(QIODevice::ReadOnly);
    if(!file.isOpen()) return;
    QString hash = QCryptographicHash::hash(file.readAll(),QCryptographicHash::Md5);
    file.close();
    m_id = hash;
}

LauncherItem::~LauncherItem()
{
}

QString LauncherItem::id() const {
    return m_id;
}

bool LauncherItem::isValid() const {
    QStringList onlyShowIn = m_entry->onlyShowIn();
    if (!onlyShowIn.isEmpty() && !onlyShowIn.contains("X-MEEGO") &&
        !onlyShowIn.contains("X-MEEGO-HS"))
        return false;

    QStringList notShowIn = m_entry->notShowIn();
    if (!notShowIn.isEmpty() && (notShowIn.contains("X-MEEGO") ||
                                 notShowIn.contains("X-MEEGO-HS")))
        return false;

    return m_entry->isValid();
}

QString LauncherItem::type() const {
    return m_entry->type();
}

QString LauncherItem::title() const {
    return m_entry->name();
}

QString LauncherItem::comment() const {
    return m_entry->comment();
}

QString LauncherItem::icon() const {
    return getIconPath(m_entry->icon());
}

QString LauncherItem::exec() const {
    return m_entry->exec();
}

QStringList LauncherItem::categories() const {
    return m_entry->categories();
}

QString LauncherItem::filename() const {
    return LauncherItem::m_filename;
}

int LauncherItem::wid() const
{
    return m_wid;
}

void LauncherItem::setWid(int wid) {
    m_wid = wid;
}

int LauncherItem::pid()
{
    return m_pid;
}

void LauncherItem::setPid(int pid)
{
    m_pid = pid;
}

bool LauncherItem::nodisplay() const {
    return m_entry->noDisplay();
}

void LauncherItem::launch() const
{
#ifndef HAS_CONTENTACTION
    // fallback code: contentaction not available
    QString cmd = exec();

    // http://standards.freedesktop.org/desktop-entry-spec/latest/ar01s06.html
    cmd.replace(QRegExp("%k"), filename());
    cmd.replace(QRegExp("%i"), QString("--icon ") + icon());
    cmd.replace(QRegExp("%c"), title());
    cmd.replace(QRegExp("%[fFuU]"), filename()); // stuff we don't handle

    qDebug("Launching %s", qPrintable(cmd));
    QProcess::startDetached(cmd);
#else
    ContentAction::Action action = ContentAction::Action::launcherAction(m_entry, QStringList());
    action.trigger();
#endif
}

QString LauncherItem::value(QString key) const {
    return m_entry->value(key);
}

bool LauncherItem::contains(QString val) const {
    return m_entry->contains(val);
}

bool LauncherItem::uninstall() {
    if (m_entry->type() == "Widget")
    {
        return QFile::remove(m_entry->fileName());
    }

    return false;
}
