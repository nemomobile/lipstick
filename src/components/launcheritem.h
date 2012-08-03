/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef LAUNCHERITEM_H
#define LAUNCHERITEM_H

#include <QObject>
#include <QSharedPointer>
#include <QStringList>

class MDesktopEntry;

class LauncherItem : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(LauncherItem)

    Q_PROPERTY(QString id READ id)
    Q_PROPERTY(QString type READ type NOTIFY typeChanged)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(QString comment READ comment NOTIFY commentChanged)
    Q_PROPERTY(QString icon READ icon NOTIFY iconChanged)
    Q_PROPERTY(QString exec READ exec NOTIFY execChanged)
    Q_PROPERTY(QStringList categories READ categories NOTIFY categoriesChanged)
    Q_PROPERTY(bool nodisplay READ nodisplay NOTIFY nodisplayChanged)
    Q_PROPERTY(QString filename READ filename NOTIFY filenameChanged)
    Q_PROPERTY(int pid READ pid WRITE setPid)
    Q_PROPERTY(int wid READ wid WRITE setWid)

private:
    QString m_filename;
    QSharedPointer<MDesktopEntry> m_entry;
    QString m_id;
    int m_pid;
    int m_wid;

    bool m_assigned;

public:
    LauncherItem(const QString &filename, QObject *parent = 0);
    ~LauncherItem();

    QString id() const;
    bool isValid() const;
    QString type() const;
    QString title() const;
    QString comment() const;
    QString icon() const;
    QString exec() const;
    QStringList categories() const;
    QString filename() const;
    int wid() const;
    void setWid(int wid);
    int pid();
    void setPid(int pid);
    bool nodisplay() const;

    Q_INVOKABLE void launch() const;

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

public slots:
    QString value(QString key) const;
    bool contains(QString val) const;
    bool uninstall();

signals:
    void typeChanged();
    void titleChanged();
    void commentChanged();
    void iconChanged();
    void execChanged();
    void categoriesChanged();
    void filenameChanged();
    void nodisplayChanged();
};

#endif // LAUNCHERITEM_H
