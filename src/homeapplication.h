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
#include <QGuiApplication>
#include "lipstickglobal.h"

class QQmlEngine;
class HomeWindow;
class ScreenLock;
class VolumeControl;
class BatteryNotifier;
class USBModeSelector;
class ShutdownScreen;
class ConnectionSelector;

/*!
 * Extends QApplication with features necessary to create a desktop.
 */
class LIPSTICK_EXPORT HomeApplication : public QGuiApplication
{
    Q_OBJECT

    HomeWindow *_mainWindowInstance;
    QString _qmlPath;
    QString _compositorPath;

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
    HomeWindow *mainWindowInstance();

    /*!
     * Gets the QQmlEngine used for all the windows in this application.
     */
    QQmlEngine *engine() const;

    /*!
      * Gets the path to the QML file to display.
      */
    const QString &qmlPath() const;

    /*!
      * Sets the path to the QML file to display.
      */
    void setQmlPath(const QString &path);

    /*!
     * Gets the path to the compositor to load.
     */
    const QString &compositorPath() const;
    /*!
     * Sets the path to the compositor QML file to run.  This must be set before
     * the first window is created, or the mainWindowInstance() method is called.
     */
    void setCompositorPath(const QString &path);

    /*!
     * Restores any installed signal handlers.
     */
    void restoreSignalHandlers();

    /*!
     * Gets the home active flag.
     */
    bool homeActive() const;

signals:
    /*!
     * Emitted whenever the home active flag changes.
     */
    void homeActiveChanged();

protected:
    virtual bool event(QEvent *);

private slots:
    /*!
     * Sends a dbus-signal after UI is visible, stops the process if it has
     * been started by upstart
     */
    void sendStartupNotifications();

private:
    friend class LipstickApi;

    //! A signal handler that quits the QApplication
    static void quitSignalHandler(int);

    //! The original SIGINT handler
    sighandler_t originalSigIntHandler;

    //! The original SIGTERM handler
    sighandler_t originalSigTermHandler;

    //! QML Engine instance
    QQmlEngine *qmlEngine;

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

    //! Login for showing the connection selector
    ConnectionSelector *connectionSelector;
};

#endif /* HOMEAPPLICATION_H_ */
