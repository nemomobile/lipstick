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

#include "contextframeworkcontext.h"
#include <QVariant>

ContextFrameworkItem::ContextFrameworkItem(const QString &key)
    : property(key)
{
    connect(&property, SIGNAL(valueChanged()), this, SIGNAL(contentsChanged()));
}

QVariant ContextFrameworkItem::value() const
{
    return property.value();
}

ContextItem *ContextFrameworkContext::createContextItem(const QString &key)
{
    return new ContextFrameworkItem(key);
}
