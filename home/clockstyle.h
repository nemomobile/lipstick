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

#ifndef CLOCKSTYLE_H_
#define CLOCKSTYLE_H_

#include <DuiWidgetStyle>

/*!
 * A style class for the Clock widget.
 */
class ClockStyle : public DuiWidgetStyle
{
    Q_OBJECT
    DUI_STYLE(ClockStyle)

    //! Formatting string for the time
    DUI_STYLE_ATTRIBUTE(QString, timeFormat, TimeFormat)
};

class ClockStyleContainer : public DuiWidgetStyleContainer
{
    DUI_STYLE_CONTAINER(ClockStyle)
};

#endif /* CLOCKSTYLE_H_ */
