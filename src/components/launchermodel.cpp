/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <QDir>
#include <QFileSystemWatcher>
#include <QDebug>
#include "launchermodel.h"

LauncherModel::LauncherModel(QObject *parent) :
    QAbstractItemModel(parent),
    m_type("Application")
{
    m_watcher = new QFileSystemWatcher(this);
    connect(m_watcher, SIGNAL(directoryChanged(QString)), this, SLOT(appsDirChanged(QString)));

    // Default dirs
    m_watcher->addPath("/usr/share/applications");

    QHash<int, QByteArray> roles;
    roles[id]="id";
    roles[name]="name";
    roles[exec]="exec";
    roles[comment]="comment";
    roles[icon]="icon";
    roles[filename]="filename";
    roles[nodisplay]="nodisplay";
    roles[object]="object";

    setRoleNames(roles);

    appsDirChanged("/usr/share/applications");
}

void LauncherModel::appsDirChanged(QString changedDir)
{
    QList<LauncherItem *> added;
    QList<LauncherItem *> removed;
    QDir dir (changedDir);
    dir.setFilter(QDir::Files | QDir::NoSymLinks);

    foreach (QFileInfo info, dir.entryInfoList())
    {
        bool found = false;
        foreach (LauncherItem *d, m_apps)
        {
            if (info.absoluteFilePath().endsWith(d->filename().right(d->filename().lastIndexOf('/'))))
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            qDebug() << "Created desktop file " << info.absoluteFilePath();
            LauncherItem *desktopEntry = new LauncherItem(info.absoluteFilePath());
            if (!desktopEntry->isValid() || (desktopEntry->type() != m_type))
            {
                delete desktopEntry;
            }
            else
            {
                added << desktopEntry;
            }
        }
    }

    foreach (LauncherItem *d, m_apps)
    {
        if (!d->filename().contains(changedDir))
            continue;

        bool found = false;
        foreach (QFileInfo info, dir.entryInfoList())
        {
            if (d->filename() == info.absoluteFilePath())
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            removed << d;
        }
    }

    if (removed.length() > 0)
    {
        QList<LauncherItem *> tmp;
        while (!m_apps.isEmpty())
        {
            LauncherItem *d = m_apps.takeLast();
            if (removed.contains(d))
            {
                delete d;
            }
            else
            {
                tmp << d;
            }
        }

        m_apps = tmp;
    }

    if (added.length() > 0)
    {
        foreach (LauncherItem *d, added)
        {
            m_apps << d;
        }
    }

    resetApps();
    emit appsChanged();
}

void LauncherModel::setDirectories(QStringList directories)
{
    m_watcher->removePaths(m_watcher->directories());

    foreach(QString directory, directories)
    {
        QString path;
        if (directory.startsWith("~"))
            path = QDir::homePath() + directory.remove(0, 1);
        else if (directory == ".")
            path = QDir::currentPath();
        else
            path = directory;

        // If directory doesn't exist, then attempt to create it
        if (!QDir(path).exists())
        {
            QDir().mkpath(path);
        }

        m_watcher->addPath(path);
    }

    resetApps();
}

void LauncherModel::resetApps()
{
    beginResetModel();

    while (!m_apps.isEmpty())
        delete m_apps.takeFirst();

    QStringList addedDirectories;

    foreach (QString target, m_watcher->directories())
    {
        QDir dir(target);
        dir.setFilter(QDir::Files | QDir::NoSymLinks);

        QStringList filters;
        filters << "*.desktop";
        foreach (QFileInfo fileInfo, dir.entryInfoList(filters))
        {

            if(!fileInfo.fileName().endsWith(".desktop"))
                continue;

            if(addedDirectories.contains(fileInfo.fileName()))
                continue;

            addedDirectories<<fileInfo.fileName();

            LauncherItem *desktopEntry = new LauncherItem(fileInfo.absoluteFilePath());
            if (!desktopEntry->isValid() ||
                desktopEntry->type() != m_type ||
                desktopEntry->nodisplay())
            {
                delete desktopEntry;
                continue;
            }

            m_apps << desktopEntry;
        }
    }

    endResetModel();
    emit appsReset();
}

QModelIndex LauncherModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    return createIndex(row, column);
}

QModelIndex LauncherModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}

int LauncherModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_apps.count();
}

int LauncherModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant LauncherModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    LauncherItem *i = m_apps.at(index.row());

    switch (role) {
        case name:
            return i->title();
        case exec:
            return i->exec();
        case icon:
            return "file:///" + i->icon();
        case comment:
            return i->comment();
        case filename:
            return i->filename();
        case nodisplay:
            return i->nodisplay();
        case object:
            return QVariant::fromValue<QObject *>(i);
        default:
            break;
    }

    return QVariant();
}

bool LauncherModel::hasChildren ( const QModelIndex & parent ) const
{
    Q_UNUSED(parent);
    return false;
}

QDeclarativeListProperty<LauncherItem> LauncherModel::apps()
{
    return QDeclarativeListProperty<LauncherItem>(this, m_apps);
}

QStringList LauncherModel::directories() const {
    return m_watcher->directories();
}

QString LauncherModel::value(QString id, QString key)
{
    foreach(LauncherItem* item, m_apps)
	{
		if(item->id() == id)
			return item->value(key);
	}
	return "";
}

QVariant LauncherModel::getNameByIndex(int idx)
{
    return data(index(idx, 0, QModelIndex()), name);
}

QVariant LauncherModel::getExecByIndex(int idx)
{
    return data(index(idx, 0, QModelIndex()), exec);
}

QVariant LauncherModel::getCommentByIndex(int idx)
{
    return data(index(idx, 0, QModelIndex()), comment);
}

QVariant LauncherModel::getIconByIndex(int idx)
{
    return data(index(idx, 0, QModelIndex()), icon);
}

QVariant LauncherModel::getFileNameByIndex(int idx)
{
    return data(index(idx, 0, QModelIndex()), filename);
}

LauncherModel::~LauncherModel()
{
    qDeleteAll(m_apps);
}
