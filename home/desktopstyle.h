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


#ifndef DESKTOPSTYLE_H_
#define DESKTOPSTYLE_H_

#include <MWidgetStyle>

class DesktopStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(DesktopStyle)

    //! The image to draw the desktop background with
    M_STYLE_PTR_ATTRIBUTE(QPixmap *, desktopBackgroundImage, DesktopBackgroundImage)
};

class DesktopStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(DesktopStyle)
};

#endif /* DESKTOPSTYLE_H_ */
