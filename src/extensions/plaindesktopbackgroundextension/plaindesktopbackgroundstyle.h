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

#ifndef PLAINDESKTOPBACKGROUNDSTYLE_H_
#define PLAINDESKTOPBACKGROUNDSTYLE_H_

#include <MWidgetStyle>

class PlainDesktopBackgroundStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(PlainDesktopBackgroundStyle)

    //! The blur radius (in pixels)
    M_STYLE_ATTRIBUTE(int, blurRadius, BlurRadius)

    //! The brightness for dimming (from 0 to 1)
    M_STYLE_ATTRIBUTE(qreal, brightness, Brightness)

    //! The defocus duration (in ms)
    M_STYLE_ATTRIBUTE(int, defocusDuration, DefocusDuration)

    //! The defocus update interval (in ms)
    M_STYLE_ATTRIBUTE(int, defocusUpdateInterval, DefocusUpdateInterval)

    //! The name of the default background image
    M_STYLE_ATTRIBUTE(QString, defaultBackgroundImage, DefaultBackgroundImage)
};

class PlainDesktopBackgroundStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(PlainDesktopBackgroundStyle)
};

#endif /* PLAINDESKTOPBACKGROUNDSTYLE_H_ */
