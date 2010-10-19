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

#ifndef LAUNCHERBUTTONSTYLE_H_
#define LAUNCHERBUTTONSTYLE_H_

#include <mbuttoniconstyle.h>

class LauncherButtonStyle : public MButtonIconStyle
{
    Q_OBJECT
    M_STYLE(LauncherButtonStyle)

    //! Launch progress indicator visibility.
    M_STYLE_ATTRIBUTE(bool, showLaunchProgress, ShowLaunchProgress)

    //! The timeout for the progress indicator (in milliseconds)
    M_STYLE_ATTRIBUTE(int, launchProgressIndicatorTimeout, LaunchProgressIndicatorTimeout)

    /*!
     * The progress indicator icon size (this is needed for implementation reasons
     * as progress indicator icons might be different size than button icon).
     */
    M_STYLE_ATTRIBUTE(QSize, progressIndicatorIconSize, ProgressIndicatorIconSize)

    //! Placeholder icon for downloading state
    M_STYLE_ATTRIBUTE(QString, downloadingPlaceholderIcon, DownloadingPlaceholderIcon);
    //! Placeholder icon for installing state
    M_STYLE_ATTRIBUTE(QString, installingPlaceholderIcon, InstallingPlaceholderIcon);
    //! Placeholder icon for broken state
    M_STYLE_ATTRIBUTE(QString, brokenPlaceholderIcon, BrokenPlaceholderIcon);
};

class LauncherButtonStyleContainer : public MButtonIconStyleContainer
{
    M_STYLE_CONTAINER(LauncherButtonStyle)
};

#endif /* LAUNCHERBUTTONSTYLE_H_ */
