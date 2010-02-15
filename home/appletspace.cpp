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

#include "appletspace.h"
#include "appletspacemodel.h"
#include <QDebug>

AppletSpace::AppletSpace(DuiWidget *parent) :
    DuiWidgetController(new AppletSpaceModel, parent)
{
}

AppletSpace::~AppletSpace()
{
}

void AppletSpace::setEnabled(bool enabled)
{
    DuiWidgetController::setEnabled(enabled);

    // Show or hide the button
    model()->setCloseButtonVisible(enabled);
}
