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
#ifndef CLOCKVIEW_H
#define CLOCKVIEW_H

#include <DuiWidgetView>
#include "clockmodel.h"
#include "clockstyle.h"

class Clock;
class DuiLabel;

/*!
 * A view class for the Clock.
 */
class ClockView : public DuiWidgetView
{
    Q_OBJECT
    DUI_VIEW(ClockModel, ClockStyle)

public:
    /*!
     * Constructs a new view for Clock. This view shows the clock's
     * time as a digital number.
     */
    ClockView(Clock *controller);

protected:
    //! \reimp
    virtual void styleUpdated();
    void setupModel();
    //! \reimp_end

    //! A label to be used to render the clock time
    DuiLabel *label;

protected slots:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    /*!
     * Updates the visuals of this view
     */
    void updateLabel();

    //! The previous label
    QString previousLabel;
};

#endif
