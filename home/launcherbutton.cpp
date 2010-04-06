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
#include <DuiDesktopEntry>

LauncherButton::LauncherButton(DuiWidget *parent) : DuiButton(parent, new LauncherButtonModel)
{
    // Connect the button clicked signal to launch application slot.
    connect(this, SIGNAL(clicked()), this, SLOT(launch()));
}

LauncherButton::LauncherButton(const DuiDesktopEntry &entry, DuiWidget *parent) : DuiButton(parent, new LauncherButtonModel)
{
    // Connect the button clicked signal to launch application slot.
    connect(this, SIGNAL(clicked()), this, SLOT(launch()));

    setTargetType(entry.type());
    setText(entry.name());

    if (!entry.icon().isEmpty()) {
        setIconID(entry.icon());
    } else {
        // FIXME: change to use correct default icon id when available
        // as incorrect id icon-Application-Default will load default icon
        // (at the moment default icon seems to be icon-l-video)
        setIconID("icon-Application-Default");
    }

    // Set target based on type
    if (entry.type() == "Application") {
        QString thisXMaemoService = entry.xMaemoService();

        if (thisXMaemoService.isEmpty()) {
            setTarget(entry.exec());
        } else {
            setTarget(thisXMaemoService);
            setTargetType("Service");
        }
    }

    model()->setDesktopEntryFile(entry.fileName());
}

LauncherButton::~LauncherButton()
{
}

void LauncherButton::setTargetType(const QString &type)
{
    model()->setTargetType(type);
}

QString LauncherButton::targetType() const
{
    return model()->targetType();
}

void LauncherButton::setTarget(const QString &target)
{
    model()->setTarget(target);
}

QString LauncherButton::target() const
{
    return model()->target();
}

QString LauncherButton::desktopEntry() const
{
    return model()->desktopEntryFile();
}

void LauncherButton::launch()
{
    // Emit a signal based on the object type
    if (targetType() == "Application") {
        emit applicationLaunched(target());
    } else if (targetType() == "Service") {
        emit duiApplicationLaunched(target());
    }
}
