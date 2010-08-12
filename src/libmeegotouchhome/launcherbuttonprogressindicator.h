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

#ifndef LAUNCHERBUTTONPROGRESSINDICATOR_H
#define LAUNCHERBUTTONPROGRESSINDICATOR_H

#include <MProgressIndicator>
#include "launcherbuttonmodel.h"
#include "launcherbuttonprogressindicatormodel.h"

class LauncherButtonProgressIndicator : public MProgressIndicator
{
    Q_OBJECT
    M_CONTROLLER(LauncherButtonProgressIndicator)

    Q_PROPERTY(LauncherButtonModel::State indicatorState WRITE setIndicatorState READ indicatorState)

public:
    LauncherButtonProgressIndicator(QGraphicsItem *parent = 0);

    ~LauncherButtonProgressIndicator();

    /*!
     * Set indicator state
     *
     * \param state State to be set
     */
    void setIndicatorState(LauncherButtonModel::State state);

    /*! Returns the state of the indicator
     * \return the state of the indicator
     */
    LauncherButtonModel::State indicatorState();
};

#endif
