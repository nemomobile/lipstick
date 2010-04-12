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
#ifndef LAUNCHERSTYLE_H
#define LAUNCHERSTYLE_H

#include <QPoint>
#include <QColor>
#include <QFont>
#include <mextendingbackgroundstyle.h>

class MScalableImage;

class LauncherStyle : public MExtendingBackgroundStyle
{
    Q_OBJECT
    M_STYLE(LauncherStyle)
};

class LauncherStyleContainer : public MExtendingBackgroundStyleContainer
{
    M_STYLE_CONTAINER(LauncherStyle)
};

#endif
