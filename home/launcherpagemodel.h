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

#ifndef LAUNCHERPAGEMODEL_H
#define LAUNCHERPAGEMODEL_H

#include <duiwidgetmodel.h>
#include <QList>
#include "launcherbutton.h"

class LauncherPageModel : public DuiWidgetModel
{
    Q_OBJECT
    DUI_MODEL(LauncherPageModel)

public:
    typedef QList< QSharedPointer<LauncherButton> > LauncherButtonList;

private:
    //! The list of widgets to show in the launcher
    DUI_MODEL_PROPERTY(LauncherPageModel::LauncherButtonList, launcherButtons, LauncherButtons, true, LauncherButtonList())

    //! Maximum number of buttons in page.
    DUI_MODEL_PROPERTY(int, maxButtons, MaxButtons, true, 12)
};
#endif
