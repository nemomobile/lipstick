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
#include <MDesktopEntry>

class LauncherButtonModel : public MButtonModel
{
    Q_OBJECT
    M_MODEL(LauncherButtonModel)

    Q_ENUMS(State)

public:
    //! Possible launcher button states
    enum State {
        //! Button state when application is downloading
        Downloading,
        //! Button state when application is installing
        Installing,
        //! Button state when application is installed successfully
        Installed,
        //! Button state when application is launching
        Launching,
        //! Button state when application installation has failed
        Broken
    };

    //! Desktop entry that launcher button is representing
    M_MODEL_PROPERTY(QSharedPointer<MDesktopEntry>, desktopEntry, DesktopEntry, true, QSharedPointer<MDesktopEntry>())
    //! The current state of this button
    M_MODEL_PROPERTY(LauncherButtonModel::State, buttonState, ButtonState, true, Installed)
    //! Progress of button operation, between 0 and 100 (eg. for download)
    M_MODEL_PROPERTY(int, operationProgress, OperationProgress, true, 0)
};

#endif /* LAUNCHERBUTTONMODEL_H_ */
