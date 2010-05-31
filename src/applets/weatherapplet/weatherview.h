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

#ifndef WEATHERVIEW_H
#define WEATHERVIEW_H

#include <MWidgetView>
#include "weathermodel.h"

class Weather;
class WeatherButton;
class MLayout;
class MLinearLayoutPolicy;
class MLabel;

/*!
 * The example view allows the user to paint on a canvas using the mouse.
 * It also provides controls for clearing the canvas and selecting the
 * color of the pen.
 */
class WeatherView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(WeatherModel, MWidgetStyle)

public:
    /*!
     * Constructs a WeatherView.
     *
     * \param desktop the Weather controller to be used
     */
    WeatherView(Weather *controller);

    /*!
     * Destroys the WeatherView.
     */
    virtual ~WeatherView();

    //! \reimp
    virtual void timerEvent(QTimerEvent *event);
    //! \reimp_end

signals:
    /*!
     * \brief Informs about a change in the title of the applet
     */
    void setAppletTitle(QString title);

private:
    /*!
     * Adds a weather button as the last item.
     */
    void addWeatherButton();

    /*!
     * Removes the last weather button.
     */
    void removeWeatherButton();

public slots:
    /*!
     * Toggles timer mode on/off
     */
    void toggleTimerMode();

    /*!
     * Toggles if local city is shown or not.
     */
    void toggleShowLocalCity(bool isShow);

    /*!
     * Changes local city temperature.
     */
    void changeLocalTemperature(int newTemperature);

protected:
    //! \reimp
    virtual void setupModel();
    //! \reimp_end

protected slots:
    //! \reimp
    virtual void updateData(const QList<const char *> &modifications);
    //! \reimp_end

private:
    //! Controller
    Weather *controller;

    //! Layout for the weather buttons
    MLayout *weatherButtonLayout;
    //! Layout policy for the weather buttons
    MLinearLayoutPolicy *weatherButtonLayoutPolicy;

    //! Timer mode
    bool timerMode;
    //! Id for the timer
    int timerId;
    //! A label to show the progress of the timer
    MLabel *timerLabel;

    //! Button for local weather
    WeatherButton *localButton;
};

#endif // WEATHERVIEW_H
