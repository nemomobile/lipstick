
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

#include "windowmanager.h"
#include "xtools/xwindowmanager.h"
#include "xtools/homewindowmonitor.h"

WindowManager::WindowManager(QObject *parent) :
    QObject(parent)
{
    connect(HomeWindowMonitor::instance(), SIGNAL(isHomeWindowOnTopChanged()), this, SIGNAL(isHomeWindowOnTopChanged()));
}

void WindowManager::windowToFront(qulonglong windowId)
{
    XWindowManager::windowToFront(windowId);
}

void WindowManager::closeWindow(qulonglong windowId)
{
    XWindowManager::closeWindow(windowId);
}

bool WindowManager::isHomeWindowOnTop()
{
    return HomeWindowMonitor::instance()->isHomeWindowOnTop();
}
