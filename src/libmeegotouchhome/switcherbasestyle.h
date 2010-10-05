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


#ifndef SWITCHERBASESTYLE_H_
#define SWITCHERBASESTYLE_H_

#include <MWidgetStyle>
#include <QEasingCurve>

class SwitcherBaseStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(SwitcherBaseStyle)

    /*!
     * The length of the pinch gesture needed to make a full transition between modes
     */
    M_STYLE_ATTRIBUTE(qreal, pinchLength, PinchLength)

    /*!
     * The progress threshold of canceling the mode transition, range 0 to 1
     */
    M_STYLE_ATTRIBUTE(qreal, pinchCancelThreshold, PinchCancelThreshold)

    /*!
     * Scaling amount of bounce animation and zooming when overpinching.
     * The value is added to or subtracted from factor 1, so that value 0.3 will scale
     * to 0.7 in overview mode and 1.3 in detail view mode
     */
    M_STYLE_ATTRIBUTE(qreal, bounceScale, BounceScale)

    /*!
     * Duration of the bounce animation in milliseconds when transitioning to overview mode
     */
    M_STYLE_ATTRIBUTE(int, bounceDuration, BounceDuration)

    /*!
     * Easing curve of the bounce animation
     */
    M_STYLE_ATTRIBUTE(QEasingCurve, bounceCurve, BounceCurve)
};

class SwitcherBaseStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(SwitcherBaseStyle)
};

#endif
