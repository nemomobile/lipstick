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

#ifndef LAUNCHERMODEL_H_
#define LAUNCHERMODEL_H_

#include <mwidgetmodel.h>
#include "launcherpage.h"

class LauncherModel : public MWidgetModel
{
    Q_OBJECT
    M_MODEL(LauncherModel)

public:
    typedef QList< QSharedPointer<LauncherPage> > LauncherPageList;

private:
    //! The list of widgets to show in the launcher
    M_MODEL_PROPERTY(LauncherModel::LauncherPageList, launcherPages, LauncherPages, true, LauncherPageList())
};

#endif /* LAUNCHERMODEL_H_ */
