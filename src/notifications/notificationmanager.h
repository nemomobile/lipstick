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
#include <QTimer>
#include <QSet>

class CategoryDefinitionStore;
class QSqlDatabase;

/*!
 * \class NotificationManager
 *
 * \brief The notification manager allows applications to display notifications to the user.
 *
 * The notification manager implements a desktop notifications service based
 * on the <a href="http://www.galago-project.org/specs/notification/0.9/">Desktop Notifications Specification</a>.
 * The service is registered as org.freedesktop.Notifications on the D-Bus
 * session bus in the path /org/freedesktop/Notifications.
 *
 * \section specifics Notes specific to the behavior of this particular implementation
 *
 * \subsection differences Differences from the specification
 *
 *   - urgency level 2 (critical) will not cause the notification not to expire
 *     as described in the specification, but will cause the notification to be
 *     handled as a system notification
 *
 * \subsection capabilities Capabilities
 *
 *   - The service implements the \c "body" capability which allows notifications
 *     to contain body text. See the body parameter for Notify().
 *   - The service implements the \c "actions" capability which allows actions to
 *     be associated with notifications. When an action is executed, the
 *     \c ActionInvoked() signal is sent. However, this requires the application
 *     related to the notification to be running in order to receive the signal.
 *     For this reason the service also implements a \c "x-nemo-remote-actions"
 *     capability, which allows remote actions to be associated with
 *     notifications. When such an action is executed, the notification manager
 *     makes the D-Bus call defined for the action. See the
 *     \c x-nemo-remote-action-actionname hint.
 *
 * \subsection hints Hints
 *
 *   - The \c "category" hint is used to load a definition for notifications in
 *     that category from
 *     \c /usr/share/lipstick/notificationcategories/categoryname.conf. This allows
 *     defining common properties for all notifications in each category.
 *        - Each category definition file contains a list of \c hint=value pairs,
 *          one per line.
 *        - Each \c hint=value pair in the category definition file is added to
 *          the hints of the notification.
 *   - The service supports the following Nemo specific hints:
 *       - \c x-nemo-icon: icon ID or path for the notification. If the string begins with a / it's interpreted to be an absolute path. Otherwise it's interpreted to be an icon ID, in which case the icon with the given ID is fetched from the theme.
 *       - \c x-nemo-item-count: the number of items represented by this notification. For example, a single notification can represent four missed calls by setting the count to 4.
 *       - \c x-nemo-timestamp: the timestamp for the notification. Should be set to the time when the event the notification is related to has occurred, not when the notification itself was sent.
 *       - \c x-nemo-preview-icon: icon ID to use in the preview banner for the notification, if any.
 *       - \c x-nemo-preview-body: body text to be shown in the preview banner for the notification, if any.
 *       - \c x-nemo-preview-summary: summary text to be shown in the preview banner for the notification, if any.
 *       - \c x-nemo-user-removable: a boolean value for defining whether the user should be able to remove the notification by tapping on it or should it be only programmatically removable. Defaults to true.
 *       - \c x-nemo-remote-action-actionname: details of the D-Bus call to be made when the action "actionname" is executed. Should be in MRemoteAction's "serviceName objectPath interface methodName" format.
 *
 * \section howto How to use notifications in various use cases
 *
 * \subsection chat Incoming chat messages
 *
 * A typical scenario for using notifications would be to let the user know
 * when an incoming chat message has been received. Since the user probably
 * does not want a separate notification about each received message to
 * clutter up the notification area but still does want some kind of a
 * notification about each message, notification grouping is required.
 *
 * \subsubsection first_chat_message Sending the notification for the first incoming chat message
 *
 * When calling Notify() to a display a notification related to the first chat message, the parameters should be set as follows:
 *   - \c app_name should be a string identifying the sender application, such as the name of its binary, for example. "chat"
 *   - \c replaces_id should be 0 since the notification is a new one and not related to any existing notification
 *   - \c app_icon should be "icon-lock chat" to use the icon with that ID on the notification area. Can be left empty; icons can be handled completely with notification hints (see below)
 *   - \c summary should contain a brief description about the notification to be shown on the notification area, like "John Doe"
 *   - \c body should contain informative text related to the notification to be shown on the notification area, like "Hi!"
 *   - \c actions can be empty since separate remote actions allow the chat application to respond to the notification even if it's not already running
 *   - \c hints should contain the following:
 *       - \c category should be "im.received" to categorize the notification to be related to an instant message
 *       - \c urgency should be 1 (normal) since chat messages are not specifically low or high priority
 *       - \c x-nemo-icon should be "icon-lock-chat" to define that the icon with that ID is to be shown on the notification area
 *       - \c x-nemo-preview-icon should be "icon-preview-chat" to define that the icon with that ID is to be shown on the preview banner
 *       - \c x-nemo-preview-summary should match the summary text ("John Doe") in order to show it also on the preview banner
 *       - \c x-nemo-preview-body should match the body text ("Hi!") in order to show it also on the preview banner
 *       - \c x-nemo-timestamp should be set to the time when the chat message was sent (or received, depending on the intended application logic)
 *       - \c x-nemo-remote-action-default should be set to "org.example.chat / org.example.chat showMessage" which will cause a D-Bus call with the given details to be made when the notification is tapped
 *   - \c expire_timeout should be -1 to let the notification manager choose an appropriate expiration time
 *
 * The Notify() call will return a notification ID which should be stored by
 * the application in order to be able to update the notification when more
 * related chat messages come in.
 *
 * \subsubsection second_chat_message Updating the notification for the second incoming chat message
 *
 * When calling Notify() to a display a notification related to the second chat message, the parameters should be set as follows:
 *   - \c app_name should be a string identifying the sender application, such as the name of its binary, for example. "chat"
 *   - \c replaces_id should be the notification ID returned by the first Notify() call in order to update the existing notification
 *   - \c app_icon should be "icon-lock chat" to use the icon with that ID on the notification area. Can be left empty; icons can be handled completely with notification hints (see below)
 *   - \c summary should contain a brief description about the notification to be shown on the notification area, like "John Doe"
 *   - \c body should contain informative text related to the notification to be shown on the notification area, like "2 messages"
 *   - \c actions can be empty since separate remote actions allow the chat application to respond to the notification even if it's not already running
 *   - \c hints should contain the following:
 *       - \c category should be "im.received" to categorize the notification to be related to an instant message
 *       - \c urgency should be 1 (normal) since chat messages are not specifically low or high priority
 *       - \c x-nemo-icon should be "icon-lock-chat" to define that the icon with that ID is to be shown on the notification area
 *       - \c x-nemo-item-count should be 2 to make the notification represent two chat messages
 *       - \c x-nemo-preview-icon should be "icon-preview-chat" to define that the icon with that ID is to be shown on the preview banner
 *       - \c x-nemo-preview-summary should contain a brief description about the notification to be shown on the preview banner, like "John Doe"
 *       - \c x-nemo-preview-body should contain informative text about the notification to be shown on the preview banner, like "Are you there?"
 *       - \c x-nemo-timestamp should be set to the time when the latest chat message was sent (or received, depending on the intended application logic)
 *       - \c x-nemo-remote-action-default should be set to "org.example.chat / org.example.chat showMessage" which will cause a D-Bus call with the given details to be made when the notification is tapped
 *   - \c expire_timeout should be -1 to let the notification manager choose an appropriate expiration time
 *
 * Notice that the summary/body and the preview summary/body now differ in
 * order to show different information on the notification area and in the
 * preview banner. Also the item count should be set when the notification
 * represents multiple content items.
 *
 * \subsection system System notifications
 *
 * System notifications are similar to other kinds of notifications but since
 * they convey information relevant only when the notification is sent they
 * should not appear on the notification area. To achieve this only
 * \c x-nemo-preview-summary should be set; the body and the summary should be
 * left empty. On the other hand the preview banner should appear on top of
 * all applications even in situations where application related notifications
 * are disabled (while recording video, for instance). This is achieved with
 * the critical urgency level, 2.
 *
 * When calling Notify() to a display a system notification, the parameters should be set as follows:
 *   - \c app_name should be a string identifying the sender application, such as the name of its binary, for example. "batterynotifier"
 *   - \c replaces_id should be 0 since the notification is a new one and not related to any existing notification
 *   - \c app_icon should be left empty; icons are handled with notification hints (see below)
 *   - \c summary should be left empty for nothing to be shown on the notification area
 *   - \c body should be left empty for nothing to be shown on the notification area
 *   - \c actions should be left empty
 *   - \c hints should contain the following:
 *       - \c category should be "device" to categorize the notification to be related to the device
 *       - \c urgency should be 2 (critical) to mark the notification as a system notification
 *       - \c x-nemo-preview-icon should be "icon-m-energy-management-charging8" to define that the icon with that ID is to be shown on the preview banner
 *       - \c x-nemo-preview-summary should be ("Charging") in order to show it on the preview banner
 *   - \c expire_timeout should be -1 to let the notification manager choose an appropriate expiration time
 *
 * \subsection categorydefinitions Using category definition files
 *
 * When notifications in a certain category always share the same hints it's
 * possible to write a category definition file in
 * \c /usr/share/lipstick/notificationcategories/categoryname.conf and then just
 * set the category hint to categoryname when calling Notify(). The category
 * definition file contains a list of hint=value pairs, one per line. Each
 * \c hint=value pair in the category definition file is added to the hints of
 * the notification.
 *
 * For example, if /usr/share/lipstick/notificationcategories/device.conf
 * contains
 *
 * \code
 * urgency=2
 * x-nemo-preview-icon=icon-m-energy-management-charging8
 * \endcode
 *
 * and Notify() is called with the hints dictionary containing the value \c "device"
 * for the \c "category" hint and the value \c "Charging" for the \c "x-nemo-preview-summary"
 * hint, the hints will be combined so that the effective hints used are
 *
 * \code
 * category=device
 * urgency=2
 * x-nemo-preview-summary=Charging
 * x-nemo-preview-icon=icon-m-energy-management-charging8
 * \endcode
 */
class LIPSTICK_EXPORT NotificationManager : public QObject
{
    Q_OBJECT

public:
    //! Standard hint: The urgency level.
    static const char *HINT_URGENCY;

    //! Standard hint: The type of notification this is.
    static const char *HINT_CATEGORY;

    //! Standard hint: This specifies the name of the desktop filename representing the calling program. This should be the same as the prefix used for the application's .desktop file. An example would be "rhythmbox" from "rhythmbox.desktop". This can be used by the daemon to retrieve the correct icon for the application, for logging purposes, etc. Not supported by this implementation.
    static const char *HINT_DESKTOP_ENTRY;

    //! Standard hint: This is a raw data image format which describes the width, height, rowstride, has alpha, bits per sample, channels and image data respectively. We use this value if the icon field is left blank. Not supported by this implementation.
    static const char *HINT_IMAGE_DATA;

    //! Standard hint: The path to a sound file to play when the notification pops up. Not supported by this implementation.
    static const char *HINT_SOUND_FILE;

    //! Standard hint: Causes the server to suppress playing any sounds, if it has that ability. This is usually set when the client itself is going to play its own sound. Not supported by this implementation.
    static const char *HINT_SUPPRESS_SOUND;

    //! Standard hint: Specifies the X location on the screen that the notification should point to. The "y" hint must also be specified. Not supported by this implementation.
    static const char *HINT_X;

    //! Standard hint: Specifies the Y location on the screen that the notification should point to. The "x" hint must also be specified. Not supported by this implementation.
    static const char *HINT_Y;

    //! Nemo hint: Icon of the notification. Allows the icon to be set using a category definition file without specifying it in the Notify() call.
    static const char *HINT_ICON;

    //! Nemo hint: Item count represented by the notification.
    static const char *HINT_ITEM_COUNT;

    //! Nemo hint: Timestamp of the notification.
    static const char *HINT_TIMESTAMP;

    //! Nemo hint: Icon of the preview of the notification.
    static const char *HINT_PREVIEW_ICON;

    //! Nemo hint: Body text of the preview of the notification.
    static const char *HINT_PREVIEW_BODY;

    //! Nemo hint: Summary text of the preview of the notification.
    static const char *HINT_PREVIEW_SUMMARY;

    //! Nemo hint: Remote action of the notification. Prefix only: the action identifier is to be appended.
    static const char *HINT_REMOTE_ACTION_PREFIX;

    //! Nemo hint: User removability of the notification.
    static const char *HINT_USER_REMOVABLE;

    //! Nemo hint: User closeability of the notification.
    static const char *HINT_USER_CLOSEABLE;

    //! Nemo hint: Feedback of the notification.
    static const char *HINT_FEEDBACK;

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
     * Returns a notification with the given ID.
     *
     * \param id the ID of the notification to return
     * \return the notification with the given ID
     */
    Notification *notification(uint id) const;

    /*!
     * Returns a list of notification IDs.
     *
     * \return a list of notification IDs.
     */
    QList<uint> notificationIds() const;

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
    uint Notify(const QString &appName, uint replacesId, const QString &appIcon, const QString &summary, const QString &body, const QStringList &actions, const QVariantHash &hints, int expireTimeout);

    /*!
     * Causes a notification to be forcefully closed and removed from the user's view.
     * It can be used, for example, in the event that what the notification pertains
     * to is no longer relevant, or to cancel a notification with no expiration time.
     * The NotificationClosed signal is emitted by this method.
     *
     * \param id the ID of the notification to be closed
     */
    void CloseNotification(uint id, NotificationClosedReason closeReason = CloseNotificationCalled);

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

    /*!
     * Returns the notifications sent by a specified application.
     *
     * \param appName the name of the application to get notifications for
     * \return a list of notifications for the application
     */
    QList<Notification> GetNotifications(const QString &appName);

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

private slots:
    /*!
     * Removes all notifications with the specified category.
     *
     * \param category the category of the notifications to remove
     */
    void removeNotificationsWithCategory(const QString &category);

    /*!
     * Update category data of all notifications with the
     * specified category.
     *
     * \param category the category of the notifications to update
     */
    void updateNotificationsWithCategory(const QString &category);

    /*!
     * Commits the current database transaction, if any.
     * Also destroys any removed notifications.
     */
    void commit();

    /*!
     * Invokes the given action if it is has been defined. The
     * sender is expected to be a Notification.
     *
     * \param action the action to be invoked
     */
    void invokeAction(const QString &action);

private:
    /*!
     * Creates a new notification manager.
     *
     * \param parent the parent object
     */
    explicit NotificationManager(QObject *parent = 0);

    //! Destroys the notification manager.
    virtual ~NotificationManager();

    /*!
     * Returns the next available notification ID
     *
     * \return The next available notification ID
     */
    uint nextAvailableNotificationID();

    /*!
     * Applies a category definition to a notification's hints by inserting
     * all key-value pairs in the category definition to the hints.
     *
     * \param hints the notification hints to apply the category definition to
     */
    void applyCategoryDefinition(QVariantHash &hints);

    /*!
     * Adds a timestamp to a notification's hints if there is no timestamp
     * defined.
     *
     * \param hints the notification hints to add the timestamp to
     */
    void addTimestamp(QVariantHash &hints);

    //! Restores the notifications from a database on the disk
    void restoreNotifications();

    /*!
     * Creates a connection to the Sqlite database.
     *
     * \return \c true if the connection was successfully established, \c false otherwise
     */
    bool connectToDatabase();

    /*!
     * Checks whether there is enough free disk space available.
     *
     * \param path any path to the file system from which the space should be checked
     * \param freeSpaceNeeded free space needed in kilobytes
     * \return \c true if there is enough free space in given file system, \c false otherwise
     */
    static bool checkForDiskSpace(const QString &path, unsigned long freeSpaceNeeded);

    /*!
     * Removes a database file from the filesystem. Removes related -wal and -shm files as well.
     *
     * \param path the path of the database file to be removed
     */
    static void removeDatabaseFile(const QString &path);

    /*!
     * Ensures that all database tables have the requires fields.
     * Recreates the tables if needed.
     *
     * \return \c true if the database can be used, \c false otherwise
     */
    bool checkTableValidity();

    /*!
     * Recreates a table in the database.
     *
     * \param tableName the name of the table to be created
     * \param definition SQL definition for the table
     * \return \c true if the table was created, \c false otherwise
     */
    bool recreateTable(const QString &tableName, const QString &definition);

    //! Fills the notifications hash table with data from the database
    void fetchData();

    /*!
     * Executes a SQL command in the database. Starts a new transaction if none is active currently, otherwise
     * the command goes to the active transaction. Restarts the transaction commit timer.
     * \param command the SQL command
     * \param args list of values to be bound to the positional placeholders ('?' -character) in the command.
     */
    void execSQL(const QString &command, const QVariantList &args = QVariantList());

    //! The singleton notification manager instance
    static NotificationManager *instance_;

    //! Hash of all notifications keyed by notification IDs
    QHash<uint, Notification*> notifications;

    //! Notifications waiting to be destroyed
    QSet<Notification *> removedNotifications;

    //! Previous notification ID used
    uint previousNotificationID;

    //! The category definition store
    CategoryDefinitionStore *categoryDefinitionStore;

    //! Database for the notifications
    QSqlDatabase *database;

    //! Whether the current database transaction has been committed to the database
    bool committed;

    //! Timer for triggering the commit of the current database transaction
    QTimer databaseCommitTimer;

#ifdef UNIT_TEST
    friend class Ut_NotificationManager;
#endif
};

#endif // NOTIFICATIONMANAGER_H
