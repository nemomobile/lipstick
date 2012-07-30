/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <QProcess>
#include <QtDeclarative/qdeclarative.h>
#include <QCryptographicHash>
#include <QFile>

#include <QDebug>
#include <QDir>
#include <QApplication>
#include <QIcon>
#include <QFile>
#include <QSettings>

#include <mdesktopentry.h>

#ifdef HAS_CONTENTACTION
#include "launcheraction.h"
#endif

#include "desktop.h"

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

Desktop::Desktop(const QString &fileName, QObject *parent)
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

Desktop::~Desktop()
{
}

QString Desktop::id() const {
    return m_id;
}

bool Desktop::isValid() const {
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

QString Desktop::type() const {
    return m_entry->type();
}

QString Desktop::title() const {
    return m_entry->name();
}

QString Desktop::comment() const {
    return m_entry->comment();
}

QString Desktop::icon() const {
    return getIconPath(m_entry->icon());
}

QString Desktop::exec() const {
    return m_entry->exec();
}

QStringList Desktop::categories() const {
    return m_entry->categories();
}

QString Desktop::filename() const {
    return Desktop::m_filename;
}

int Desktop::wid() const
{
    return m_wid;
}

void Desktop::setWid(int wid) {
    m_wid = wid;
}

int Desktop::pid()
{
    return m_pid;
}

void Desktop::setPid(int pid)
{
    m_pid = pid;
}

bool Desktop::nodisplay() const {
    return m_entry->noDisplay();
}

void Desktop::launch() const
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
    LauncherAction action(m_entry);
    action.trigger();
#endif
}

QString Desktop::value(QString key) const {
    return m_entry->value(key);
}

bool Desktop::contains(QString val) const {
    return m_entry->contains(val);
}

bool Desktop::uninstall() {
    if (m_entry->type() == "Widget")
    {
        return QFile::remove(m_entry->fileName());
    }

    return false;
}
