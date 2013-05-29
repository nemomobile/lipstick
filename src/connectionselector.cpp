/***************************************************************************
**
** Copyright (C) 2013 Jolla Ltd.
** Contact: Robin Burchell <robin.burchell@jollamobile.com>
**
** This file is part of lipstick.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include <QApplication>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QDesktopWidget>
#include <QTimer>
#include "utilities/closeeventeater.h"
#include "connectionselector.h"

ConnectionSelector::ConnectionSelector(QObject *parent) :
    QObject(parent),
    window(new QDeclarativeView())
{
    QTimer::singleShot(0, this, SLOT(createWindow()));
}

ConnectionSelector::~ConnectionSelector()
{
    delete window;
}

void ConnectionSelector::createWindow()
{
    window->setAttribute(Qt::WA_TranslucentBackground);
    window->setAttribute(Qt::WA_X11NetWmWindowTypeMenu);
    window->setWindowTitle("Connection");
    window->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    window->viewport()->setAutoFillBackground(false);
    window->rootContext()->setContextProperty("initialSize", QApplication::desktop()->screenGeometry(window).size());
    window->rootContext()->setContextProperty("connectionSelector", this);
    window->setSource(QUrl("qrc:/qml/ConnectionSelector.qml"));
    window->installEventFilter(new CloseEventEater(this));
}

void ConnectionSelector::setWindowVisible(bool visible)
{
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
