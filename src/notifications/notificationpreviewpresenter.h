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

#ifndef NOTIFICATIONPREVIEWPRESENTER_H
#define NOTIFICATIONPREVIEWPRESENTER_H

#include "lipstickglobal.h"
#include <QObject>

class QDeclarativeView;
class Notification;

namespace MeeGo {
class QmLocks;
class QmDisplayState;
}

/*!
 * \class NotificationPreviewPresenter
 *
 * \brief Presents notification previews one at a time.
 *
 * Creates a transparent notification window which can be used to show
 * notification previews.
 */
class LIPSTICK_EXPORT NotificationPreviewPresenter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Notification *notification READ notification NOTIFY notificationChanged)

public:
    /*!
     * Creates a notification preview presenter.
     *
     * \param parent the parent object
     */
    explicit NotificationPreviewPresenter(QObject *parent = 0);

    /*!
     * Destroys the notification preview presenter.
     */
    virtual ~NotificationPreviewPresenter();

    /*!
     * Sets the notification window's shape region according to the given
     * coordinates. The coordinates should be set to match the coordinates of
     * the notification preview in the window. This allows input events to be
     * caught only on the notification preview area and passed through to the
     * underneath window otherwise.
     *
     * \param x1 the X coordinate of the top left edge of the notification preview
     * \param y1 the Y coordinate of the top left edge of the notification preview
     * \param x2 the X coordinate of the bottom right edge of the notification preview
     * \param y2 the Y coordinate of the bottom right edge of the notification preview
     */
    Q_INVOKABLE void setNotificationPreviewRect(qreal x1, qreal y1, qreal x2, qreal y2);
    
    /*!
     * Returns the notification to be currently shown or 0 if no notification
     * should be shown.
     *
     * \return the notification to be currently shown or 0 if no notification should be shown
     */
    Notification *notification() const;

signals:
    //! Sent when the notification to be shown has changed.
    void notificationChanged();

public slots:
    /*!
     * Shows the next notification to be shown, if any. If the notification
     * window is not yet visible, shows the window. If there is no
     * notification to be shown but the window is visible, hides the window.
     */
    void showNextNotification();

private slots:
    /*!
     * Updates the notification with the given ID.
     *
     * \param id the ID of the notification to be updated
     */
    void updateNotification(uint id);

    /*!
     * Removes the notification with the given ID.
     *
     * \param id the ID of the notification to be removed
     */
    void removeNotification(uint id, bool onlyFromQueue = false);

private:
    //! Creates the notification window if it has not been created yet.
    void createWindowIfNecessary();

    //! Checks whether the given notification has a preview body and a preview summary.
    bool notificationShouldBeShown(Notification *notification);

    //! Sets the given notification as the current notification
    void setCurrentNotification(Notification *notification);

    //! The notification window
    QDeclarativeView *window;

    //! Notifications to be shown
    QList<Notification *> notificationQueue;

    //! Notification currently being shown
    Notification *currentNotification;

    //! For getting information about the touch screen lock state
    MeeGo::QmLocks *locks;

    //! For getting information about the display state
    MeeGo::QmDisplayState *displayState;

#ifdef UNIT_TEST
    friend class Ut_NotificationPreviewPresenter;
#endif
};

#endif // NOTIFICATIONPREVIEWPRESENTER_H
