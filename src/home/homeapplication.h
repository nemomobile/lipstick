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

class HomeScreenService;

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
     */
    HomeApplication(int &argc, char **argv);

    /*!
     * Destroys the HomeApplication.
     */
    ~HomeApplication();

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
     * Sends a dbus-signal after UI is visible, stops the process if
     * duihome has been started by upstart
     */
    void sendStartupNotifications();

private:
    //! Flag that indicates whether duihome was started by upstart or not
    bool upstartMode;

    //! Timer used for sending startup notifications
    QTimer startupNotificationTimer;

    //! Implementations for com.meego.core.HomeScreen interface.
    HomeScreenService *homeScreenService;

#ifdef UNIT_TEST
    friend class Ut_HomeApplication;
#endif
};

#endif /* HOMEAPPLICATION_H_ */
