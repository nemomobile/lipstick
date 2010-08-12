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

#ifndef LAUNCHERBUTTONPROGRESSINDICATORMODEL_H
#define LAUNCHERBUTTONPROGRESSINDICATORMODEL_H

#include "mprogressindicatormodel.h"
#include "launcherbuttonmodel.h"

class LauncherButtonProgressIndicatorModel : public MProgressIndicatorModel
{
    Q_OBJECT
    M_MODEL(LauncherButtonProgressIndicatorModel)

    //! The current state of this progress indicator
    M_MODEL_PROPERTY(LauncherButtonModel::State, indicatorState, IndicatorState, true, LauncherButtonModel::Launching)
};

#endif
