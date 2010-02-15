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
#ifndef TESTNOTIFICATIONPARAMETERS
#define TESTNOTIFICATIONPARAMETERS

#include "notificationwidgetparameterfactory.h"
#include "notificationparameters.h"

class TestNotificationParameters : public NotificationParameters
{
public:
    TestNotificationParameters(QString imageId = "", QString body = "", QString iconId = "", QString action = "");
};

TestNotificationParameters::TestNotificationParameters(QString imageId, QString body, QString iconId, QString action)
{
    if (!imageId.isEmpty()) {
        add(NotificationWidgetParameterFactory::createImageIdParameter(imageId));
    }
    if (!body.isEmpty()) {
        add(NotificationWidgetParameterFactory::createBodyParameter(body));
    }
    if (!iconId.isEmpty()) {
        add(NotificationWidgetParameterFactory::createIconIdParameter(iconId));
    }
    if (!action.isEmpty()) {
        add(NotificationWidgetParameterFactory::createActionParameter(action));
    }
}

#endif
