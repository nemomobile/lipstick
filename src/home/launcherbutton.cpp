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
#include <QApplication>

bool LauncherButton::launching = false;

LauncherButton::LauncherButton(MWidget *parent) : MButton(parent, new LauncherButtonModel)
{
    init();
}

LauncherButton::LauncherButton(const QString &desktopEntryPath, MWidget *parent) :
        MButton(parent, new LauncherButtonModel)
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

    // When the progress indicator timer times out the progress indicator should be hidden
    progressIndicatorTimeoutTimer.setSingleShot(true);
    connect(&progressIndicatorTimeoutTimer, SIGNAL(timeout()), this, SLOT(hideProgressIndicator()));
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
    if (!launching) {
        launching = true;
        model()->setShowProgressIndicator(true);

        connect(qApp, SIGNAL(windowStackingOrderChanged(const QList<WindowInfo> &)), this, SLOT(hideProgressIndicatorIfObscured(const QList<WindowInfo> &)));

        action().trigger();

        progressIndicatorTimeoutTimer.start();
    }
}

void LauncherButton::hideProgressIndicator()
{
    launching = false;
    model()->setShowProgressIndicator(false);

    if (progressIndicatorTimeoutTimer.isActive()) {
        progressIndicatorTimeoutTimer.stop();
    }

    disconnect(qApp, SIGNAL(windowStackingOrderChanged(const QList<WindowInfo> &)), this, SLOT(hideProgressIndicatorIfObscured(const QList<WindowInfo> &)));
}

void LauncherButton::hideProgressIndicatorIfObscured(const QList<WindowInfo> &windowList)
{
    if (!windowList.isEmpty()) {
        const QList<Atom>& windowTypes = windowList.last().types();
        if (!windowTypes.contains(WindowInfo::NotificationAtom) &&
            !windowTypes.contains(WindowInfo::DesktopAtom) &&
            !windowTypes.contains(WindowInfo::DialogAtom) &&
            !windowTypes.contains(WindowInfo::MenuAtom)) {
            hideProgressIndicator();
        }
    }
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

bool LauncherButton::isInProgress() const
{
    return model()->showProgressIndicator();
}

void LauncherButton::setProgressIndicatorTimeout(int timeout)
{
    progressIndicatorTimeoutTimer.setInterval(timeout);
}
