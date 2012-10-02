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

#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include "lipstickglobal.h"
#include "notificationhints.h"
#include <QStringList>
#include <QDateTime>

/*!
 * An object for storing information about a single notification.
 */
class LIPSTICK_EXPORT Notification : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString appName READ appName)
    Q_PROPERTY(QString appIcon READ appIcon)
    Q_PROPERTY(QString summary READ summary NOTIFY summaryChanged)
    Q_PROPERTY(QString body READ body NOTIFY bodyChanged)
    Q_PROPERTY(QStringList actions READ actions)
    Q_PROPERTY(int expireTimeout READ expireTimeout)
    Q_PROPERTY(QString icon READ icon NOTIFY iconChanged)
    Q_PROPERTY(QDateTime timestamp READ timestamp)
    Q_PROPERTY(QString localizedTimestamp READ localizedTimestamp NOTIFY localizedTimestampChanged)

public:
    /*!
     * Creates an object for storing information about a single notification.
     *
     * \param appName name of the application sending the notification
     * \param appIcon icon ID of the application sending the notification
     * \param summary summary text for the notification
     * \param body body text for the notification
     * \param actions actions for the notification as a list of identifier/string pairs
     * \param hints hints for the notification
     * \param expireTimeout expiration timeout for the notification
     * \param parent the parent QObject
     */
    Notification(const QString &appName, const QString &appIcon, const QString &summary, const QString &body, const QStringList &actions, const NotificationHints &hints, int expireTimeout, QObject *parent = 0);

    //! Returns the name of the application sending the notification
    QString appName() const;

    //! Sets the name of the application sending the notification
    void setAppName(const QString &appName);

    //! Returns the icon ID of the application sending the notification
    QString appIcon() const;

    //! Sets the icon ID of the application sending the notification
    void setAppIcon(const QString &appIcon);

    //! Returns the summary text for the notification
    QString summary() const;

    //! Sets the summary text for the notification
    void setSummary(const QString &summary);

    //! Returns the body text for the notification
    QString body() const;

    //! Sets the body text for the notification
    void setBody(const QString &body);

    //! Returns the actions for the notification as a list of identifier/string pairs
    QStringList actions() const;

    //! Sets the actions for the notification as a list of identifier/string pairs
    void setActions(const QStringList &actions);

    //! Returns the hints for the notification
    NotificationHints hints() const;

    //! Sets the hints for the notification
    void setHints(const NotificationHints &hints);

    //! Returns the expiration timeout for the notification
    int expireTimeout() const;

    //! Sets the expiration timeout for the notification
    void setExpireTimeout(int expireTimeout);

    //! Returns the icon ID for the notification
    QString icon() const;

    //! Returns the timestamp for the notification
    QDateTime timestamp() const;

    //! Returns the timestamp for the notification in localized text format
    QString localizedTimestamp() const;

signals:
    /*!
     * Sent when an action defined for the notification is invoked.
     *
     * \param action the action that was invoked
     */
    void actionInvoked(QString action);

    //! Sent when the summary has been modified
    void summaryChanged();

    //! Sent when the body has been modified
    void bodyChanged();

    //! Sent when the icon has been modified
    void iconChanged();

    //! Sent when the localized timestamp has changed
    void localizedTimestampChanged();

private:
    //! Name of the application sending the notification
    QString appName_;

    //! Icon ID of the application sending the notification
    QString appIcon_;

    //! Summary text for the notification
    QString summary_;

    //! Body text for the notification
    QString body_;

    //! Actions for the notification as a list of identifier/string pairs
    QStringList actions_;

    //! Hints for the notification
    NotificationHints hints_;

    //! Expiration timeout for the notification
    int expireTimeout_;
};

#endif // NOTIFICATION_H
