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

#ifndef SWITCHERBUTTONMODEL_H_
#define SWITCHERBUTTONMODEL_H_

#include <duibuttonmodel.h>
#include <X11/X.h>

class SwitcherButtonModel : public DuiButtonModel
{
    Q_OBJECT
    DUI_MODEL(SwitcherButtonModel)

public:
    //! The possible UI elements that the mouse can be presed on
    enum PressedType {
        NonePressed,
        ButtonPressed,
        ClosePressed
    };

private:
    //! The X Window represented by the switcher buttom
    DUI_MODEL_PROPERTY(Window, xWindow, XWindow, true, 0)
    //! The UI element that the mouse was pressed on
    DUI_MODEL_PROPERTY(SwitcherButtonModel::PressedType, pressed, Pressed, true, NonePressed)
    //! Repsesents the if this switcher button is emphasized or not
    DUI_MODEL_PROPERTY(bool, emphasized, Emphasized, true, false)

};


#endif /* SWITCHERBUTTONMODEL_H_ */
