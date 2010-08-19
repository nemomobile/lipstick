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

#ifndef SWITCHERBUTTONWITHTITLEBARSTYLE_H_
#define SWITCHERBUTTONWITHTITLEBARSTYLE_H_

#include "switcherbuttonstyle.h"

class SwitcherButtonWithTitleBarStyle : public SwitcherButtonStyle
{
    Q_OBJECT
    M_STYLE(SwitcherButtonWithTitleBarStyle)

    //! Close button vertical offset relative to top-right corner of switcher button
    M_STYLE_ATTRIBUTE(qreal, closeButtonVOffset, CloseButtonVOffset)

    //! Close button horizontal offset relative to top-right corner of switcher button
    M_STYLE_ATTRIBUTE(qreal, closeButtonHOffset, CloseButtonHOffset)

    //! The close button icon
    M_STYLE_ATTRIBUTE(QString, closeIcon, CloseIcon)
};

class SwitcherButtonWithTitleBarStyleContainer : public SwitcherButtonStyleContainer
{
    M_STYLE_CONTAINER(SwitcherButtonWithTitleBarStyle)
};

#endif /* SWITCHERBUTTONWITHTITLEBARSTYLE_H_ */
