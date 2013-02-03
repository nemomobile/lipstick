/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** Copyright (C) 2012 Jolla Ltd.
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
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QDesktopWidget>
#include <QX11Info>
#include <X11/extensions/shape.h>
#include "utilities/closeeventeater.h"
#include "xtools/x11wrapper.h"
#include "notifications/notificationmanager.h"
#include "homeapplication.h"
#include "shutdownscreen.h"

ShutdownScreen::ShutdownScreen(QObject *parent) :
    QObject(parent),
    window(0),
    systemState(new MeeGo::QmSystemState(this))
{
    connect(systemState, SIGNAL(systemStateChanged(MeeGo::QmSystemState::StateIndication)), this, SLOT(applySystemState(MeeGo::QmSystemState::StateIndication)));
}

void ShutdownScreen::setWindowVisible(bool visible)
{
    if (visible) {
        if (window == 0) {
            window = new QDeclarativeView();
            window->setAttribute(Qt::WA_TranslucentBackground);
            window->setAttribute(Qt::WA_X11DoNotAcceptFocus);
            window->setAttribute(Qt::WA_X11NetWmWindowTypeNotification);
            window->setWindowTitle("Shutdown");
            window->setResizeMode(QDeclarativeView::SizeRootObjectToView);
            window->viewport()->setAutoFillBackground(false);
            window->rootContext()->setContextProperty("initialSize", QApplication::desktop()->screenGeometry(window).size());
            window->rootContext()->setContextProperty("shutdownScreen", this);
            window->setSource(QUrl("qrc:/qml/ShutdownScreen.qml"));
            window->installEventFilter(new CloseEventEater(this));

            Display *dpy = QX11Info::display();
            unsigned int customRegion[] = { window->rect().x(), window->rect().y(), window->rect().width(), window->rect().height() };
            Atom customRegionAtom = X11Wrapper::XInternAtom(dpy, "_MEEGOTOUCH_CUSTOM_REGION", False);
            X11Wrapper::XChangeProperty(dpy, window->winId(), customRegionAtom, XA_CARDINAL, 32, PropModeReplace, reinterpret_cast<unsigned char *>(&customRegion[0]), 4);

            Atom stackingLayerAtom = X11Wrapper::XInternAtom(dpy, "_MEEGO_STACKING_LAYER", False);
            if (stackingLayerAtom != None) {
                long layer = 6;
                X11Wrapper::XChangeProperty(dpy, window->winId(), stackingLayerAtom, XA_CARDINAL, 32, PropModeReplace, (unsigned char*)&layer, 1);
            }
            X11Wrapper::XSync(dpy, False);
        }

        if (!window->isVisible()) {
            window->show();
            emit windowVisibleChanged();
        }
    } else if (window != 0 && window->isVisible()) {
        window->hide();
        emit windowVisibleChanged();
    }
}

bool ShutdownScreen::windowVisible() const
{
    return window != 0 && window->isVisible();
}

void ShutdownScreen::applySystemState(MeeGo::QmSystemState::StateIndication what)
{
    switch (what) {
        case MeeGo::QmSystemState::Shutdown:
            // To avoid early quitting on shutdown
            HomeApplication::instance()->restoreSignalHandlers();
            setWindowVisible(true);
            break;

        case MeeGo::QmSystemState::ThermalStateFatal:
            //% "Temperature too high. Device shutting down."
            createAndPublishNotification("x-nemo.battery.temperature", qtTrId("qtn_shut_high_temp"));
            break;

        case MeeGo::QmSystemState::ShutdownDeniedUSB:
            //% "USB cable plugged in. Unplug the USB cable to shutdown."
            createAndPublishNotification("device.added", qtTrId("qtn_shut_unplug_usb"));
            break;

        case MeeGo::QmSystemState::BatteryStateEmpty:
            //% "Battery empty. Device shutting down."
            createAndPublishNotification("x-nemo.battery.shutdown", qtTrId("qtn_shut_batt_empty"));
            break;

        default:
            break;
    }
}

void ShutdownScreen::createAndPublishNotification(const QString &category, const QString &body)
{
    NotificationManager *manager = NotificationManager::instance();
    QVariantHash hints;
    hints.insert(NotificationManager::HINT_CATEGORY, category);
    hints.insert(NotificationManager::HINT_PREVIEW_BODY, body);
    manager->Notify(qApp->applicationName(), 0, QString(), QString(), QString(), QStringList(), hints, -1);
}
