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

#include "notificationparameters.h"
#include "notificationparameter.h"

NotificationParameters::NotificationParameters()
{
}

NotificationParameters::~NotificationParameters()
{
}

void NotificationParameters::add(const QString &parameter, const QVariant &value)
{
    parameterValues.insert(parameter, value);
}

void NotificationParameters::add(const NotificationParameter &parameter)
{
    parameterValues.insert(parameter.name(), parameter.value());
}

QVariant NotificationParameters::value(const QString &parameter) const
{
    return parameterValues.value(parameter);
}


QDataStream &operator<<(QDataStream &datastream, const NotificationParameters &parameters)
{
    return datastream << parameters.parameterValues;
}

QDataStream &operator>>(QDataStream &datastream, NotificationParameters &parameters)
{
    return datastream >> parameters.parameterValues;
}
