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

#ifndef LAUNCHERBUTTONMODEL_H_
#define LAUNCHERBUTTONMODEL_H_

#include <mbuttonmodel.h>
#include "launcheraction.h"

class LauncherButtonModel : public MButtonModel
{
private:
    Q_OBJECT
    M_MODEL(LauncherButtonModel)

    //! Desktop entry file that launcher button is representing
    M_MODEL_PROPERTY(QString, desktopEntryFile, DesktopEntryFile, true, QString())
    //! Action for the desktop entry file
    M_MODEL_PROPERTY(LauncherAction, action, Action, true, LauncherAction())
    //! Whether to show a progress indicator or not
    M_MODEL_PROPERTY(bool, showProgressIndicator, ShowProgressIndicator, true, false)
};

#endif /* LAUNCHERBUTTONMODEL_H_ */
