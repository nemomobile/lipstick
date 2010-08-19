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

#include <QFont>
#include <QColor>
#include <QPointF>
#include <QSize>
#include <mbuttonstyle.h>

class QPixmap;
class MScalableImage;

class SwitcherButtonStyle : public MButtonStyle
{
    Q_OBJECT
    M_STYLE(SwitcherButtonStyle)

    //! Close button vertical offset relative to top-right corner of switcher button
    M_STYLE_ATTRIBUTE(qreal, closeButtonVOffset, CloseButtonVOffset)

    //! Close button horizontal offset relative to top-right corner of switcher button
    M_STYLE_ATTRIBUTE(qreal, closeButtonHOffset, CloseButtonHOffset)

    //! The close button icon
    M_STYLE_ATTRIBUTE(QString, closeIcon, CloseIcon)

    //! The image to be drawn as the container for the button
    M_STYLE_PTR_ATTRIBUTE(MScalableImage *, containerImage, ContainerImage)

    //! The height of the title bar
    M_STYLE_ATTRIBUTE(int, titleBarHeight, TitleBarHeight)
};

class SwitcherButtonStyleContainer : public MButtonStyleContainer
{
    M_STYLE_CONTAINER(SwitcherButtonStyle)
    M_STYLE_MODE(Small)
    M_STYLE_MODE(Medium)
    M_STYLE_MODE(Large)
};

#endif /* SWITCHERBUTTONSTYLE_H_ */
