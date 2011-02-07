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

#ifndef LAUNCHERACTION_H
#define LAUNCHERACTION_H

#include <contentaction.h>

/*!
 * A convenience wrapper around libcontentaction's ContentAction::Action.
 * The object is used to turn .desktop files into objects that take care
 * of invoking of the default action (D-Bus method call, execution of
 * a binary, etc) specified in these .desktop files.
 *
 * Instantiated with a valid .desktop file path, the object provides a
 * trigger() method that asynchronously triggers the default action for that
 * .desktop file.
 */
class LauncherAction : public ContentAction::Action
{
    public:
        /*!
         * Constructs an empty LauncherAction object.
         */
        LauncherAction();

        /*!
         * Constructs a LauncherAction object from a given .desktop file.
         *
         * \param dekstopEntry Path to the .desktop file to initialize
         * the action for.
         */
        LauncherAction(const QString& dekstopEntry);
};

//! Comparison operator for LauncherAction objects
bool operator==(const LauncherAction &, const LauncherAction &);
//! The reverse of the comparison operator for LauncherAction objects
bool operator!=(const LauncherAction &, const LauncherAction &);

#endif
