/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of mhome.
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

#include <MWidgetStyle>
#include "switcherbasestyle.h"
#include <QFont>
#include <QEasingCurve>

class SwitcherStyle : public SwitcherBaseStyle
{
    Q_OBJECT
    M_STYLE(SwitcherStyle)

     /*!
     * Defines horizontal spacing between switcher buttons.
     */
    M_STYLE_ATTRIBUTE(qreal, buttonHorizontalSpacing, ButtonHorizontalSpacing)

    /*!
     * Defines vertical spacing between switcher buttons.
     */
    M_STYLE_ATTRIBUTE(qreal, buttonVerticalSpacing, ButtonVerticalSpacing)

     /*!
     * Defines amount of rows in switcher view.
     */
    M_STYLE_ATTRIBUTE(int, rowsPerPage, RowsPerPage)

    /*!
     * Defines amount of columns in switcher view.
     */
    M_STYLE_ATTRIBUTE(int, columnsPerPage, ColumnsPerPage)
};

class SwitcherStyleContainer : public SwitcherBaseStyleContainer
{
    M_STYLE_CONTAINER(SwitcherStyle)
};

#endif /* SWITCHERSTYLE_H_ */
