/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include "homeapplication.h"
#include "xeventlistener.h"

XEventListener::XEventListener()
{
    HomeApplication *app = dynamic_cast<HomeApplication*>(qApp);
    if (app) {
        app->addXEventListener(this);
    }
}

XEventListener::~XEventListener()
{
    HomeApplication *app = dynamic_cast<HomeApplication*>(qApp);
    if (app) {
        app->removeXEventListener(this);
    }
}
