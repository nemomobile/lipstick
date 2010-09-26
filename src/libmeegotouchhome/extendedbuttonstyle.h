/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouchhome.
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

#ifndef EXTENDEDBUTTONSTYLE_H_
#define EXTENDEDBUTTONSTYLE_H_

#include <mbuttoniconstyle.h>

class ExtendedButtonStyle : public MButtonIconStyle
{
    Q_OBJECT
    M_STYLE(ExtendedButtonStyle)

    //! The amount of pixels to extend the bounding rectangle with in different directions
    M_STYLE_ATTRIBUTE(int, extendLeft, ExtendLeft);
    M_STYLE_ATTRIBUTE(int, extendRight, ExtendRight);
    M_STYLE_ATTRIBUTE(int, extendTop, ExtendTop);
    M_STYLE_ATTRIBUTE(int, extendBottom, ExtendBottom);
};

class ExtendedButtonStyleContainer : public MButtonIconStyleContainer
{
    M_STYLE_CONTAINER(ExtendedButtonStyle)
};

#endif /* EXTENDEDBUTTONSTYLE_H_ */
