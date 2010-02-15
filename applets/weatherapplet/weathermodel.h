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

#ifndef WEATHERMODEL_H_
#define WEATHERMODEL_H_

#include <DuiWidgetModel>

class WeatherModel : public DuiWidgetModel
{
    Q_OBJECT
    DUI_MODEL(WeatherModel)

    DUI_MODEL_PROPERTY(QList<QString>, cities, Cities, true, QList<QString>())
    DUI_MODEL_PROPERTY(int, unit, Unit, true, 0)
    DUI_MODEL_PROPERTY(int, numMonitoredCities, NumMonitoredCities, true, 2)
    DUI_MODEL_PROPERTY(bool, showLocal, ShowLocal, true, false)
    DUI_MODEL_PROPERTY(int, localTemperature, LocalTemperature, true, 0)
};

#endif /* WEATHERMODEL_H_ */
