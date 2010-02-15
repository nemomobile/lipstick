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

#ifndef DUICOMPOSITORNOTIFICATIONSINK_H_
#define DUICOMPOSITORNOTIFICATIONSINK_H_

#include <QHash>
#include "widgetnotificationsink.h"
#include <X11/Xlib.h>

class QGraphicsView;
class QTimer;
class DuiInfoBanner;

/*!
 * DuiCompositorNotificationSink implements the NotificationSink interface for
 * displaying notifications on top of other applications than duihome.
 *
 * Notification is displayed for a certain time after which it is hidden.
 */
class DuiCompositorNotificationSink : public WidgetNotificationSink
{
    Q_OBJECT

public:
    /*!
     * Constructs a new DuiCompositorNotificationSink.
     */
    DuiCompositorNotificationSink();

    /*!
     * Destroys the DuiCompositorNotificationSink.
     */
    virtual ~DuiCompositorNotificationSink();

private slots:
    //! \reimp
    virtual void addNotification(const Notification &notification);
    virtual void removeNotification(uint notificationId);
    //! \reimp_end

    /*!
     * \brief Rotates the info banner to a new orientation
     *
     * \param orientation the new orientation
     */
    void rotateInfoBanners(const Dui::Orientation &orientation);

    /*!
     * A slot for timing out the notification windows
     */
    void timeout();

private:
    /*!
     * Checks whether the desktop window is on top.
     *
     * \return true if the desktop window is the topmost window, false otherwise
     */
    bool isDesktopWindowOnTop();

    /*!
     * Updates an existing info banner with the given notification parameters.
     *
     * \param notification the notification to be updated
     */
    void updateNotification(const Notification &notification);

    /*!
     * Destroys the notification objects and optionally marks the notification ID to be still in use.
     *
     * \param notificationId the ID of the notification to be removed
     * \param notificationIdInUse true if the notification ID should be marked to be still in use, false otherwise
     */
    void notificationDone(uint notificationId, bool notificationIdInUse);

    /*!
     * Sets the size and rotation of a view based on the size of an info
     * banner and the current orientation.
     *
     * \param view the view to manipulate
     * \param infoBanner the infoBanner from which to take the size
     */
    static void setViewSizeAndRotation(QGraphicsView &view, const DuiInfoBanner &infoBanner);

    /*!
     * A private class for storing notification information
     */
    class DuiCompositorNotificationSinkNotification
    {
    public:
        DuiCompositorNotificationSinkNotification(QGraphicsView *view, QTimer *timer, DuiInfoBanner *infoBanner);
        ~DuiCompositorNotificationSinkNotification();

        //! The view in which the widget resides
        QGraphicsView *view;
        //! A timer for dismissing the notification
        QTimer *timer;
        //! The DuiInfoBanner
        DuiInfoBanner *infoBanner;
    };

    //! A mapping between notification IDs and private notification information classes
    QHash<uint, DuiCompositorNotificationSinkNotification *> idToNotification;

    // X11 Atoms for different window types
    Atom windowTypeAtom;
    Atom windowTypeDesktopAtom;
    Atom clientListStackingAtom;

    //! Whether the orientation change signal has been connected
    bool orientationChangeSignalConnected;
};

#endif /* DUICOMPOSITORNOTIFICATIONSINK_H_ */
