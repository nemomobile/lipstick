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

#ifndef WEATHERBUTTON_H
#define WEATHERBUTTON_H

#include <QTimer>
#include <MButton>
#include "weatherbuttonmodel.h"

/*!
 * WeatherButton widget represents an open window in the Weather.
 */
class WeatherButton : public MButton
{
    Q_OBJECT
    M_CONTROLLER(WeatherButton)

public:
    /*!
     * Constructs an WeatherButton.
     *
     * \param title the Name of the city represented by this button
     * \param parent parent MWidget
     * \param iconIndex the index of the icon to use (0-4)
     * \param timeout the time in milliseconds until the icon should be changed
     */
    WeatherButton(const QString &title, MWidget *parent = NULL, int iconIndex = 0, int timeout = 750);

    /*!
     * Destroys the WeatherButton.
     */
    virtual ~WeatherButton();

    //! \reimp
    virtual void timerEvent(QTimerEvent *event);
    //! \reimp_end

public slots:
    /*!
     * Sets the temperature unit this button uses.
     * \param unit the unit type.
     */
    void setUnit(WeatherButtonModel::UnitType unit);

    /*!
     * Toggles wether to use the generic style or a hardcoded one.
     * By default the generic style is used.
     */
    void toggleUseStyle();

private:
    //! Icon timer
    QTimer iconTimer;
};

#endif // WEATHERBUTTON_H
