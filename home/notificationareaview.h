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

#ifndef NOTIFICATIONAREAVIEW_H_
#define NOTIFICATIONAREAVIEW_H_

#include <DuiWidgetView>

#include "notificationareastyle.h"
#include "notificationareamodel.h"

class NotificationArea;
class DuiFlowLayoutPolicy;

class NotificationAreaView : public DuiWidgetView
{
    Q_OBJECT
    DUI_VIEW(NotificationAreaModel, NotificationAreaStyle)

public:
    /*!
     * Constructs a view for a notification area.
     *
     * \param controller the controller of this NotificationAreaView
     */
    NotificationAreaView(NotificationArea *controller);

    /*!
     * Destroys the notification area view.
     */
    virtual ~NotificationAreaView();

protected:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    //! The layout policy of the notification area
    DuiFlowLayoutPolicy *layoutPolicy;
};

#endif /* NOTIFICATIONAREAVIEW_H_ */
