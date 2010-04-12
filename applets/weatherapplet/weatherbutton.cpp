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

#include "weatherbutton.h"

WeatherButton::WeatherButton(const QString &title, MWidget *parent, int iconIndex, int timeout) :
    MButton(title, parent, new WeatherButtonModel)
{
    // Set the initial icon index
    model()->setIconIndex(iconIndex);

    model()->setTemperature(title.length() - 2);

    // Update the icon when the timeout occurs
    startTimer(timeout);
}

WeatherButton::~WeatherButton()
{
}

void WeatherButton::timerEvent(QTimerEvent *)
{
    model()->setIconIndex((model()->iconIndex() + 1) % 5);

    update();
}

void WeatherButton::setUnit(WeatherButtonModel::UnitType unit)
{
    model()->setUnit(unit);
}

void WeatherButton::toggleUseStyle()
{
    model()->setUseStyle(!model()->useStyle());
}
