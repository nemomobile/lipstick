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

#ifndef SWITCHERBUTTONSTYLE_H_
#define SWITCHERBUTTONSTYLE_H_

#include <QFont>
#include <QColor>
#include <QPointF>
#include <QSize>
#include <duibuttonstyle.h>

class QPixmap;
class DuiScalableImage;

class SwitcherButtonStyle : public DuiButtonStyle
{
    Q_OBJECT
    DUI_STYLE(SwitcherButtonStyle)

    //! The opacity of the title
    DUI_STYLE_ATTRIBUTE(qreal, textOpacity, TextOpacity)

    //! The position of the icon relative to the top-left corner of the button
    DUI_STYLE_ATTRIBUTE(QPointF, iconPosition, IconPosition)

    //! The close button
    DUI_STYLE_ATTRIBUTE(qreal, closeButtonVerticalPosition, CloseButtonVerticalPosition)

    //! The close button icon
    DUI_STYLE_ATTRIBUTE(QString, closeIcon, CloseIcon)

    //! The image to be drawn as the container for the button
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *, containerImage, ContainerImage)
};

class SwitcherButtonStyleContainer : public DuiButtonStyleContainer
{
    DUI_STYLE_CONTAINER(SwitcherButtonStyle)
    DUI_STYLE_MODE(Small)
    DUI_STYLE_MODE(Medium)
    DUI_STYLE_MODE(Large)
};

#endif /* SWITCHERBUTTONSTYLE_H_ */
