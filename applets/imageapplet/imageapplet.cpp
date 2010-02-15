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
#include <QtGui>

#include <DuiLibrary>
#include "imageapplet.h"
#include "imagewidget.h"

DUI_LIBRARY
Q_EXPORT_PLUGIN2(imageapplet, ImageApplet)

DuiWidget *ImageApplet::constructWidget(const DuiAppletMetaData &metadata, DuiDataStore &, DuiDataAccess &)
{
    if (!metadata.contains("DUI/X-DUIApplet-ImageLandscape") || !metadata.contains("DUI/X-DUIApplet-ImagePortrait")) {
        return NULL;
    }

    ImageWidget *widget = new ImageWidget(metadata.value("DUI/X-DUIApplet-ImageLandscape"),
                                          metadata.value("DUI/X-DUIApplet-ImagePortrait"));

    return widget;
}
