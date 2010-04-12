/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of mhome.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include <cstring>
#include "windowinfo.h"

WindowInfo::WindowInfo(QString &title, Window window, XWindowAttributes windowAttributes,
                       Pixmap icon, WindowPriority priority) :
    title_(title), windowPriority_(priority), window_(window),
    attributes_(windowAttributes), pixmap_(icon)
{
}

WindowInfo::~WindowInfo()
{
}

const QString &WindowInfo::title() const
{
    return title_;
}

WindowInfo::WindowPriority WindowInfo::windowPriority() const
{
    return windowPriority_;
}

Window WindowInfo::window() const
{
    return window_;
}

XWindowAttributes WindowInfo::windowAttributes() const
{
    return attributes_;
}

Pixmap WindowInfo::icon() const
{
    return pixmap_;
}

bool operator==(const WindowInfo &wi1, const WindowInfo &wi2)
{
    return wi1.window() == wi2.window();
}

