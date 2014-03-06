// This file is part of lipstick, a QML desktop library
//
// Copyright (c) 2014 Jolla Ltd.
// Contact: Thomas Perl <thomas.perl@jolla.com>
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

#ifndef LAUNCHERDBUS_H
#define LAUNCHERDBUS_H

#include <QObject>

#include <QString>

class LauncherModel;

class LauncherDBus : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(LauncherDBus)
    Q_CLASSINFO("D-Bus Interface", "org.nemomobile.lipstick.LauncherModel")

public:
    LauncherDBus(LauncherModel *model);
    ~LauncherDBus();

    void requestLaunch(QString packageName);

public slots:
    void installStarted(QString packageName, QString label, QString iconPath, QString desktopFile);
    void installProgress(QString packageName, int progress);
    void installFinished(QString packageName);

signals:
    void showInstallProgress(QString packageName);

private:
    LauncherModel *m_model;
};

#endif // LAUNCHERDBUS_H
