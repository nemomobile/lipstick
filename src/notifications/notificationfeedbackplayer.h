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

#ifndef NOTIFICATIONFEEDBACKPLAYER_H
#define NOTIFICATIONFEEDBACKPLAYER_H

#include <QHash>

class Notification;
namespace Ngf {
    class Client;
}

/*!
 * \class NotificationFeedbackPlayer
 *
 * \brief Plays non-graphical feedback for notifications.
 */
class NotificationFeedbackPlayer : public QObject
{
    Q_OBJECT

public:
    explicit NotificationFeedbackPlayer(QObject *parent = 0);
    
private slots:
    /*!
     * Adds the notification with the given ID.
     *
     * \param id the ID of the notification to be added
     */
    void addNotification(uint id);

    /*!
     * Removes the notification with the given ID.
     *
     * \param id the ID of the notification to be removed
     */
    void removeNotification(uint id);

private:
    //! Non-graphical feedback player
    Ngf::Client *ngfClient;

    //! A mapping between notification IDs and NGF play IDs.
    QHash<Notification *, uint> idToEventId;

#ifdef UNIT_TEST
    friend class Ut_NotificationFeedbackPlayer;
#endif
};

#endif // NOTIFICATIONFEEDBACKPLAYER_H
