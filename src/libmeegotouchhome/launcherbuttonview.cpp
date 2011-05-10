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

#include "launcherbuttonview.h"

#include <QFileInfo>
#include <QDir>
#include "launcherbuttonprogressindicator.h"
#include "launcherbutton.h"

static const QString DEFAULT_APPLICATION_ICON_ID = "icon-l-default-application";

LauncherButtonView::LauncherButtonView(LauncherButton *controller) :
    MButtonIconView(controller),
    controller(controller),
    progressIndicator(new LauncherButtonProgressIndicator(controller))
{
}

LauncherButtonView::~LauncherButtonView()
{
}

void LauncherButtonView::setupModel()
{
    MButtonIconView::setupModel();

    updateButtonIcon();
    resetProgressIndicator();
}

void LauncherButtonView::updateData(const QList<const char *>& modifications)
{
    MButtonIconView::updateData(modifications);

    const char *member;
    foreach(member, modifications) {
        if (member == LauncherButtonModel::ButtonState) {
            updateButtonIcon();
            resetProgressIndicator();

            if (model()->buttonState() == LauncherButtonModel::Launching) {
                controller->setEnabled(false);
            } else {
                if (model()->buttonState()
                    == LauncherButtonModel::Installed
                    || model()->buttonState()
                    == LauncherButtonModel::Broken) {
                    controller->setEnabled(true);
                } else {
                    controller->setEnabled(false);
                }
            }
        } else if (member == LauncherButtonModel::OperationProgress) {
            if (model()->buttonState() == LauncherButtonModel::Downloading) {
                progressIndicator->setValue(model()->operationProgress());
            }
        } else if (member == LauncherButtonModel::DesktopEntry) {
            updateButtonIcon();
        }
    }
}

void LauncherButtonView::resetProgressIndicator()
{
    LauncherButtonModel::State state = model()->buttonState();

    progressIndicator->setIndicatorState(state);
    switch(state) {
        case LauncherButtonModel::Launching:
            if (style()->showLaunchProgress()) {
                progressIndicator->reset();
                progressIndicator->setUnknownDuration(true);
                progressIndicator->show();
            } else {
                progressIndicator->hide();
            }
            break;
        case LauncherButtonModel::Installing:
            progressIndicator->reset();
            progressIndicator->setUnknownDuration(true);
            progressIndicator->show();
            break;
        case LauncherButtonModel::Downloading:
            if (!progressIndicator) {
                progressIndicator = createProgressIndicator();
            }
            progressIndicator->setIndicatorState(state);
            progressIndicator->setUnknownDuration(false);
            break;
        default:
            progressIndicator->hide();
            break;
    }
}

void LauncherButtonView::updateButtonIcon()
{
    // To assure icon is updated we need to set icon and icon id to null.
    model()->setIconID(QString());
    model()->setIcon(QIcon());

    switch(model()->buttonState()) {
        case LauncherButtonModel::Downloading:
            model()->setIconID(style()->downloadingPlaceholderIcon());
            break;
        case LauncherButtonModel::Installing:
            model()->setIconID(style()->installingPlaceholderIcon());
            break;
        case LauncherButtonModel::Broken:
            model()->setIconID(style()->brokenPlaceholderIcon());
            break;
        default:
            setIconFromDesktopEntry();
            break;
    }
}

void LauncherButtonView::setIconFromDesktopEntry()
{
    if (!model()->desktopEntry().isNull()) {
        QString icon = model()->desktopEntry()->icon();
        if (!icon.isEmpty()) {
            QFileInfo fileInfo(icon);
            if (fileInfo.isAbsolute()) {
                if (fileInfo.exists()) {
                    model()->setIcon(QIcon(icon));
                } else {
                    model()->setIconID(DEFAULT_APPLICATION_ICON_ID);
                    unavailableIconPath = icon;
                    // Add non-existent icon path to watcher for to be loaded when available
                    iconWatcher.addPath(QFileInfo(unavailableIconPath).absolutePath());
                    connect(&iconWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(updateUnavailableIcon(QString)), Qt::UniqueConnection);
                }
            } else {
                if (QIcon::hasThemeIcon(icon)) {
                    model()->setIcon(QIcon::fromTheme(icon));
                } else {
                    model()->setIconID(icon);
                }
            }
        } else {
            model()->setIconID(DEFAULT_APPLICATION_ICON_ID);
        }
    }
}

void LauncherButtonView::updateUnavailableIcon(const QString& dirPath)
{
    if (!unavailableIconPath.isEmpty()) {
        QFileInfo fileInfo(unavailableIconPath);
        if (fileInfo.absoluteDir() == dirPath && fileInfo.exists()) {
            // To assure icon is updated we need to set icon and icon id to null.
            model()->setIconID(QString());
            model()->setIcon(QIcon());

            // Icon available, update
            model()->setIcon(QIcon(unavailableIconPath));

            iconWatcher.removePath(unavailableIconPath);
            unavailableIconPath = QString();
            disconnect(&iconWatcher, SIGNAL(directoryChanged(const QString&)), this, SLOT(updateUnavailableIcon(const QString&)));
        }
    }
}

M_REGISTER_VIEW_NEW(LauncherButtonView, LauncherButton)
