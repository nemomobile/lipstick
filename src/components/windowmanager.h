
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

#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <QObject>

class WindowManager : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(WindowManager)

public:
    explicit WindowManager(QObject *parent = 0);
    Q_INVOKABLE void closeWindow(qulonglong windowId);
    Q_INVOKABLE void windowToFront(qulonglong windowId);
    Q_INVOKABLE bool isHomeWindowOnTop();

};

#endif // WINDOWMANAGER_H
