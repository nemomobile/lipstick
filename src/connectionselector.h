/****************************************************************************
**
** Copyright (C) 2013 Jolla Ltd
** Contact: lorn.potter@jollamobile.com
**
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
****************************************************************************/

#ifndef CONNECTIONSELECTOR_H
#define CONNECTIONSELECTOR_H

#include <QObject>
#include "lipstickglobal.h"

class HomeWindow;

class LIPSTICK_EXPORT ConnectionSelector : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool windowVisible READ windowVisible WRITE setWindowVisible NOTIFY windowVisibleChanged)
public:
    explicit ConnectionSelector(QObject *parent = 0);
    virtual~ConnectionSelector();
    /*!
     * Returns whether the window is visible or not.
     *
     * \return \c true if the window is visible, \c false otherwise
     */
    bool windowVisible() const;

    /*!
     * Sets the visibility of the window.
     *
     * \param visible \c true if the window should be visible, \c false otherwise
     */
    void setWindowVisible(bool visible);

signals:

    //! Sent when the visibility of the window has changed.
    void windowVisibleChanged();

    void hide();
    void show();

public slots:
    void onConnectionRequest();
    void onCanceled();
    void showNotification(const QString &message, const QString &type);

private:
    HomeWindow *window;
    uint currentNotification;

};

#endif // CONNECTIONSELECTOR_H
