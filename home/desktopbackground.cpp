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

#include <MSceneManager>
#include "mainwindow.h"
#include "desktopbackground.h"
#include "desktopstyle.h"
static const QString PORTRAIT_GCONF_KEY = "/desktop/meego/background/portrait";
static const QString LANDSCAPE_GCONF_KEY = "/desktop/meego/background/landscape";
static const QString PICTURE_KEY_NAME = "/picture_filename";

DesktopBackground::DesktopBackground(QObject* parent) :
    QObject(parent),
    portraitBackgroundImageConfig_(PORTRAIT_GCONF_KEY + PICTURE_KEY_NAME),
    landscapeBackgroundImageConfig_(LANDSCAPE_GCONF_KEY + PICTURE_KEY_NAME)
{
    loadPortraitPixmap();
    loadLandscapePixmap();
    connect(&portraitBackgroundImageConfig_, SIGNAL(valueChanged()),
            this, SLOT(reloadPortraitImage()));
    connect(&landscapeBackgroundImageConfig_, SIGNAL(valueChanged()),
            this, SLOT(reloadLandscapeImage()));
}

DesktopBackground::~DesktopBackground()
{
}

QPixmap DesktopBackground::backgroundImage(const DesktopStyleContainer& style)
{
    QPixmap returnValue;
    MSceneManager *manager = MainWindow::instance(true)->sceneManager();
    if (manager->orientation() == M::Portrait) {
        returnValue = portraitBackgroundImage_;
    } else {
        returnValue = landscapeBackgroundImage_;
    }
    if (returnValue.isNull()) {
        const QPixmap* tmpValue = style->desktopBackgroundImage();
        if (tmpValue != NULL) {
            returnValue = *tmpValue;
        }
    }
    return returnValue;
}

void DesktopBackground::reloadPortraitImage()
{
    if (!loadPortraitPixmap()) {
        // image wasn't changed
        return;
    }
    MSceneManager *manager = MainWindow::instance(true)->sceneManager();
    if (manager->orientation() == M::Portrait) {
        emit backgroundImageChanged();
    }
}

void DesktopBackground::reloadLandscapeImage()
{
    if (!loadLandscapePixmap()) {
        // image wasn't changed
        return;
    }
    MSceneManager *manager = MainWindow::instance(true)->sceneManager();
    if (manager->orientation() != M::Portrait) {
        emit backgroundImageChanged();
    }
}

static bool loadPixmap(MGConfItem& config, QPixmap& image)
{
    QString confValue = config.value().toString();
    bool loadResult = image.load(confValue);
    if (!loadResult) {
        if (confValue.isEmpty()) {
            // image was changed
            image = QPixmap();
            return true;
        }
    }
    // if confValue isn't empty, loadResult tells whether the image changed
    return loadResult;
}

static bool verifyImageSize(M::Orientation orientation, QPixmap& image)
{
    if (image.isNull()) {
        return true;
    }
    MSceneManager *manager = MainWindow::instance(true)->sceneManager();
    QSize sceneSize = manager->visibleSceneSize(orientation);
    if (image.size() != sceneSize) {
        image = QPixmap();
        return false;
    }
    return true;
}

bool DesktopBackground::loadPortraitPixmap()
{
    bool returnValue =
        loadPixmap(portraitBackgroundImageConfig_,
                   portraitBackgroundImage_)
        && verifyImageSize(M::Portrait, portraitBackgroundImage_);
    if (returnValue) {
        lastKnownGoodPortraitBackgroundImagePath_ =
            portraitBackgroundImageConfig_.value().toString();
    } else {
        portraitBackgroundImageConfig_.set(
            QVariant(lastKnownGoodPortraitBackgroundImagePath_));
    }
    return returnValue;
}

bool DesktopBackground::loadLandscapePixmap()
{
    bool returnValue =
        loadPixmap(landscapeBackgroundImageConfig_,
                   landscapeBackgroundImage_)
        && verifyImageSize(M::Landscape, landscapeBackgroundImage_);
    if (returnValue) {
        lastKnownGoodLandscapeBackgroundImagePath_ =
            landscapeBackgroundImageConfig_.value().toString();
    } else {
        landscapeBackgroundImageConfig_.set(
            QVariant(lastKnownGoodLandscapeBackgroundImagePath_));
    }
    return returnValue;
}
