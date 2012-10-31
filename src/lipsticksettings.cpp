
// This file is part of lipstick, a QML desktop library
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License version 2.1 as published by the Free Software Foundation
// and appearing in the file LICENSE.LGPL included in the packaging
// of this file.
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// Copyright (c) 2012, Robin Burchell <robin+nemo@viroteck.net>
//

#include <QX11Info>
#include <QDeclarativeView>
#include <QApplication>
#include <QDesktopWidget>
#include <QDBusMessage>
#include <QDBusPendingCall>
#include <QDBusConnection>
#include <QDebug>
#include <QtSensors/QOrientationSensor>
#include <QtSensors/QOrientationReading>
#include <mce/mode-names.h>

#include "lipsticksettings.h"
#include "homeapplication.h"

#include <X11/Xatom.h>
#include <X11/Xlib.h>

Q_GLOBAL_STATIC(LipstickSettings, settingsInstance)

LipstickSettings::LipstickSettings()
    : QObject()
    , _lockscreenVisible(false)
{
}

LipstickSettings *LipstickSettings::instance()
{
    return settingsInstance();
}

bool LipstickSettings::lockscreenVisible() const
{
    return _lockscreenVisible;
}

void LipstickSettings::setLockscreenVisible(bool lockscreenVisible, bool externallyChanged)
{
    if (lockscreenVisible == _lockscreenVisible)
        return;

    _lockscreenVisible = lockscreenVisible;
    QDeclarativeView *view = HomeApplication::instance()->mainWindowInstance();

    long layer = 0;
    if (lockscreenVisible) {
        // mcompositor searches for exactly this title when marking a window as
        // a lockscreen, so make it happy
        view->setWindowTitle("Screen Lock");
        layer = 2;

        if (!externallyChanged) {
            // Lock screen entered from inside the home screen: request screen to be locked
            QDBusMessage message = QDBusMessage::createMethodCall("com.nokia.mce", "/com/nokia/mce/request", "com.nokia.mce.request", "req_tklock_mode_change");
            message.setArguments(QVariantList() << MCE_TK_LOCKED_DELAY);
            QDBusConnection::systemBus().asyncCall(message);
        }
    } else {
        view->setWindowTitle("Lipstick");
    }

    // Set the stacking layer
    Display *display = QX11Info::display();
    Atom stackingLayerAtom = XInternAtom(display, "_MEEGO_STACKING_LAYER", False);
    if (stackingLayerAtom != None)
        XChangeProperty(display, view->winId(), stackingLayerAtom, XA_CARDINAL, 32, PropModeReplace, (unsigned char*)&layer, 1);

    emit lockscreenVisibleChanged();
}

QSize LipstickSettings::screenSize()
{
    return QApplication::desktop()->screenGeometry(HomeApplication::instance()->mainWindowInstance()).size();
}

bool LipstickSettings::getIsInPortrait()
{
    QtMobility::QOrientationSensor sensor;
    sensor.start();
    QtMobility::QOrientationReading::Orientation orientation = sensor.reading()->orientation();
    qDebug() << Q_FUNC_INFO << "current orientation is" << orientation;
    sensor.stop();
    return orientation == QtMobility::QOrientationReading::TopUp;
}
