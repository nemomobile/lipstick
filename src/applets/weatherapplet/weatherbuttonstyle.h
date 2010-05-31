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


#ifndef WEATHERBUTTONSTYLE_H_
#define WEATHERBUTTONSTYLE_H_

#include <QFont>
#include <QColor>
#include <QPointF>
#include <QSize>
#include <QSizeF>
#include <mbuttonstyle.h>

class WeatherButtonStyle : public MButtonStyle
{
    Q_OBJECT
    M_STYLE(WeatherButtonStyle)

    M_STYLE_ATTRIBUTE(QFont, temperatureFont, TemperatureFont)
    M_STYLE_ATTRIBUTE(QColor, temperatureColor, TemperatureColor)
    M_STYLE_ATTRIBUTE(QPointF, temperaturePosition, TemperaturePosition)
    M_STYLE_ATTRIBUTE(QSize, temperatureSize, TemperatureSize)
    M_STYLE_ATTRIBUTE(QFont, cityFont, CityFont)
    M_STYLE_ATTRIBUTE(QColor, cityColor, CityColor)
    M_STYLE_ATTRIBUTE(QPointF, cityPosition, CityPosition)
    M_STYLE_ATTRIBUTE(QSize, citySize, CitySize)
    M_STYLE_ATTRIBUTE(QPointF, iconPosition, IconPosition)
};

class WeatherButtonStyleContainer : public MButtonStyleContainer
{
    M_STYLE_CONTAINER(WeatherButtonStyle)
};

#endif /* WEATHERBUTTONSTYLE_H_ */
