/***************************************************************************
**
** Copyright (C) 2013 Jolla Ltd.
** Contact: Aaron Kennedy <aaron.kennedy@jollamobile.com>
**
** This file is part of lipstick.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include "lipstickapi.h"

#include "homeapplication.h"
#include "compositor/lipstickcompositor.h"

LipstickApi::LipstickApi(QObject *parent)
: QObject(parent)
{
    QObject::connect(HomeApplication::instance(), SIGNAL(homeActiveChanged()), this, SIGNAL(activeChanged()));
}

bool LipstickApi::active() const
{
    return HomeApplication::instance()->homeActive();
}

QObject *LipstickApi::compositor() const
{
    return LipstickCompositor::instance();
}
