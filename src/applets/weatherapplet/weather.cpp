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

#include "weather.h"
#include <MDataAccess>
#include <MDataStore>
#include <MAppletMetaData>
#include <QDebug>


Weather::Weather(const MAppletMetaData &metaData, MDataStore &instanceData, MDataAccess &settings) :
    MWidgetController(new WeatherModel, NULL),
    instanceData(instanceData),
    appletTitle(qtTrId("xx_weathertoday"))
{
    QList<QString> cities = QList<QString>() <<
                            qtTrId("xx_local_weather_button") <<
                            "New York" <<
                            "Paris" <<
                            "Madrid" <<
                            "Moscow" <<
                            "Tokyo" <<
                            "Helsinki" <<
                            "Beijing" <<
                            "London" <<
                            "Sydney" <<
                            "Los Angeles";

    // Get names of 5 cities from settings
    for (int i = 1; i <= 5; ++i) {
        QVariant city = settings.value(QString("city").append(QString::number(i)));
        if (city.type() != QVariant::Invalid) {
            cities[i - 1] = city.toString();
        }
    }

    model()->setCities(cities);

    settingChanged("unit", settings.value("unit"));
    settingChanged("togglelocal", settings.value("togglelocal"));
    settingChanged("localtemperature", settings.value("localtemperature"));
    connect(&settings, SIGNAL(valueChanged(const QString &, const QVariant &)), this, SLOT(settingChanged(const QString &, const QVariant &)));

    // Get the number of monitored cities. The value defined in metadata overrides the value in instanceData
    bool ok = false;
    uint initialMonitoredCities = 0;
    if (metaData.contains("X-Additional", "NumMonitoredCities")) {
        initialMonitoredCities = metaData.value("X-Additional", "NumMonitoredCities").toUInt(&ok);
    } else if (instanceData.contains("numMonitoredCities")) {
        initialMonitoredCities = instanceData.value("numMonitoredCities").toInt(&ok);
    }

    if (ok && initialMonitoredCities >= 1 && initialMonitoredCities <= 10) {
        model()->setNumMonitoredCities(initialMonitoredCities);
    }
}

Weather::~Weather()
{
}

QString Weather::title() const
{
    return appletTitle;
}

void Weather::addWeatherCity()
{
    WeatherModel *m = model();
    if (m->numMonitoredCities() < 10) {
        m->setNumMonitoredCities(m->numMonitoredCities() + 1);
        instanceData.setValue("numMonitoredCities", m->numMonitoredCities());
    }
}

void Weather::removeWeatherCity()
{
    WeatherModel *m = model();
    if (m->numMonitoredCities() > 1) {
        m->setNumMonitoredCities(m->numMonitoredCities() - 1);
        instanceData.setValue("numMonitoredCities", m->numMonitoredCities());
    }
}

void Weather::enterInfiniteLoop() const
{
    qDebug() << "Entering an infinite loop. Goodbye";
    while (true) {
        usleep(1000000);
    }
}

void Weather::crash() const
{
    qDebug() << "Crash";
    int a = 1, b = 1;
    a = a / (a - b);
}

void Weather::settingChanged(const QString &key, const QVariant &value)
{
    QRegExp cityRE("city([1-9]|10)");
    if (cityRE.exactMatch(key) && value.canConvert(QVariant::String)) {
        bool ok = false;
        int index = cityRE.cap(1).toInt(&ok);
        // Decrease the index to 0-9 interval
        --index;
        if (ok) {
            QList<QString> cities = model()->cities();
            cities[index] = value.toString();
            model()->setCities(cities);
        }
    } else if (key == "unit" && value.canConvert(QVariant::Int)) {
        bool ok = false;
        int unit = value.toInt(&ok);
        if (ok && (unit == 0 || unit == 1)) {
            // 0 means celsius, 1 means fahrenheit
            model()->setUnit(unit);
        }
    } else if (key == "togglelocal" && value.canConvert(QVariant::Bool)) {
        model()->setShowLocal(value.toBool());
    } else if (key == "localtemperature" && value.canConvert(QVariant::Int)) {
        bool ok = false;
        int tempe = value.toInt(&ok);
        if (ok) {
            model()->setLocalTemperature(tempe);
        }
    }
}
