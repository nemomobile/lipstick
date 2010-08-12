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

#include "launcherbuttonprogressindicatorview.h"

LauncherButtonProgressIndicatorView::LauncherButtonProgressIndicatorView(LauncherButtonProgressIndicator *controller)
        : MSpinnerView(controller)
{
}

void LauncherButtonProgressIndicatorView::setupModel()
{
    MSpinnerView::setupModel();

    // Set the default style mode
    updateStyleMode();
}

void LauncherButtonProgressIndicatorView::updateData(const QList<const char *>& modifications)
{
    MSpinnerView::updateData(modifications);

    const char *member;
    foreach(member, modifications) {
        if (member == LauncherButtonProgressIndicatorModel::IndicatorState) {
            updateStyleMode();
        }
    }
}

void LauncherButtonProgressIndicatorView::updateStyleMode()
{
    switch(model()->indicatorState()) {
        case LauncherButtonModel::Installing:
            style().setModeInstalling();
        break;
        case LauncherButtonModel::Downloading:
            style().setModeDownloading();
        break;
        case LauncherButtonModel::Launching:
            style().setModeLaunching();
        break;
        default:
        break;
    }
    applyStyle();
}

// bind controller widget and view widget together by registration macro
M_REGISTER_VIEW_NEW(LauncherButtonProgressIndicatorView, LauncherButtonProgressIndicator)
