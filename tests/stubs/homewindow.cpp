/***************************************************************************
**
** Copyright (C) 2013 Jolla Ltd.
** Contact: Aaron Kennedy <aaron.kennedy@jollamobile.com>
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

#include "homewindow.h"

#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>

class HomeWindowPrivate : public QQuickView
{
public:
    HomeWindowPrivate();
    bool isVisible;
    bool isNotification;
};

HomeWindowPrivate::HomeWindowPrivate()
: isVisible(false), isNotification(false)
{
}

HomeWindow::HomeWindow()
{
    d = new HomeWindowPrivate;
    d->setResizeMode(QQuickView::SizeRootObjectToView);
}

HomeWindow::~HomeWindow()
{
    d->hide();
    d->isVisible = false;
    delete d;
}

bool HomeWindow::isVisible() const
{
    return d->isVisible;
}

void HomeWindow::show()
{
    d->show();
    d->isVisible = true;
}

void HomeWindow::hide()
{
    d->hide();
    d->isVisible = false;
}

void HomeWindow::showFullScreen()
{
    d->showFullScreen();
    d->isVisible = true;
}

QQuickItem *HomeWindow::rootObject() const
{
    return d->rootObject();
}

void HomeWindow::setSource(const QUrl &url)
{
    d->setSource(url);
}

void HomeWindow::setWindowTitle(const QString &t)
{
    d->setTitle(t);
}

bool HomeWindow::hasErrors() const
{
    return d->status() == QQuickView::Error;
}

QList<QQmlError> HomeWindow::errors() const
{
    return d->errors();
}

bool HomeWindow::isNotification() const
{
    return d->isNotification;
}

void HomeWindow::setIsNotification(bool n)
{
    d->isNotification = n;
}

void HomeWindow::resize(const QSize &s)
{
    d->resize(s);
}

void HomeWindow::setGeometry(const QRect &g)
{
    d->setGeometry(g);
}

QQmlEngine *HomeWindow::engine() const
{
    return d->engine();
}

void HomeWindow::setContextProperty(const QString &name, const QVariant &v)
{
    d->engine()->rootContext()->setContextProperty(name, v);
}

void HomeWindow::setContextProperty(const QString &name, QObject *v)
{
    d->engine()->rootContext()->setContextProperty(name, v);
}
