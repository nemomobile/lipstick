/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef DESKTOP_H
#define DESKTOP_H

#include <QDebug>
#include <QFile>
#include <QObject>
#include <QIcon>
#include <mdesktopentry.h>
#include <QProcess>
#include "qticonloader.h"

#ifdef HAS_CONTENTACTION
#include "launcheraction.h"
#endif

class Desktop : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id);
    Q_PROPERTY(QString type READ type NOTIFY typeChanged);
    Q_PROPERTY(QString title READ title NOTIFY titleChanged);
    Q_PROPERTY(QString comment READ comment NOTIFY commentChanged);
    Q_PROPERTY(QString icon READ icon NOTIFY iconChanged);
    Q_PROPERTY(QString exec READ exec NOTIFY execChanged);
    Q_PROPERTY(QStringList categories READ categories NOTIFY categoriesChanged);
    Q_PROPERTY(bool nodisplay READ nodisplay NOTIFY nodisplayChanged);
    Q_PROPERTY(QString filename READ filename NOTIFY filenameChanged);
    Q_PROPERTY(int pid READ pid WRITE setPid);
    Q_PROPERTY(int wid READ wid WRITE setWid);

public:
    Desktop(const QString &filename, QObject *parent = 0);
    ~Desktop();

    QString id() const {
        return m_id;
    }

    bool isValid() const {
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

    QString type() const {
        return m_entry->type();
    }

    QString title() const {
        return m_entry->name();
    }

    QString comment() const {
        return m_entry->comment();
    }

    QString icon() const {
        return QtIconLoader::icon(m_entry->icon());
    }

    QString exec() const {
        return m_entry->exec();
    }

    QStringList categories() const {
        return m_entry->categories();
    }

    QString filename() const {
        return m_filename;
    } 

    int wid() const
    {
        return m_wid;
    }

    void setWid(int wid) {
        m_wid = wid;
    }

    int pid()
    {
        return m_pid;
    }

    void setPid(int pid)
    {
        m_pid = pid;
    }

    bool nodisplay() const {
        return m_entry->noDisplay();
    } 

    enum Role {
        Type = Qt::UserRole + 1,
        Title = Qt::UserRole + 2,
        Comment = Qt::UserRole + 3,
        Icon = Qt::UserRole + 4,
        Exec = Qt::UserRole + 5,
        Categories = Qt::UserRole + 6,
        Filename = Qt::UserRole + 7,
        NoDisplay = Qt::UserRole + 8,
        Pid = Qt::UserRole + 9,
        Wid = Qt::UserRole + 10
    };

    Q_INVOKABLE void launch() const
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

public slots:

    QString value(QString key) const {
        return m_entry->value(key);
    }

    bool contains(QString val) const {
        return m_entry->contains(val);
    }

    bool uninstall() {
        if (m_entry->type() == "Widget")
        {
            return QFile::remove(m_entry->fileName());
        }

        return false;
    }

signals:
    void typeChanged();
    void titleChanged();
    void commentChanged();
    void iconChanged();
    void execChanged();
    void categoriesChanged();
    void filenameChanged();
    void nodisplayChanged();

private:
    QString m_filename;
    MDesktopEntry *m_entry;
    QString m_id;
    int m_pid;
    int m_wid;

    bool m_assigned;
    Q_DISABLE_COPY(Desktop)
};

#endif // DESKTOP_H
