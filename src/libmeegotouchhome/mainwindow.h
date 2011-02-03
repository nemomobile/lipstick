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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <MWindow>
#include <X11/Xdefs.h>

class Home;
class MScene;
class MSceneManager;
class QGLContext;

/*!
 * Main window for the Home application. Contains a MHome widget.
 */
class MainWindow : public MWindow
{
    Q_OBJECT

public:
    /*!
     * Destroys the MainWindow.
     */
    ~MainWindow();

    /*!
     * Returns an instance to MainWindow, creating it if requested.
     * The ownership of the created MainWindow is transferred to the caller.
     *
     * \param create \c true if a MainWindow should be created if it doesn't exist, \c false otherwise
     * \return an instance to MainWindow or NULL if it has not been created
     */
    static MainWindow *instance(bool create = false);

    /*!
     * Returns the OpenGL context.
     *
     * \return the GL context
     */
    static QGLContext *glContext();

    //! \reimp
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void closeEvent(QCloseEvent *event);
    //! \reimp_end

private slots:
    //! Clears keyPressesBeingSent and sends keyPressesToBeSent (if any)
    void markKeyPressesSentAndSendRemainingKeyPresses();

    //! Moves keyPressesBeingSent to the beginning of keyPressesToBeSent
    void markKeyPressesNotSent();

private:
    /*!
     * Constructs a main window for the Home application.
     *
     * \param parent Parent for the widget, defaults to NULL
     */
    MainWindow(QWidget *parent = NULL);

    /*!
     * Sets the X window properties for the window so that the window is not
     * included in the task bar.
     */
    void excludeFromTaskBar();

    //! Applies the orientation and locking from the style
    void applyStyle();

    /*!
     * Changes the _NET_WM_STATE property of a the window.
     *
     * \param set \c true if the first data value will be 1, \c false if it will be 0
     * \param one the first Atom to put in the value
     * \param two the second Atom to put in the value
     */
    void changeNetWmState(bool set, Atom one, Atom two = 0);

    /*!
     * Returns whether the given key should launch the Call UI dialer.
     *
     * \param key a key to check
     * \return \c true if call UI should be launched, \c false otherwise
     */
    static bool isCallUILaunchingKey(int key);

    /*!
     * Sets up the external service to be called when keys are pressed.
     * Expects the passed QStrings to be static constants which will not be
     * destroyed before sendKeyPresses() is called.
     *
     * \param service the name of the DBus service to launch
     * \param path the path of the DBus service to launch
     * \param interface the interface name of the DBus service to launch
     * \param method the name of the DBus method to call
     */
    void setupExternalService(const QString &service, const QString &path, const QString &interface, const QString &method);

    /*!
     * Sends the keypresses to be sent to the external service defined with
     * setupExternalService(). This can only be called after
     * setupExternalService() has been called.
     */
    void sendKeyPresses();

    //! The DBus service of the content search application
    static const QString CONTENT_SEARCH_DBUS_SERVICE;
    //! The DBus path of the content search application
    static const QString CONTENT_SEARCH_DBUS_PATH;
    //! The DBus interface of the content search application
    static const QString CONTENT_SEARCH_DBUS_INTERFACE;
    //! The DBus method of the content search application
    static const QString CONTENT_SEARCH_DBUS_METHOD;

    //! The DBus service of the call ui dialer
    static const QString CALL_UI_DBUS_SERVICE;
    //! The DBus path of the call ui dialer
    static const QString CALL_UI_DBUS_PATH;
    //! The DBus interface of the call ui dialer
    static const QString CALL_UI_DBUS_INTERFACE;
    //! The DBus method of the call ui dialer
    static const QString CALL_UI_DBUS_METHOD;

    //! The MainWindow instance
    static MainWindow *mainWindowInstance;

    //! The OpenGL context
    static QGLContext *openGLContext;

    //! The Home widget
    Home *home;

    //! The service name of the external service being called
    const QString *externalServiceService;
    //! The path of the external service being called
    const QString *externalServicePath;
    //! The interface of the external service being called
    const QString *externalServiceInterface;
    //! The method of the external service being called
    const QString *externalServiceMethod;

    //! Key presses not sent to the external service yet
    QString keyPressesToBeSent;

    //! Key presses being sent to an external service
    QString keyPressesBeingSent;

#ifdef UNIT_TEST
    friend class Ut_MainWindow;
#endif
};

#endif // MAINWINDOW_H
