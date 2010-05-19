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

#ifndef WEATHER_H
#define WEATHER_H

#include <MWidgetController>
#include "weathermodel.h"

class MDataStore;
class MDataAccess;
class MAppletMetaData;

/*!
 * The weather applet is only for demonstration purposes.
 */
class Weather : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(Weather)

    Q_PROPERTY(QString appletTitle READ title)

public:
    /*!
     * Constructs an Weather widget.
     * \param metaData the metadata object for the applet.
     * \param instanceData the instance data object for the applet.
     * \param settings the settings object for the applet.
     */
    Weather(const MAppletMetaData &metaData, MDataStore &instanceData, MDataAccess &settings);

    /*!
     * Destroys the Weather.
     */
    virtual ~Weather();

    /*!
     * Gets the title of this applet.
     * \return the title of the applet.
     */
    QString title() const;

public slots:
    /*!
     * Adds a monitored city.
     */
    void addWeatherCity();

    /*!
     * Removes the last monitored city.
     */
    void removeWeatherCity();

    /*!
     * Enters the process into an infinite loop.
     * This is used to simulate a malfunctioning applet.
     */
    void enterInfiniteLoop() const;

    /*!
     * Crashes the process. This is used to simulate a malfunctioning applet.
     */
    void crash() const;

private slots:
    /*!
     * \brief Notifies the applet that a setting value has changed.
     * \param key the name of the button to be changed. One of city1 to city10.
     * \param value the new value of the city. Must be able to be handled as a QString.
     */
    void settingChanged(const QString &key, const QVariant &value);

signals:
    /*!
     * \brief Informs about a change in the title of the applet
     */
    void setAppletTitle(QString title);

private:
    //! The instance data object for the applet
    MDataStore &instanceData;

    //! The title of the applet
    QString appletTitle;

};

#endif // WEATHER_H
