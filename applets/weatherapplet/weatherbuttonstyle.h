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


#ifndef WEATHERBUTTONSTYLE_H_
#define WEATHERBUTTONSTYLE_H_

#include <QFont>
#include <QColor>
#include <QPointF>
#include <QSize>
#include <QSizeF>
#include <duibuttonstyle.h>

class WeatherButtonStyle : public DuiButtonStyle
{
    Q_OBJECT
    DUI_STYLE(WeatherButtonStyle)

    DUI_STYLE_ATTRIBUTE(QFont, temperatureFont, TemperatureFont)
    DUI_STYLE_ATTRIBUTE(QColor, temperatureColor, TemperatureColor)
    DUI_STYLE_ATTRIBUTE(QPointF, temperaturePosition, TemperaturePosition)
    DUI_STYLE_ATTRIBUTE(QSize, temperatureSize, TemperatureSize)
    DUI_STYLE_ATTRIBUTE(QFont, cityFont, CityFont)
    DUI_STYLE_ATTRIBUTE(QColor, cityColor, CityColor)
    DUI_STYLE_ATTRIBUTE(QPointF, cityPosition, CityPosition)
    DUI_STYLE_ATTRIBUTE(QSize, citySize, CitySize)
    DUI_STYLE_ATTRIBUTE(QPointF, iconPosition, IconPosition)
};

class WeatherButtonStyleContainer : public DuiButtonStyleContainer
{
    DUI_STYLE_CONTAINER(WeatherButtonStyle)
};

#endif /* WEATHERBUTTONSTYLE_H_ */
