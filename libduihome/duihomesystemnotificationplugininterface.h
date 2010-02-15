/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of duihome.
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

#ifndef DUIHOMESYSTEMNOTIFICATIONPLUGININTERFACE_H
#define DUIHOMESYSTEMNOTIFICATIONPLUGININTERFACE_H

#include <QtPlugin>
class DuiHomeSystemNotificationSourceInterface;

/*!
 * DuiSystemNotificationPluginInterface is an Interface to the system notification plugin loaded by home. A concrete implementation of this interface is in libduihome.
 */
class DuiHomeSystemNotificationPluginInterface
{
public:
    /*!
     * This method is called by the plugin loader in system notification source to set the notification source context
     * This method SHOULD NOT be called by the derived plugin.
     * \param sourceInterface System Notification source interface which is loading the plugin and which would be called for adding a new notification
     */
    virtual void initialize(DuiHomeSystemNotificationSourceInterface &sourceInterface) = 0;
};

Q_DECLARE_INTERFACE(DuiHomeSystemNotificationPluginInterface, "com.nokia.duihome.duihomesystemnotificationplugininterface/1.0")

#endif // DUIHOMESYSTEMNOTIFICATIONPLUGININTERFACE_H
