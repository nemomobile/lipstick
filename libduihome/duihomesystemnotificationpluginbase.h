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

#ifndef DUIHOMESYSTEMNOTIFICATIONPLUGINBASE_H
#define DUIHOMESYSTEMNOTIFICATIONPLUGINBASE_H

#include <QObject>
#include "duihomesystemnotificationsourceinterface.h"
#include "duihomesystemnotificationplugininterface.h"

/*!
* This class is derived by a system notification plugin. Plugin is free to interact with the application/service which owns the notification.
* When the plugin is ready to send a notification, it should call addNotification with suitable notification parameters. Derived plugin is only
* supposed to call addNotification from this class.
 */

class DuiHomeSystemNotificationPluginBase : public QObject, public DuiHomeSystemNotificationPluginInterface
{
    Q_OBJECT;
    Q_INTERFACES(DuiHomeSystemNotificationPluginInterface)
public:
    /*! Constructs a new  DuiSystemNotificationPluginBase
     */
    DuiHomeSystemNotificationPluginBase();

    //! \reimp
    void initialize(DuiHomeSystemNotificationSourceInterface &sourceInterface);
    //! \reimp_end

    /*!
     * Destroys the plugin.
     */
    virtual ~DuiHomeSystemNotificationPluginBase() {}

    /*!
     * Returns the source which is tied to this plugin
     */
    DuiHomeSystemNotificationSourceInterface *source();

protected:
    /*!
     * The plugin should call this method when it wants to send a system notification
     * \param eventType the event type of the notification
     * \param summary the summary text to be used in the notification
     * \param body the body text to be used in the notification
     * \param action the ID of the content to be used in the notification
     * \param imageURI the ID of the icon to be used in the notification
     * \param count the number of items inside this notification
     * \return Notification id for the new notification created.
     */
    uint addNotification(const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count);

private:
    //! Handle to the notification source which loaded the plugin
    DuiHomeSystemNotificationSourceInterface *source_;
    //! Notification User id obtained from the manager.
    uint notificationUserId;
};

#endif // DUIHOMESYSTEMNOTIFICATIONPLUGINBASE_H
