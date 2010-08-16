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

#ifndef LAUNCHERPAGEMODEL_H
#define LAUNCHERPAGEMODEL_H

#include <mwidgetmodel.h>
#include <QList>
#include "launcherbutton.h"

class LauncherPageModel : public MWidgetModel
{
    Q_OBJECT
    M_MODEL(LauncherPageModel)

public:
    typedef QList< QSharedPointer<LauncherButton> > LauncherButtonList;

private:
    //! The list of widgets to show in the launcher
    M_MODEL_PROPERTY(LauncherPageModel::LauncherButtonList, launcherButtons, LauncherButtons, true, LauncherButtonList())

    //! Maximum number of buttons in page.
    M_MODEL_PROPERTY(int, maxButtons, MaxButtons, true, 12)
};
#endif
