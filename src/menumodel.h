/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#ifndef MENUMODEL_H
#define MENUMODEL_H

#include <QObject>
#include <QtDeclarative>
#include <QAbstractItemModel>
#include <QHash>
#include <mdesktopentry.h>
#include "menuitem.h"
#include "desktop.h"

class QFileSystemWatcher;

class MenuModel : public QAbstractItemModel
{
    Q_OBJECT
	Q_PROPERTY(QStringList directories READ directories WRITE setDirectories)
	Q_PROPERTY(QString directory READ directory WRITE setDirectory)
        Q_PROPERTY(QDeclarativeListProperty<Desktop> apps READ apps NOTIFY appsChanged)
        Q_PROPERTY(QString customField READ customValue WRITE setCustomValue)
	Q_PROPERTY(QString type READ type WRITE setType)

public:
    explicit MenuModel(QObject *parent = 0);
    ~MenuModel();

    enum Role
    {
        id = Qt::UserRole + 1,
        name,
        exec,
        comment,
        icon,
        filename,
        nodisplay,
        object
    };

    QDeclarativeListProperty<Desktop> apps();

    QString directory() const {
        qDebug("Warning, 'directory' has been deprecated. Use 'directories' instead.");
        return m_watcher->directories().at(0);
    }

    QStringList directories() const {
        return m_watcher->directories();
    }

    void setDirectory(QString dir){ qDebug("Warning, directory property is deprecated. Use 'directories' instead"); setDirectories(QStringList()<<dir); }
    void setDirectories(QStringList directory);

    void setCustomValue(QString value) { m_customValue = value; }
    QString customValue() { return m_customValue; }

    void setType(QString value) { m_type = value; }
    QString type() { return m_type; }

    ///overrides from QAbstractModel:
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role) const;
    bool hasChildren ( const QModelIndex & parent = QModelIndex() ) const;

signals:
    void appsReset();
    void appsChanged();
    void recentsChanged();
    void startingApp(QString title, QString icon);

public slots:
    QString value(QString id, QString key);
    QVariant getNameByIndex(int idx);
    QVariant getExecByIndex(int idx);
    QVariant getCommentByIndex(int idx);
    QVariant getIconByIndex(int idx);
    QVariant getFileNameByIndex(int idx);

private slots:
    void appsDirChanged(QString);
    void resetApps();

private:
    QList<Desktop *> m_apps;
    QString m_customValue;
    QString m_type;
    QFileSystemWatcher *m_watcher;
    QList<MenuItem *> m_categories;
    QHash<QString, MenuItem *> m_appsHash;

    Q_DISABLE_COPY(MenuModel)
};

#endif // MENUMODEL_H
