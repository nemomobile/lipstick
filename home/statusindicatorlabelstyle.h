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


#ifndef STATUSINDICATORLABELSTYLE_H_
#define STATUSINDICATORLABELSTYLE_H_

#include <DuiWidgetStyle>

class StatusIndicatorLabelStyle : public DuiWidgetStyle
{
    Q_OBJECT
    DUI_STYLE(StatusIndicatorLabelStyle)
};

class StatusIndicatorLabelStyleContainer : public DuiWidgetStyleContainer
{
    DUI_STYLE_CONTAINER(StatusIndicatorLabelStyle)
};

#endif /* STATUSINDICATORLABELSTYLE_H_ */
