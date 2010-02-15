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


#ifndef STATUSINDICATORIMAGESTYLE_H_
#define STATUSINDICATORIMAGESTYLE_H_

#include <DuiWidgetStyle>

class StatusIndicatorImageStyle : public DuiWidgetStyle
{
    Q_OBJECT
    DUI_STYLE(StatusIndicatorImageStyle)

    //! The list of IDs of the images to be shown in the status indicator separated by spaces
    DUI_STYLE_ATTRIBUTE(QString, imageList, ImageList)

    //! The number of milliseconds in which the images in imageList are animated
    DUI_STYLE_ATTRIBUTE(int, animationDuration, AnimationDuration)
};

class StatusIndicatorImageStyleContainer : public DuiWidgetStyleContainer
{
    DUI_STYLE_CONTAINER(StatusIndicatorImageStyle)
};

#endif /* STATUSINDICATORIMAGESTYLE_H_ */
