/***************************************************************************
**
** Copyright (C) 2012 Jolla Ltd.
** Contact: Robin Burchell <robin.burchell@jollamobile.com>
**
** This file is part of lipstick.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#ifndef NOTIFICATIONMANAGER_H
#define NOTIFICATIONMANAGER_H

#include "lipstickglobal.h"
#include "notification.h"
#include <QObject>

/*!
 * The notification manager allows applications to display notifications to the user.
 */
class LIPSTICK_EXPORT NotificationManager : public QObject
{
    Q_OBJECT

public:
    //! Notifation closing reasons used in the NotificationClosed signal
    enum NotificationClosedReason {
        //! The notification expired.
        NotificationExpired = 1,
        //! The notification was dismissed by the user.
        NotificationDismissedByUser,
        //! The notification was closed by a call to CloseNotification.
        CloseNotificationCalled
    };

    /*!
     * Returns a singleton instance of the notification manager.
     *
     * \return an instance of the notification manager
     */
    static NotificationManager *instance();

    /*!
     * Returns an array of strings. Each string describes an optional capability
     * implemented by the server. Refer to the Desktop Notification Specifications for
     * the defined capabilities.
     *
     * \return an array of strings, each string describing an optional capability implemented by the server
     */
    QStringList GetCapabilities();

    /*!
     * Sends a notification to the notification server.
     *
     * \param appName The optional name of the application sending the notification. Can be blank.
     * \param replacesId The optional notification ID that this notification replaces. The server must atomically (ie with no flicker or other visual cues) replace the given notification with this one. This allows clients to effectively modify the notification while it's active. A value of value of 0 means that this notification won't replace any existing notifications.
     * \param appIcon The optional program icon of the calling application. Can be an empty string, indicating no icon.
     * \param summary The summary text briefly describing the notification.
     * \param body The optional detailed body text. Can be empty.
     * \param actions Actions are sent over as a list of pairs. Each even element in the list (starting at index 0) represents the identifier for the action. Each odd element in the list is the localized string that will be displayed to the user.
     * \param hints Optional hints that can be passed to the server from the client program. Although clients and servers should never assume each other supports any specific hints, they can be used to pass along information, such as the process PID or window ID, that the server may be able to make use of. Can be empty.
     * \param expireTimeout he timeout time in milliseconds since the display of the notification at which the notification should automatically close.  If -1, the notification's expiration time is dependent on the notification server's settings, and may vary for the type of notification. If 0, never expire.
     */
    uint Notify(const QString &appName, uint replacesId, const QString &appIcon, const QString &summary, const QString &body, const QStringList &actions, NotificationHints hints, int expireTimeout);

    /*!
     * Causes a notification to be forcefully closed and removed from the user's view.
     * It can be used, for example, in the event that what the notification pertains
     * to is no longer relevant, or to cancel a notification with no expiration time.
     * The NotificationClosed signal is emitted by this method.
     *
     * \param id the ID of the notification to be closed
     */
    void CloseNotification(uint id);

    /*!
     * This message returns the information on the server. Specifically, the server name, vendor,
     * and version number.
     *
     * \param name The product name of the server.
     * \param vendor The vendor name. For example, "KDE," "GNOME," "freedesktop.org," or "Microsoft."
     * \param version The server's version number.
     * \return an empty string
     */
    QString GetServerInformation(QString &name, QString &vendor, QString &version);

signals:
    /*!
     * A completed notification is one that has timed out, or has been dismissed by the user.
     *
     * \param id The ID of the notification that was closed.
     * \param reason The reason the notification was closed. 1 - The notification expired. 2 - The notification was dismissed by the user. 3 - The notification was closed by a call to CloseNotification. 4 - Undefined/reserved reasons.
     */
    void NotificationClosed(uint id, uint reason);

    /*!
     * This signal is emitted when one of the following occurs:
     *   * The user performs some global "invoking" action upon a notification. For instance, clicking somewhere on the notification itself.
     *   * The user invokes a specific action as specified in the original Notify request. For example, clicking on an action button.
     *
     * \param id The ID of the notification emitting the ActionInvoked signal.
     * \param actionKey The key of the action invoked. These match the keys sent over in the list of actions.
     */
    void ActionInvoked(uint id, const QString &actionKey);

    /*!
     * Emitted when a notification is modified (added or updated).
     *
     * \param id the ID of the modified notification
     */
    void notificationModified(uint id);

    /*!
     * Emitted when a notification is removed.
     *
     * \param id the ID of the removed notification
     */
    void notificationRemoved(uint id);

private:
    /*!
     * Creates a new NotificationManager.
     *
     * \param parent the parent object
     */
    explicit NotificationManager(QObject *parent = 0);

    /*!
     * Returns the next available notification ID
     *
     * \return The next available notification ID
     */
    uint nextAvailableNotificationID();

    //! The singleton notification manager instance
    static NotificationManager *instance_;

    //! Hash of all notifications keyed by notification IDs
    QHash<uint, Notification> notifications;

    //! Previous notification ID used
    uint previousNotificationID;
};

#endif // NOTIFICATIONMANAGER_H
