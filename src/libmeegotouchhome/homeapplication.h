/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of mhome.
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

#ifndef HOMEAPPLICATION_H_
#define HOMEAPPLICATION_H_

#include <MApplication>
#include <QTimer>
#include <QSet>
#include <QVariant>

class HomeScreenService;
class XEventListener;

/*!
 * HomeApplication extends MApplication with additional services.
 */
class HomeApplication : public MApplication
{
    Q_OBJECT

public:
    /*!
     * Constructs an application object.
     *
     * \param argc number of arguments passed to the application from the command line
     * \param argv argument strings passed to the application from the command line
     * \param appIdentifier An optional identifier for the application
     */
    HomeApplication(int &argc, char **argv,const QString& appIdentifier = QString());

    /*!
     * Destroys the HomeApplication.
     */
    virtual ~HomeApplication();

    /*!
     * Adds an X event listener object for this application. When X events arrive,
     * they are forwarded to the listener.
     * Before destroying the event listener object, remove the listener from the application
     * by calling \c removeXEventListener.
     * \param listener the X event listener
     */
    void addXEventListener(XEventListener *listener);

    /*!
     * Removes the X11 event listener object. The listener won't receive anymore events
     * from this application.
     * \param listener the listener object to remove.
     */
    void removeXEventListener(XEventListener *listener);

    /*!
     * Returns the locked orientation as set using the command line
     * arguments. The orientation is returned as a QVariant. If the
     * variant is not valid, the orientation locking has not been set
     * through the command line arguments. Otherwise the variant
     * contains a string representing the orientation to which the
     * UI should be locked to: either an empty string (meaning no
     * locking), portrait or landscape.
     *
     * \return a QVariant representing the locked orientation: invalid (use default), an empty string (unlocked), portrait or landscape
     */
    QVariant lockedOrientation() const;

signals:
    /*!
     * \brief A Signal to request launcher focus on specific launcher application
     */
     void focusToLauncherAppRequested(const QString &fileEntryPath);

#ifdef BENCHMARKS_ON
    void startBenchmarking();
    void stopBenchmarking();
#endif

protected:
    //! \reimp
    virtual bool x11EventFilter(XEvent *event);
    //! \reimp_end

private slots:
    /*!
     * Sends a dbus-signal after UI is visible, stops the process if it has
     * been started by upstart
     */
    void sendStartupNotifications();

private:
    /*!
     * Parses the command line parameters and sets upstart mode and forced
     * orientation locking based on the arguments.
     *
     * \param argc number of parameters
     * \param argv parameters
     */
    void parseArguments(int argc, char *argv[]);

    //! Flag that indicates whether the process was started by upstart or not
    bool upstartMode;

    //! A QVariant representing the locked orientation: invalid (use default), an empty string (unlocked), portrait or landscape
    QVariant lockedOrientation_;

    //! Timer used for sending startup notifications
    QTimer startupNotificationTimer;

    //! Implementations for com.meego.core.HomeScreen interface.
    HomeScreenService *homeScreenService;

    /*!
     * The X event listener objects registered for receiving X events.
     */
    QList<XEventListener*> xEventListeners;

    //! A flag that tells if there is an active iterator going through the X event listener container
    bool iteratorActiveForEventListenerContainer;

    //! Listener objects that are to be removed from listening to X events.
    //! Once a listener is on this list, it won't receive any X events any more.
    QList<XEventListener*> toBeRemovedEventListeners;

#ifdef UNIT_TEST
    friend class Ut_HomeApplication;
#endif
};

#endif /* HOMEAPPLICATION_H_ */
