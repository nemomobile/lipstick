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


#ifndef LAUNCHERBUTTONSTYLE_H_
#define LAUNCHERBUTTONSTYLE_H_

#include <QFont>
#include <QColor>
#include <QPointF>
#include <QSize>
#include <QSizeF>
#include <duibuttoniconstyle.h>

class QPixmap;
class DuiScalableImage;

class LauncherButtonStyle : public DuiButtonIconStyle
{
    Q_OBJECT
    DUI_STYLE(LauncherButtonStyle)

    //! The position of title relative to top-left corner of the button
    DUI_STYLE_ATTRIBUTE(QPointF, textPosition, TextPosition)

    //! The size of the title
    DUI_STYLE_ATTRIBUTE(QSize, textSize, TextSize)

    //! The position of the icon relative to top-left corner of the button
    DUI_STYLE_ATTRIBUTE(QPointF, iconPosition, IconPosition)

    //! The image to be used as the container (background) of the button if the button represents a shortcut
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *, shortcutContainerImage, ShortcutContainerImage)

    //! The image to be used as the container (background) of the button if the button represents an invalid shortcut
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *, invalidShortcutContainerImage, InvalidShortcutContainerImage)

    //! The image to be used as the mask for a shortcut thumbnail
    DUI_STYLE_PTR_ATTRIBUTE(DuiScalableImage *, shortcutThumbnailMaskImage, ShortcutThumbnailMaskImage)

    //! The position of the shortcut thumbnail relative to top-left corner of the button
    DUI_STYLE_ATTRIBUTE(QPointF, shortcutThumbnailPosition, ShortcutThumbnailPosition)

    //! The size of the shortcut thumbnail
    DUI_STYLE_ATTRIBUTE(QSizeF, shortcutThumbnailSize,  ShortcutThumbnailSize)

    //! The position of title relative to top-left corner of the button if the button represents a shortcut
    DUI_STYLE_ATTRIBUTE(QPointF, shortcutTextPosition, ShortcutTextPosition)

    //! The size of the title if the button represents a shortcut
    DUI_STYLE_ATTRIBUTE(QSize, shortcutTextSize, ShortcutTextSize)

    //! The position of the icon relative to top-left corner of the button if the button represents a shortcut
    DUI_STYLE_ATTRIBUTE(QPointF, shortcutIconPosition, ShortcutIconPosition)

    //! The size of the icon if the button represents a shortcut
    DUI_STYLE_ATTRIBUTE(QSize, shortcutIconSize, ShortcutIconSize)
};

class LauncherButtonStyleContainer : public DuiButtonIconStyleContainer
{
    DUI_STYLE_CONTAINER(LauncherButtonStyle)
};

#endif /* LAUNCHERBUTTONSTYLE_H_ */
