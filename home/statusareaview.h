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

#ifndef STATUSAREAVIEW_H
#define STATUSAREAVIEW_H

#include <DuiButtonView>
#include <DuiNamespace>

#include "statusareastyle.h"
#include "statusareamodel.h"
#include "contextframeworkcontext.h"

class Notifier;
class StatusArea;
class StatusIndicator;
class DuiPannableViewport;
class DuiSceneWindow;

/*!
 * Status area view draws the status area.
 */
class StatusAreaView : public DuiWidgetView
{
    Q_OBJECT
    DUI_VIEW(StatusAreaModel, StatusAreaStyle)

public:
    /*!
     * Constructs a view for a status area button.
     *
     * \param controller the controller of this StatusAreaView
     */
    StatusAreaView(StatusArea *controller);

    /*!
     * Destructor.
     */
    virtual ~StatusAreaView();

signals:
    /*!
     * A signal which is sent whenever the notification area button is pressed.
     */
    void toggleNotificationArea();

private:
    //! Context framework application context for the indicators
    ContextFrameworkContext contextFrameworkContext;

    //! The controller instance
    StatusArea *controller;

    //! Alarm indicator
    StatusIndicator *alarmIndicator;

    //! Battery indicator
    StatusIndicator *batteryIndicator;

    //! Phone signal strength indicator
    StatusIndicator *phoneSignalStrengthIndicator;

    //! Internet connection indicator
    StatusIndicator *internetConnectionIndicator;

    //! Bluetooth indicator
    StatusIndicator *bluetoothIndicator;

    //! Notifier
    Notifier *notifier;

    //! Placeholder for the launcher button
    DuiButton *launcherButtonPlaceholder;
};

#endif // STATUSAREAVIEW_H
