
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
// Copyright (c) 2012, Timur Kristóf <venemo@fedoraproject.org>

#ifndef LAUNCHERMODEL_H
#define LAUNCHERMODEL_H

#include <QObject>
#include "launcheritem.h"
#include "utilities/qobjectlistmodel.h"

class QFileSystemWatcher;

class LauncherModel : public QObjectListModel
{
    Q_OBJECT
    Q_DISABLE_COPY(LauncherModel)

    Q_PROPERTY(QStringList directories READ directories WRITE setDirectories NOTIFY directoriesChanged)

    QFileSystemWatcher *_fileSystemWatcher;

private slots:
    void monitoredDirectoryChanged(QString);

public:
    explicit LauncherModel(QObject *parent = 0);
    virtual ~LauncherModel();

    QStringList directories() const;
    void setDirectories(QStringList);

signals:
    void directoriesChanged();

};

#endif // LAUNCHERMODEL_H
