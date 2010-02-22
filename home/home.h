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

#ifndef HOME_H
#define HOME_H

#include <QTimeLine>
#include <DuiWidget>
#include <DuiDeviceProfile>
#include <DuiSceneWindow>

class QGraphicsView;
class Desktop;
class DuiPannableViewport;
class DuiButton;
class Launcher;
class StatusAreaWindow;
class NotificationArea;
class DuiModalSceneWindow;
class DuiOverlay;

/*!
 * A class for capturing the mouse events that occur on top of some widget, used for
 * closing the notification area when desktop is clicked
 */
class MouseEventCaptureWidget : public DuiWidget
{
    Q_OBJECT

public:
    /*!
     * Constructs a MouseEventCaptureWidget
     * \param parent the widget which defines the area where the event capture is performed
     */
    MouseEventCaptureWidget(QGraphicsItem *parent = 0);

    /*!
     * Destroys the widget
     */
    virtual ~MouseEventCaptureWidget();

protected:
    //! \reimp
    virtual QRectF boundingRect() const;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    //! \reimp_end

signals:
    /*!
     * Emitted whenever a mouse event happens in the area defined by this widget's parent
     * \param event the mouse event
     */
    void eventReceived(QGraphicsSceneMouseEvent *event);
};

/*!
 * The main widget of the Home application. Acts as container for the desktop
 * and the notification area.
 */
class Home : public DuiSceneWindow
{
    Q_OBJECT

public:
    /*!
     * Constructs a Home widget.
     *
     * \param parent the parent widget of the Home, defaults to NULL
     */
    Home(QGraphicsItem *parent = NULL);

    /*!
     * Destroys the Home.
     */
    virtual ~Home();

    //! \reimp
    virtual void setGeometry(const QRectF &rect);
    virtual QRectF boundingRect() const;
    //! \reimp_end

    //! Sets the visibility status of the notification area
    void setNotificationAreaVisible(bool visible);

public slots:
    //! Opens or closes the notification area
    void toggleNotificationArea();

    //! Receives information about changes in the notification count
    void notificationCountChanged(int count);

private slots:
    /*!
     * \brief A slot for receiving mouse events when notification area is open
     */
    void mouseEventReceived();

    /*!
     * \brief A slot for animating the opening/closing of the notification area
     */
    void notificationAreaTimeLineValueChanged(qreal value);

    /*!
     * \brief A slot for setting the correct state after notification area has finished animating
     */
    void notificationAreaTimeLineFinished();

private:
    //! The states in which the notification area can be
    enum NotificationAreaState { Opening, Closing, Open, Closed };

    //! The current notification area state
    NotificationAreaState notificationAreaState;

    //! Pans the notification area to the bottom so that most recent notifications are visible
    void panNotificationAreaToBottom();

    //! Updates the geometry of the notification area
    void updateNotificationAreaGeometry();

    //! The desktop
    Desktop *desktop;

    //! The status area window
    StatusAreaWindow *statusAreaWindow;

    //! An overlay to contain the notification area viewport
    DuiOverlay *notificationAreaViewportOverlay;

    //! Pannable viewport in which the notification area is displayed
    DuiPannableViewport *notificationAreaViewport;

    //! The notification area widget
    NotificationArea *notificationArea;

    //! Notification area visibility status
    bool notificationAreaVisible;

    //! An mouse event capture widget for closing notification area
    MouseEventCaptureWidget *eventCaptureWidget;

    //! The height of the status area
    qreal statusAreaHeight;

    //! A timeline for animating the opening/closing of notification area
    QTimeLine notificationAreaTimeLine;

    //! Source value for animating the notification area
    qreal desktopSourceY;

    //! Destination value for animating the notification area
    qreal desktopDestY;

    //! The previous size of the page
    QSizeF previousSize;

#ifdef UNIT_TEST
    friend class Ut_Home;
#endif
};

#endif // HOME_H
