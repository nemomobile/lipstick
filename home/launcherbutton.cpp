/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of duihome.
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

#include "launcherbutton.h"
#include "launcherbuttonview.h"

LauncherButton::LauncherButton(DuiWidget *parent) : DuiButton(parent, new LauncherButtonModel)
{
    // Connect the button clicked signal to launch application slot.
    connect(this, SIGNAL(clicked()), this, SLOT(launch()));
}

LauncherButton::~LauncherButton()
{
}

void LauncherButton::setTargetType(const QString &type)
{
    model()->setTargetType(type);
}

const QString &LauncherButton::targetType() const
{
    return model()->targetType();
}

void LauncherButton::setTarget(const QString &target)
{
    model()->setTarget(target);
}

const QString &LauncherButton::target() const
{
    return model()->target();
}

void LauncherButton::setThumbnail(const QString &thumbnail)
{
    model()->setThumbnail(thumbnail);
}

const QString &LauncherButton::thumbnail() const
{
    return model()->thumbnail();
}

void LauncherButton::launch()
{
    // Emit a signal based on the object type
    if (targetType() == "Application") {
        emit applicationLaunched(target());
    } else if (targetType() == "Link") {
        emit linkLaunched(target());
    } else if (targetType() == "Directory") {
        emit directoryLaunched(target(), text(), iconID());
    }
}
