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

#include <QApplication>
#include <QDeclarativeView>

class XEventListener;

/*!
 * Extends QApplication with features necessary to create a desktop.
 */
class HomeApplication : public QApplication
{
    Q_OBJECT

    QList<XEventListener*> xEventListeners;
    QList<XEventListener*> toBeRemovedEventListeners;
    bool iteratorActiveForEventListenerContainer;
    int xDamageEventBase;
    int xDamageErrorBase;
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

signals:

    /*!
     * Signal application about a changed X pixmap
     */
    void damageEvent(Qt::HANDLE &damage, short &x, short &y, unsigned short &width, unsigned short &height);

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

};

#endif /* HOMEAPPLICATION_H_ */
