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
    //! \reimp_end

private:
    //! The DBus service of the content search application
    static const QString CONTENT_SEARCH_DBUS_SERVICE;
    //! The DBus path of the content search application
    static const QString CONTENT_SEARCH_DBUS_PATH;
    //! The DBus interface of the content search application
    static const QString CONTENT_SEARCH_DBUS_INTERFACE;

    //! The MainWindow instance
    static MainWindow *mainWindowInstance;

    //! The Home widget
    Home *home;

    //! The OpenGL context
    static QGLContext *openGLContext;

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

    /*!
     * Changes the _NET_WM_STATE property of a the window.
     *
     * \param set \c true if the first data value will be 1, \c false if it will be 0
     * \param one the first Atom to put in the value
     * \param two the second Atom to put in the value
     */
    void changeNetWmState(bool set, Atom one, Atom two = 0);

    /*!
     * Returns whether the given key should launch the content search.
     *
     * \param key a key to check
     * \return \c true if content search should be launched, \c false otherwise
     */
    static bool isContentSearchLaunchingKey(int key);

    /*!
     * Launches content search with the given search string.
     *
     * \param searchString the search string to send to content search
     */
    void launchContentSearch(const QString &searchString);

#ifdef UNIT_TEST
    friend class Ut_MainWindow;
#endif
};

#endif // MAINWINDOW_H
