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

#ifndef QUICKLAUNCHBARMODEL_H_
#define QUICKLAUNCHBARMODEL_H_

#include <MWidgetModel>
#include "launcherbutton.h"

typedef QMap<int, QSharedPointer<LauncherButton> > ButtonMap;

/*!
 * A model for the QuickLaunchBar widget.
 */
class QuickLaunchBarModel : public MWidgetModel
{
    Q_OBJECT
    M_MODEL(QuickLaunchBarModel)

    //! Mapping of buttons to positions
    M_MODEL_PROPERTY(ButtonMap, buttons, Buttons, true, ButtonMap())
};

#endif /* QUICKLAUNCHBARMODEL_H_ */
