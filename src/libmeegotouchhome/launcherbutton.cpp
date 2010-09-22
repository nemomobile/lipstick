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

#include <MWidgetCreator>
M_REGISTER_WIDGET(LauncherButton)

bool LauncherButton::launching = false;

LauncherButton::LauncherButton(const QString &desktopEntryPath, MWidget *parent) :
        MButton(parent, new LauncherButtonModel),
        windowMonitor(new HomeWindowMonitor)
{
    init();

    updateFromDesktopEntry(desktopEntryPath);
}

LauncherButton::~LauncherButton()
{
}

void LauncherButton::init()
{
    // When the button is clicked the related object should be launched
    connect(this, SIGNAL(clicked()), this, SLOT(launch()));
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

LauncherButtonModel::State LauncherButton::buttonState() const
{
    return model()->buttonState();
}

void LauncherButton::launch()
{
    if (!launching) {
        if (model()->buttonState() == LauncherButtonModel::Installed) {
            model()->setButtonState(LauncherButtonModel::Launching);

            connect(windowMonitor.data(), SIGNAL(fullscreenWindowOnTopOfOwnWindow()), SLOT(stopLaunchProgress()));

            launching = true;
            action().trigger();
        } else if (model()->buttonState() == LauncherButtonModel::Broken) {
            action().trigger();
        }
    }
}

void LauncherButton::stopLaunchProgress()
{
    model()->setButtonState(LauncherButtonModel::Installed);

    launching = false;

    disconnect(windowMonitor.data(), SIGNAL(fullscreenWindowOnTopOfOwnWindow()), this, SLOT(stopLaunchProgress()));
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
    if (!desktopEntryPath.isEmpty()) {
        LauncherAction action(desktopEntryPath);

        setText(action.localizedName());

        model()->setDesktopEntryFile(desktopEntryPath);
        setAction(action);
    }
}

void LauncherButton::setState(LauncherButtonModel::State state, int progress, const QString &desktopEntryPath)
{
    model()->setButtonState(state);
    if (progress >= 0 && progress <= 100) {
        model()->setOperationProgress(progress);
    }

    // Override the current desktop entry when changing to broken or installed state
    if (state == LauncherButtonModel::Broken || state == LauncherButtonModel::Installed) {
        updateFromDesktopEntry(desktopEntryPath);
    }
}

int LauncherButton::operationProgress() const
{
    return model()->operationProgress();
}
