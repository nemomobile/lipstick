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

#ifndef TRANSFORMLAYOUTANIMATIONSTYLE_H
#define TRANSFORMLAYOUTANIMATIONSTYLE_H

#include <mlayoutanimationstyle.h>

#include <QtGlobal>
#include <QEasingCurve>

class TransformLayoutAnimationStyle : public MAnimationStyle
{
    Q_OBJECT
    M_STYLE(TransformLayoutAnimationStyle)

    /*! The easing curve used by the animation */
    M_STYLE_ATTRIBUTE(QEasingCurve, easingCurve, EasingCurve)

    /*! Minimum speed at which the animation progresses when not manually controlled.
        The value is the amount added to the progress each frame */
    M_STYLE_ATTRIBUTE(qreal, minimumSpeed, MinimumSpeed)

    /*! Smoothness value for filtering out sudden changes in animation speed due to user input.
        The value has range 0 to 1 where higher values are more smooth */
    M_STYLE_ATTRIBUTE(qreal, speedSmoothness, SpeedSmoothness)
};

class TransformLayoutAnimationStyleContainer : public MAnimationStyleContainer
{
    M_STYLE_CONTAINER(TransformLayoutAnimationStyle)
};

#endif
