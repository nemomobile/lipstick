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

#ifndef SWITCHERBUTTONSTYLE_H_
#define SWITCHERBUTTONSTYLE_H_

#include <mbuttonstyle.h>

class MScalableImage;

class SwitcherButtonStyle : public MButtonStyle
{
    Q_OBJECT
    M_STYLE(SwitcherButtonStyle)

     //! The image to be drawn as the container for the button
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *, containerImage, ContainerImage)

    //! The amount of pixels to be cropped from the top of the window
    M_STYLE_ATTRIBUTE(int, croppingTop, CroppingTop)
};

class SwitcherButtonStyleContainer : public MButtonStyleContainer
{
    M_STYLE_CONTAINER(SwitcherButtonStyle)
    M_STYLE_MODE(Small)
    M_STYLE_MODE(Medium)
    M_STYLE_MODE(Large)
};

#endif /* SWITCHERBUTTONSTYLE_H_ */
