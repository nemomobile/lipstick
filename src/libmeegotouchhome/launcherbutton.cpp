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
#include "launcher.h"
#include <MDesktopEntry>
#include <QDBusInterface>

#include <MWidgetCreator>
M_REGISTER_WIDGET(LauncherButton)

#include "homewindowmonitor.h"

LauncherButton::LauncherButton(const QString &desktopEntryPath, MWidget *parent, LauncherButtonModel *model) :
        MButton(parent, model)
{
    QSharedPointer<MDesktopEntry> entry(new MDesktopEntry(desktopEntryPath));

    init();

    updateFromDesktopEntry(entry);
}

LauncherButton::LauncherButton(const QSharedPointer<MDesktopEntry> &entry, MWidget *parent, LauncherButtonModel *model) :
        MButton(parent, model)
{
    init();

    updateFromDesktopEntry(entry);
}

LauncherButton::~LauncherButton()
{
}

void LauncherButton::init()
{
    // When the button is clicked the related object should be launched
    connect(this, SIGNAL(clicked()), this, SLOT(launch()), Qt::UniqueConnection);
}

QString LauncherButton::desktopEntry() const
{
    return model()->desktopEntry()->fileName();
}

LauncherButtonModel::State LauncherButton::buttonState() const
{
    return model()->buttonState();
}

void LauncherButton::launch()
{
    if (model()->buttonState() == LauncherButtonModel::Installed) {
        model()->setButtonState(LauncherButtonModel::Launching);
        connect(HomeWindowMonitor::instance(), SIGNAL(fullscreenWindowOnTopOfOwnWindow()), SLOT(stopLaunchProgress()));

        action.trigger();
    } else if (model()->buttonState() == LauncherButtonModel::Broken) {
        // Show the exception dialog for this package
        if (!model()->desktopEntry().isNull()) {
            QString package = model()->packageName();
            if (!package.isEmpty()) {
                QDBusInterface interface("com.nokia.package_manager_install_ui",
                        "/com/nokia/package_manager_install_ui",
                        "com.nokia.package_manager_install_ui",
                        QDBusConnection::sessionBus());
                interface.call("show_installation_exception", package);
            }
        }
    }
}

void LauncherButton::stopLaunchProgress()
{
    model()->setButtonState(LauncherButtonModel::Installed);
    disconnect(HomeWindowMonitor::instance(), SIGNAL(fullscreenWindowOnTopOfOwnWindow()), this, SLOT(stopLaunchProgress()));
}

void LauncherButton::retranslateUi()
{
    if (!model()->desktopEntry().isNull()) {
        setText(model()->desktopEntry()->name());
    }
    MButton::retranslateUi();
}

void LauncherButton::updateFromDesktopEntry(const QSharedPointer<MDesktopEntry> &entry)
{
    setText(entry->name());
    action = LauncherAction(entry);
    model()->setDesktopEntry(entry);
}

void LauncherButton::setState(LauncherButtonModel::State state)
{
    model()->setButtonState(state);
}

void LauncherButton::setOperationProgress(int newProgress, int total)
{
    if (newProgress >= 0 && newProgress <= total) {
        int normalizedProgress = (newProgress*100)/total;
        model()->setOperationProgress(normalizedProgress);
    }
}

int LauncherButton::operationProgress() const
{
    return model()->operationProgress();
}

void LauncherButton::setPackageName(const QString &packageName)
{
    model()->setPackageName(packageName);
}

QString LauncherButton::packageName() const
{
    return model()->packageName();
}

void LauncherButton::setPackageRemovable(const bool removable)
{
    model()->setPackageRemovable(removable);
}

bool LauncherButton::packageRemovable() const
{
    return model()->packageRemovable();
}
