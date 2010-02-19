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


#ifndef SWITCHERSTYLE_H_
#define SWITCHERSTYLE_H_

#include <DuiWidgetStyle>
#include <QFont>

class SwitcherStyle : public DuiWidgetStyle
{
    Q_OBJECT
    DUI_STYLE(SwitcherStyle)

    /*! Defines how much the focused switcher button will be scaled */
    DUI_STYLE_ATTRIBUTE(qreal, scaleFactor, ScaleFactor)

    /*!
     * Defines how much extra the focused switcher button will move 
     * horizontally "out of the way".
     */
    DUI_STYLE_ATTRIBUTE(qreal, fastForward, FastForward)

    /*!
     * Defines how much the focused switcher button will rotate at maximum 
     * during horizontal movement
     */
    DUI_STYLE_ATTRIBUTE(qreal, itemRotation, ItemRotation)
    /*!
     * Defines how mush the items will over lap when they are stationary
     */
    DUI_STYLE_ATTRIBUTE(qreal, itemOverLap, ItemOverLap)

     /*!
     * Defines horizontal spacing between switcher buttons.
     */
    DUI_STYLE_ATTRIBUTE(qreal, buttonHorizontalSpacing, ButtonHorizontalSpacing)

    /*!
     * Defines vertical spacing between switcher buttons.
     */
    DUI_STYLE_ATTRIBUTE(qreal, buttonVerticalSpacing, ButtonVerticalSpacing)

     /*!
     * Defines amount of rows in switcher view.
     */
    DUI_STYLE_ATTRIBUTE(int, rowsPerPage, RowsPerPage)

    /*!
     * Defines amount of columns in switcher view.
     */
    DUI_STYLE_ATTRIBUTE(int, columnsPerPage, ColumnsPerPage)
};

class SwitcherStyleContainer : public DuiWidgetStyleContainer
{
    DUI_STYLE_CONTAINER(SwitcherStyle)
};

#endif /* SWITCHERSTYLE_H_ */
