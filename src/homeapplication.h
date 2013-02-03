/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include <signal.h>
#include <QApplication>
#include <QDeclarativeView>
#include "lipstickglobal.h"

class XEventListener;
class ScreenLock;
class VolumeControl;
class BatteryNotifier;
class USBModeSelector;
class ShutdownScreen;

/*!
 * Extends QApplication with features necessary to create a desktop.
 */
class LIPSTICK_EXPORT HomeApplication : public QApplication
{
    Q_OBJECT

    QDeclarativeView *_mainWindowInstance;
    QString _qmlPath;

public:
    /*!
     * Constructs an application object.
     *
     * \param argc number of arguments passed to the application from the command line
     * \param argv argument strings passed to the application from the command line
     * \param qmlPath The path of the QML file to load for the main window
     */
    HomeApplication(int &argc, char **argv, const QString &qmlPath);

    /*!
     * Destroys the application object.
     */
    virtual ~HomeApplication();

    static HomeApplication *instance();

    /*!
      * Gets the main window instance associated to this application.
      * If it hasn't been created yet, this will create it.
      */
    QDeclarativeView *mainWindowInstance();

    /*!
      * Gets the path to the QML file to display.
      */
    const QString &qmlPath() const;

    /*!
      * Sets the path to the QML file to display.
      */
    void setQmlPath(const QString &path);

    /*!
     * Restores any installed signal handlers.
     */
    void restoreSignalHandlers();

private slots:
    /*!
     * Sends a dbus-signal after UI is visible, stops the process if it has
     * been started by upstart
     */
    void sendStartupNotifications();

private:
    //! A signal handler that quits the QApplication
    static void quitSignalHandler(int);

    //! The original SIGINT handler
    sighandler_t originalSigIntHandler;

    //! The original SIGTERM handler
    sighandler_t originalSigTermHandler;

    //! Logic for locking and unlocking the screen
    ScreenLock *screenLock;

    //! Logic for setting the device volume
    VolumeControl *volumeControl;

    //! Logic for notifying about battery state
    BatteryNotifier *batteryNotifier;

    //! Logic for showing the USB mode selection dialog
    USBModeSelector *usbModeSelector;

    //! Logic for showing the shutdown screen and related notifications
    ShutdownScreen *shutdownScreen;
};

#endif /* HOMEAPPLICATION_H_ */
