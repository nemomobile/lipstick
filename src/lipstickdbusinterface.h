
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
// Copyright (c) 2012, Robin Burchell <robin+nemo@viroteck.net>

#ifndef DBUSINTERFACE_H
#define DBUSINTERFACE_H

#include <QDBusAbstractAdaptor>

class LipstickDBusInterface : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.nemomobile.lipstick")

public:
    LipstickDBusInterface(QObject *parent);

public slots:
    Q_NOREPLY void showLockScreen();
    Q_NOREPLY void hideLockScreen();

private slots:
    void lockscreenVisibilityChanged();

signals:
    void screenUnlocked();
};

#endif // DBUSINTERFACE_H
