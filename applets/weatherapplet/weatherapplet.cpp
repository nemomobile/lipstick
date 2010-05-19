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

#include <MAppletMetaData>
#include <MLibrary>
#include "weatherapplet.h"
#include "weather.h"
#include "weatherview.h"

M_LIBRARY
Q_EXPORT_PLUGIN2(weatherapplet, WeatherApplet)

QGraphicsWidget *WeatherApplet::constructWidget(const MAppletMetaData &metaData, MDataStore &instanceData, MDataAccess &settings)
{
    if (metaData.contains("X-Additional", "StartupDelay")) {
        QString delayStr = metaData.value("X-Additional", "StartupDelay");
        bool result;
        uint delay = delayStr.toUInt(&result);
        if (result) {
            sleep(delay);
        }
    }

    return new Weather(metaData, instanceData, settings);
}
