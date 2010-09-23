/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libmeegotouch.
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

#ifndef LAUNCHERPOSITIONINDICATORSTYLE_H
#define LAUNCHERPOSITIONINDICATORSTYLE_H

#include <mwidgetstyle.h>

class PagePositionIndicatorStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(PagePositionIndicatorStyle)

    //! Spacing between position indicator icons
    M_STYLE_ATTRIBUTE(int, spacing, Spacing)
    //! Size of the position indicator icons
    M_STYLE_ATTRIBUTE(QSize, iconSize, IconSize)
    //! Image for a focused page indicator icon
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *, focusedIndicatorImage, FocusedIndicatorImage)
    //! Image for an unfocused page indicator icon
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *, unfocusedIndicatorImage, UnfocusedIndicatorImage)
    //! If the indicator is focused between pages
    M_STYLE_ATTRIBUTE(bool, focusedBetweenPages, FocusedBetweenPages)

};

class PagePositionIndicatorStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(PagePositionIndicatorStyle)
};

#endif

