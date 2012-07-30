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
#include <mdesktopentry.h>
#include "qticonloader.h"

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
    return QtIconLoader::icon(m_entry->icon());
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
