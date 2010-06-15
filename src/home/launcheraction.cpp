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

#include "launcheraction.h"

LauncherAction::LauncherAction()
    : Action()
{
}

LauncherAction::LauncherAction(const QString& desktopEntry)
    : Action(Action::defaultActionForFile(desktopEntry, "application/x-desktop"))
{
}

bool operator==(const LauncherAction &a, const LauncherAction &b)
{
    return 
        a.isValid() && b.isValid() &&
        a.name() == b.name() &&
        a.localizedName() == b.localizedName() &&
        a.icon() == b.icon();
}

bool operator!=(const LauncherAction &a, const LauncherAction &b)
{
    return !(a == b);
}
