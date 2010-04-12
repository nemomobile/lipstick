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

#ifndef SWITCHERBUTTONMODEL_H_
#define SWITCHERBUTTONMODEL_H_

#include <mbuttonmodel.h>
#include <X11/X.h>

class SwitcherButtonModel : public MButtonModel
{
    Q_OBJECT
    M_MODEL(SwitcherButtonModel)

public:
    //! The possible UI elements that the mouse can be pressed on
    enum PressedType {
        NonePressed,
        ButtonPressed,
        ClosePressed
    };

    enum ViewModeType {
        Small,
        Medium,
        Large
    };
private:
    //! The X Window represented by the switcher buttom
    M_MODEL_PROPERTY(Window, xWindow, XWindow, true, 0)
    //! The UI element that the mouse was pressed on
    M_MODEL_PROPERTY(SwitcherButtonModel::PressedType, pressed, Pressed, true, NonePressed)
    //! Represents the if this switcher button is emphasized or not
    M_MODEL_PROPERTY(bool, emphasized, Emphasized, true, false)
    //! The current view mode of this button
    M_MODEL_PROPERTY(SwitcherButtonModel::ViewModeType, viewMode, ViewMode, true, Large)
};


#endif /* SWITCHERBUTTONMODEL_H_ */
