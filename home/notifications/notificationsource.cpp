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

#include "notificationsource.h"
#include "notificationwidgetparameterfactory.h"
#include "genericnotificationparameterfactory.h"

NotificationSource::NotificationSource(NotificationManagerInterface &m)
    : manager(m)
{
}

NotificationParameters NotificationSource::notificationParameters(const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count)
{
    // Create the notification widget body text and icon ID
    QString widgetBody;

    if (!summary.isEmpty() || !body.isEmpty()) {
        if (!summary.isEmpty()) {
            widgetBody.append("<p><b>");
            widgetBody.append(summary);
            widgetBody.append("</b></p>");
        }

        if (!body.isEmpty()) {
            widgetBody.append("<p>");
            widgetBody.append(body);
            widgetBody.append("</p>");
        }
    }

    NotificationParameters parameters;
    parameters.add(GenericNotificationParameterFactory::createEventTypeParameter(eventType));
    parameters.add(GenericNotificationParameterFactory::createCountParameter(count));
    if (!widgetBody.isEmpty()) {
        parameters.add(NotificationWidgetParameterFactory::createBodyParameter(widgetBody));
    }
    if (!imageURI.isEmpty()) {
        parameters.add(NotificationWidgetParameterFactory::createImageIdParameter(imageURI));
    }
    if (!action.isEmpty()) {
        parameters.add(NotificationWidgetParameterFactory::createActionParameter(action));
    }

    return parameters;
}
