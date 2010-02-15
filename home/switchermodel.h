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


#ifndef SWITCHERMODEL_H_
#define SWITCHERMODEL_H_

#include <DuiWidgetModel>
#include "switcherbutton.h"

class SwitcherModel : public DuiWidgetModel
{
    Q_OBJECT

public:
    typedef QList< QSharedPointer<SwitcherButton> > ButtonList;

private:
    DUI_MODEL(SwitcherModel)
    DUI_MODEL_PROPERTY(SwitcherModel::ButtonList, buttons, Buttons, true, ButtonList())
};

#endif /* SWITCHERMODEL_H_ */
