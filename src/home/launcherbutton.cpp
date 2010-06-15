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

#include "launcherbutton.h"
#include "launcheraction.h"
#include "launcher.h"
#include <MDesktopEntry>
#include <QDateTime>
#include <QFileInfo>

LauncherButton::LauncherButton(MWidget *parent) : MButton(parent, new LauncherButtonModel)
{
    // Connect the button clicked signal to launch application slot.
    connect(this, SIGNAL(clicked()), this, SLOT(launch()));
}

LauncherButton::LauncherButton(const QString &desktopEntryPath, MWidget *parent) :
        MButton(parent, new LauncherButtonModel)
{
    // Connect the button clicked signal to launch application slot.
    connect(this, SIGNAL(clicked()), this, SLOT(launch()));

    updateFromDesktopEntry(desktopEntryPath);
}

LauncherButton::~LauncherButton()
{
}

void LauncherButton::setAction(const LauncherAction &action)
{
    model()->setAction(action);
}

LauncherAction LauncherButton::action() const
{
    return model()->action();
}

QString LauncherButton::desktopEntry() const
{
    return model()->desktopEntryFile();
}

void LauncherButton::launch()
{
    action().trigger();
}

void LauncherButton::retranslateUi()
{
    if (!desktopEntry().isNull()) {
        setText(action().localizedName());
    }
    MButton::retranslateUi();
}

void LauncherButton::updateFromDesktopEntry(const QString &desktopEntryPath)
{
    // only update if not initialized yet or if from the same desktop entry
    if (desktopEntry().isNull()
        || desktopEntryPath == desktopEntry()) {

        LauncherAction action(desktopEntryPath);

        setText(action.localizedName());
        updateIcon(action);

        model()->setDesktopEntryFile(desktopEntryPath);
        setAction(action);
    }
}

void LauncherButton::updateIcon(const LauncherAction& action)
{
    QString icon = action.icon();

    if (!icon.isEmpty()) {
        if (QFileInfo(icon).isAbsolute()) {
            setIcon(QIcon(icon));
        } else {
            if (QIcon::hasThemeIcon(icon)) {
                setIcon(QIcon::fromTheme(icon));
            } else {
                setIconID(icon);
            }
        }
    } else {
        // FIXME: change to use correct default icon id when available
        // as incorrect id icon-Application-Default will load default icon
        // (at the moment default icon seems to be icon-l-video)
        setIconID("icon-Application-Default");
    }
}
