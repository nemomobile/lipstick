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

#include "connectionselector.h"

#include <QGuiApplication>
#include "homewindow.h"
#include <QQuickItem>
#include <QQmlContext>
#include <QScreen>

#include "utilities/closeeventeater.h"
#include "notifications/notificationmanager.h"

ConnectionSelector::ConnectionSelector(QObject *parent) :
    QObject(parent),
    window(0),
    currentNotification(0)
{
    window = new HomeWindow();
    window->setGeometry(QRect(QPoint(), QGuiApplication::primaryScreen()->size()));
    window->setWindowTitle("Connection");
    window->setContextProperty("connectionSelector", this);
    window->setContextProperty("initialSize", QGuiApplication::primaryScreen()->size());
    window->setSource(QUrl("qrc:/qml/ConnectionSelector.qml"));
    window->installEventFilter(new CloseEventEater(this));
    QQuickItem *rootObject = window->rootObject();
    QObject::connect(rootObject,SIGNAL(canceled()),this,SLOT(onCanceled()));
    QObject::connect(rootObject,SIGNAL(connectionRequested()),
                     this,SLOT(onConnectionRequest()));
}

ConnectionSelector::~ConnectionSelector()
{
    delete window;
}

void ConnectionSelector::setWindowVisible(bool visible)
{
    window->resize(QGuiApplication::primaryScreen()->size());
    if (visible) {

        if (!window->isVisible()) {
            window->showFullScreen();
            emit windowVisibleChanged();
        }
    } else if (window != 0 && window->isVisible()) {
        window->hide();
        emit windowVisibleChanged();
    }
}

bool ConnectionSelector::windowVisible() const
{
    return window != 0 && window->isVisible();
}

void ConnectionSelector::onConnectionRequest()
{
    setWindowVisible(true);
}

void ConnectionSelector::onCanceled()
{
    setWindowVisible(false);
}

void ConnectionSelector::showNotification(const QString &message,const QString &type)
{
    if (message.isEmpty())
        return;

    NotificationManager *manager = NotificationManager::instance();
    QVariantHash hints;
    QString cat = "x-nemo.connectionselector.";
    cat.append(type);
    hints.insert(NotificationManager::HINT_CATEGORY, cat);
    hints.insert(NotificationManager::HINT_PREVIEW_BODY, message);
    uint tmpUint;

    tmpUint = manager->Notify(qApp->applicationName(), 0, QString(), QString(), QString(),
                              QStringList(), hints, -1);
    currentNotification = tmpUint;
}
