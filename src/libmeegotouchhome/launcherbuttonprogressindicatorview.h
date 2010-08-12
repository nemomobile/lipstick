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

#ifndef LAUNCHERBUTTONPROGRESSINDICATORVIEW_H
#define LAUNCHERBUTTONPROGRESSINDICATORVIEW_H

#include "mspinnerview.h"
#include "launcherbuttonprogressindicator.h"
#include "launcherbuttonprogressindicatorstyle.h"
#include "launcherbuttonstyle.h"

class LauncherButtonProgressIndicatorView : public MSpinnerView
{
    Q_OBJECT
    M_VIEW(LauncherButtonProgressIndicatorModel, LauncherButtonProgressIndicatorStyle)

public:
    LauncherButtonProgressIndicatorView(LauncherButtonProgressIndicator *controller);

protected:
    //! reimp
    void setupModel();
    void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    //! Updates indicator style mode according to state.
    void updateStyleMode();
};

#endif
