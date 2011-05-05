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
#include "launcherbuttonprogressindicator.h"
#include "launcherbutton.h"

static const QString DEFAULT_APPLICATION_ICON_ID = "icon-l-default-application";

LauncherButtonView::LauncherButtonView(LauncherButton *controller) :
    MButtonIconView(controller),
    controller(controller),
    progressIndicator(new LauncherButtonProgressIndicator(controller))
{
    progressIndicator->setContentsMargins(0,0,0,0);
    progressIndicator->setRange(0, 100);
    progressIndicator->hide();
}

LauncherButtonView::~LauncherButtonView()
{
}

void LauncherButtonView::applyStyle()
{
    MButtonIconView::applyStyle();

    // Set position and size for progress indicator
    int hMargin = style()->paddingLeft() + style()->paddingRight() + style()->marginLeft() + style()->marginRight();
    int vMargin = style()->paddingTop() + style()->paddingBottom() + style()->marginTop() + style()->marginBottom();

    int progressIndicatorHOffset = (style()->preferredSize().width() / 2) - (style()->progressIndicatorIconSize().width() / 2);
    int progressIndicatorVOffset = (style()->iconSize().height() - style()->progressIndicatorIconSize().height())/2;
    int progressIndicatorLeftPosition =  progressIndicatorHOffset + hMargin / 2;
    int progressIndicatorTopPosition = progressIndicatorVOffset + vMargin / 2;

    progressIndicator->setPreferredSize(style()->progressIndicatorIconSize());
    progressIndicator->setPos(QPointF(progressIndicatorLeftPosition, progressIndicatorTopPosition));
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
            progressIndicator->reset();
            progressIndicator->show();
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
                // Only set the icon if it exists
                if (fileInfo.exists()) {
                    model()->setIcon(QIcon(icon));
                } else {
                    model()->setIconID(DEFAULT_APPLICATION_ICON_ID);
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

M_REGISTER_VIEW_NEW(LauncherButtonView, LauncherButton)
