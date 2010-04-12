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
#ifndef WEATHERBUTTONVIEW_H
#define WEATHERBUTTONVIEW_H

#include <QTimer>
#include <MWidgetView>
#include "weatherbuttonstyle.h"
#include "weatherbuttonmodel.h"

class WeatherButton;
class QPainter;
class QPixmap;

/*!
 * \class WeatherButtonView
 *
 * \brief WeatherButtonView implements a weather style view
 */
class WeatherButtonView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(WeatherButtonModel, WeatherButtonStyle)

public:
    /*!
     * \brief Constructs a new WeatherButtonView.
     *
     * \param controller The button's controller
     */
    WeatherButtonView(WeatherButton *controller);

    /*!
     * \brief Destroys the WeatherButtonView.
     */
    virtual ~WeatherButtonView();

protected:
    //! \reimp
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void setupModel();
    //! \reimp_end

protected slots:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    //! WeatherButton controller
    WeatherButton *controller;

    //! Number of icons
    static const int numberOfIcons;

    //! Icons
    static const QPixmap **icons;

    //! Icons reference count
    static int iconsReferenceCount;

    Q_DISABLE_COPY(WeatherButtonView);
};

#endif
