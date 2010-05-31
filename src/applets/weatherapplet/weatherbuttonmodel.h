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

#ifndef WEATHERBUTTONMODEL_H_
#define WEATHERBUTTONMODEL_H_

#include <mbuttonmodel.h>

class WeatherButtonModel : public MButtonModel
{
    Q_OBJECT
    M_MODEL(WeatherButtonModel)

public:
    enum UnitType {
        Celsius,
        Fahrenheit
    };

private:
    M_MODEL_PROPERTY(WeatherButtonModel::UnitType, unit, Unit, true, Celsius)
    M_MODEL_PROPERTY(int, iconIndex, IconIndex, true, 0)
    //! A property to tell if the button should use the generic style or a hardcoded one
    M_MODEL_PROPERTY(bool, useStyle, UseStyle, true, true)
    M_MODEL_PROPERTY(int, temperature, Temperature, true, 0)
};

#endif /* WEATHERBUTTONMODEL_H_ */
