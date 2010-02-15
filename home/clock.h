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

#ifndef CLOCK_H
#define CLOCK_H

class QTimer;

#include <DuiWidgetController>
#include "clockmodel.h"

/*!
 * A widget for showing the current time.
 */
class Clock : public DuiWidgetController
{
    Q_OBJECT
    DUI_CONTROLLER(Clock)

public:
    /*!
     * Constructs a new Clock.
     */
    explicit Clock(QGraphicsItem *parent = NULL);

private:
    //! \reimp
    virtual void enterDisplayEvent();
    virtual void exitDisplayEvent();
    //! \reimp_end

    /*!
     * \brief Enables or disables model updates.
     *
     * \param enabled \c true if the model updates should be enabled, \c false otherwise
     */
    void setModelUpdatesEnabled(bool enabled);

    //! \reimp
    void timerEvent(QTimerEvent *event);
    //! \reimp_end

    //! ID for the update timer
    int updateTimerId;
};

#endif
