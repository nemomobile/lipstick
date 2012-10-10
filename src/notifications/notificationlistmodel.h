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

#ifndef NOTIFICATIONLISTMODEL_H
#define NOTIFICATIONLISTMODEL_H

#include "utilities/qobjectlistmodel.h"
#include "lipstickglobal.h"

class Notification;

class LIPSTICK_EXPORT NotificationListModel : public QObjectListModel
{
    Q_OBJECT

public:
    explicit NotificationListModel(QObject *parent = 0);
    virtual ~NotificationListModel();

private slots:
    void updateNotification(uint id);
    void removeNotification(uint id);

private:
    /*!
     * Checks whether the given notification should be shown. A notification
     * should be shown when it's class is not system and it has a body and a
     * summary.
     *
     * \param notification the notification to check
     * \return \c true if the notification should be shown, \c false otherwise
     */
    static bool notificationShouldBeShown(Notification *notification);

    Q_DISABLE_COPY(NotificationListModel)

#ifdef UNIT_TEST
    friend class Ut_NotificationListModel;
#endif
};

#endif // NOTIFICATIONLISTMODEL_H
