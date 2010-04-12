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
#include <QtGui>

#include <MLibrary>
#include "imageapplet.h"
#include "imagewidget.h"

M_LIBRARY
Q_EXPORT_PLUGIN2(imageapplet, ImageApplet)

MWidget *ImageApplet::constructWidget(const MAppletMetaData &metadata, MDataStore &, MDataAccess &)
{
    if (!metadata.contains("M/X-MApplet-ImageLandscape") || !metadata.contains("M/X-MApplet-ImagePortrait")) {
        return NULL;
    }

    ImageWidget *widget = new ImageWidget(metadata.value("M/X-MApplet-ImageLandscape"),
                                          metadata.value("M/X-MApplet-ImagePortrait"));

    return widget;
}
