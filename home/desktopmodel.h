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


#ifndef DESKTOPMODEL_H_
#define DESKTOPMODEL_H_

#include <DuiWidgetModel>

class DesktopModel : public DuiWidgetModel
{
    Q_OBJECT
    DUI_MODEL(DesktopModel)

    //! Set to true when the notification area is open, the desktop will then draw the background underneath the notification area
    DUI_MODEL_PROPERTY(bool, notificationAreaOpen, NotificationAreaOpen, true, false)
};

#endif /* DESKTOPMODEL_H_ */
