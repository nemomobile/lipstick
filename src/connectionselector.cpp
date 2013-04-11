/****************************************************************************
**
** Copyright (C) 2013 Jolla Ltd
** Contact: lorn.potter@gmail.com
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

#include <QApplication>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QDesktopWidget>
#include <QGraphicsObject>
#include <QDebug>
#include <QScreen>

#include "utilities/closeeventeater.h"
#include "notificationmanager.h"

ConnectionSelector::ConnectionSelector(QObject *parent) :
    QObject(parent),
    window(0),
    currentNotification(0)
{
    if (window == 0) {
        window = new QDeclarativeView();
        window->setAttribute(Qt::WA_TranslucentBackground);
        window->setAttribute(Qt::WA_X11NetWmWindowTypeMenu);
        window->setWindowTitle("Connection");
        window->setResizeMode(QDeclarativeView::SizeRootObjectToView);
        window->viewport()->setAutoFillBackground(false);
        window->rootContext()->setContextProperty("connectionSelector", this);
        window->rootContext()->setContextProperty("showButtons", QVariant(true));

        QSize desktopSize = QApplication::desktop()->screenGeometry(window).size();
        if (desktopSize.width() > desktopSize.height()) {
            window->rootContext()->setContextProperty("initialSize", desktopSize);
        } else {
            QSize initialSize(desktopSize.height(),desktopSize.width());
            window->rootContext()->setContextProperty("initialSize", initialSize);
        }
        window->setSource(QUrl("qrc:/qml/ConnectionSelectionBanner.qml"));
        window->installEventFilter(new CloseEventEater(this));
        QObject *rootObject = qobject_cast< QObject * >(window->rootObject());
        QObject::connect(rootObject,SIGNAL(canceled()),this,SLOT(onCanceled()));
        QObject::connect(rootObject,SIGNAL(connectionRequested()),
                         this,SLOT(onConnectionRequest()));
    }
}

ConnectionSelector::~ConnectionSelector()
{
}

void ConnectionSelector::setWindowVisible(bool visible)
{
    window->resize(QApplication::desktop()->screenGeometry(window).size());
    if (visible) {
        emit dialogShown();

        if (!window->isVisible()) {
            window->show();
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
    window->rootObject()->setProperty("showButtons", true);
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
    hints.insert(NotificationManager::HINT_URGENCY, 1);
    QString cat = "connection.";
    cat.append(type);
    hints.insert(NotificationManager::HINT_CATEGORY, cat);
    hints.insert(NotificationManager::HINT_PREVIEW_BODY, message);
    uint tmpUint;

    tmpUint = manager->Notify(qApp->applicationName(), 0, QString(), QString(), QString(),
                              QStringList(), hints, -1);
    currentNotification = tmpUint;
}
