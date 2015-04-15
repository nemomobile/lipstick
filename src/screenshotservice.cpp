/***************************************************************************
**
** Copyright (C) 2013 Jolla Ltd.
** Contact: Robin Burchell <robin.burchell@jollamobile.com>
**
** This file is part of lipstick.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/
#include <QStandardPaths>
#include <QDateTime>
#include "lipstickcompositor.h"
#include "screenshotservice.h"

#include <private/qquickwindow_p.h>
#include "hwcrenderstage.h"

ScreenshotService::ScreenshotService(QObject *parent) :
    QObject(parent)
{
}

void ScreenshotService::saveScreenshot(const QString &path)
{
    if (LipstickCompositor::instance() != 0) {
        QQuickWindowPrivate *wd = QQuickWindowPrivate::get(LipstickCompositor::instance());
        HwcRenderStage *renderStage = (HwcRenderStage *) wd->customRenderStage;
        if (renderStage)
            renderStage->setBypassHwc(true);
        LipstickCompositor::instance()->grabWindow().save(path.isEmpty() ? (QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/" + QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + ".png") : path);
        if (renderStage)
            renderStage->setBypassHwc(false);
    }
}
