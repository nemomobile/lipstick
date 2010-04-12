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

#ifndef WEATHERMODEL_H_
#define WEATHERMODEL_H_

#include <MWidgetModel>

class WeatherModel : public MWidgetModel
{
    Q_OBJECT
    M_MODEL(WeatherModel)

    M_MODEL_PROPERTY(QList<QString>, cities, Cities, true, QList<QString>())
    M_MODEL_PROPERTY(int, unit, Unit, true, 0)
    M_MODEL_PROPERTY(int, numMonitoredCities, NumMonitoredCities, true, 2)
    M_MODEL_PROPERTY(bool, showLocal, ShowLocal, true, false)
    M_MODEL_PROPERTY(int, localTemperature, LocalTemperature, true, 0)
};

#endif /* WEATHERMODEL_H_ */
