/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at 
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <QtDeclarative/qdeclarative.h>
#include <QDir>
#include <QFileInfoList>
#include <QFileSystemWatcher>
#include <QRegExp>
#include <mdesktopentry.h>
#include "menumodel.h"
#include "desktop.h"

MenuModel::MenuModel(QObject *parent) :
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

void MenuModel::appsDirChanged(QString changedDir)
{
    QList<Desktop *> added;
    QList<Desktop *> removed;
    QDir dir (changedDir);
    dir.setFilter(QDir::Files | QDir::NoSymLinks);

    foreach (QFileInfo info, dir.entryInfoList())
    {
        bool found = false;
        foreach (Desktop *d, m_apps)
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
            Desktop *desktopEntry = new Desktop(info.absoluteFilePath());
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

    foreach (Desktop *d, m_apps)
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
        QList<Desktop *> tmp;
        while (!m_apps.isEmpty())
        {
            Desktop *d = m_apps.takeLast();
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
        foreach (Desktop *d, added)
        {
            m_apps << d;
        }
    }

    resetApps();
    emit appsChanged();
}

void MenuModel::setDirectories(QStringList directories)
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

void MenuModel::resetApps()
{
    beginResetModel();

    while (!m_apps.isEmpty())
        delete m_apps.takeFirst();

    m_categories.clear();
    m_appsHash.clear();

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

            Desktop *desktopEntry = new Desktop(fileInfo.absoluteFilePath());
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

QModelIndex MenuModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    return createIndex(row, column);
}

QModelIndex MenuModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}

int MenuModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_apps.count();
}

int MenuModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant MenuModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Desktop *i = m_apps.at(index.row());

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

bool MenuModel::hasChildren ( const QModelIndex & parent ) const
{
    Q_UNUSED(parent);
    return false;
}

QDeclarativeListProperty<Desktop> MenuModel::apps()
{
    return QDeclarativeListProperty<Desktop>(this, m_apps);
}

QString MenuModel::value(QString id, QString key)
{
	foreach(Desktop* item, m_apps)
	{
		if(item->id() == id)
			return item->value(key);
	}
	return "";
}

QVariant MenuModel::getNameByIndex(int idx)
{
    return data(index(idx, 0, QModelIndex()), name);
}

QVariant MenuModel::getExecByIndex(int idx)
{
    return data(index(idx, 0, QModelIndex()), exec);
}

QVariant MenuModel::getCommentByIndex(int idx)
{
    return data(index(idx, 0, QModelIndex()), comment);
}

QVariant MenuModel::getIconByIndex(int idx)
{
    return data(index(idx, 0, QModelIndex()), icon);
}

QVariant MenuModel::getFileNameByIndex(int idx)
{
    return data(index(idx, 0, QModelIndex()), filename);
}

MenuModel::~MenuModel()
{
    qDeleteAll(m_apps);
}

QML_DECLARE_TYPE(MenuModel);
