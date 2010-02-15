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

#ifndef STATUSAREA_H
#define STATUSAREA_H

#include "statusareamodel.h"

#include <DuiButton>

/*!
 * Status area is the top part on the home screen. It contains the clock,
 * indicator area and a search button.
 */
class StatusArea : public DuiWidgetController
{
    Q_OBJECT
    DUI_CONTROLLER(StatusArea)

public:
    /*!
     * Constructs a new StatusArea.
     *
     * \param parent the parent widget of the status area
     */
    StatusArea(DuiWidget *parent = NULL);

    //! \reimp
    bool sceneEvent(QEvent *event);
    //! \reimp_end
signals:
    /*!
     * Emitted when the notification area button is pressed.
     */
    void toggleNotificationArea();

private:
    //! The name of the status indicator menu service
    static const QString STATUS_INDICATOR_MENU_SERVICE_NAME;
    //! position of mouse button press(firstPos) and position of last point of mouse move(lastPos)
    QPointF firstPos, lastPos;

    /*!
     * Shows the status indicator menu when the user swipes the status area
     */
    void showStatusIndicatorMenu();
};

#endif // STATUSAREA_H
