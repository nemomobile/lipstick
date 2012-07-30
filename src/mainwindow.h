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

#include <QKeyEvent>
#include <QDeclarativeView>

#include <X11/Xdefs.h>

/*!
 * Main window for the Home application. Contains a MHome widget.
 */
class MainWindow : public QDeclarativeView
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

    //! \reimp
    virtual void closeEvent(QCloseEvent *event);
    //! \reimp_end

private:
    /*!
     * Constructs a main window for the Home application.
     *
     * \param parent Parent for the widget, defaults to NULL
     */
    MainWindow(QWidget *parent = NULL);

    //! The MainWindow instance
    static MainWindow *mainWindowInstance;

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

};

#endif // MAINWINDOW_H
