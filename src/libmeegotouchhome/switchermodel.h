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


#ifndef SWITCHERMODEL_H_
#define SWITCHERMODEL_H_

#include <MWidgetModel>
#include <X11/Xlib.h>
#include "switcherbutton.h"

class SwitcherModel : public MWidgetModel
{
    Q_OBJECT

public:
    enum Mode {
        Detailview = 1,
        Overview
    };

    typedef QList< QSharedPointer<SwitcherButton> > ButtonList;

private:
    M_MODEL(SwitcherModel)
    M_MODEL_PROPERTY(SwitcherModel::ButtonList, buttons, Buttons, true, ButtonList())
    M_MODEL_PROPERTY(SwitcherModel::Mode, switcherMode, SwitcherMode, true, Overview)
    M_MODEL_PROPERTY(Window, topmostWindow, TopmostWindow, true, 0)
};

#endif /* SWITCHERMODEL_H_ */
