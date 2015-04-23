/*!
 * @file qmusbmode.cpp
 * @brief QmUSBMode

   <p>
   Copyright (C) 2009-2011 Nokia Corporation

   @author Timo Olkkonen <ext-timo.p.olkkonen@nokia.com>
   @author Matias Muhonen <ext-matias.muhonen@nokia.com>

   This file is part of SystemSW QtAPI.

   SystemSW QtAPI is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License
   version 2.1 as published by the Free Software Foundation.

   SystemSW QtAPI is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with SystemSW QtAPI.  If not, see <http://www.gnu.org/licenses/>.
   </p>
 */
#include "qmusbmode.h"
#include "qmusbmode_p.h"

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusReply>
#include <QDBusInterface>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QMetaMethod>
#endif

#include <QDebug>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#if HAVE_USB_MODED_DEV
    #include <usb_moded-dbus.h>
    #include <usb_moded-modes.h>
#else
    /* Use QmSystem D-Bus i/f declarations because usb-moded-dev is not available */
    #include "msystemdbus_p.h"
#endif

#define SIGNAL_USB_MODE 0
#define SIGNAL_USB_ERROR 1
#define SIGNAL_USB_SUPPORTED_MODES 2

namespace MeeGo {

QmUSBMode::QmUSBMode(QObject *parent) : QObject(parent) {
    MEEGO_INITIALIZE(QmUSBMode);

    connect(priv, SIGNAL(modeChanged(MeeGo::QmUSBMode::Mode)), this, SIGNAL(modeChanged(MeeGo::QmUSBMode::Mode)));
    connect(priv, SIGNAL(fileSystemWillUnmount(MeeGo::QmUSBMode::MountPath)), this, SIGNAL(fileSystemWillUnmount(MeeGo::QmUSBMode::MountPath)));
    connect(priv, SIGNAL(error(const QString&)), this, SIGNAL(error(const QString&)));
    connect(priv, SIGNAL(supportedModesChanged(QList<MeeGo::QmUSBMode::Mode>)), this, SIGNAL(supportedModesChanged(QList<MeeGo::QmUSBMode::Mode>)));
}

QmUSBMode::~QmUSBMode() {
    MEEGO_PRIVATE(QmUSBMode);

    disconnect(priv, SIGNAL(modeChanged(MeeGo::QmUSBMode::Mode)), this, SIGNAL(modeChanged(MeeGo::QmUSBMode::Mode)));
    disconnect(priv, SIGNAL(fileSystemWillUnmount(MeeGo::QmUSBMode::MountPath)), this, SIGNAL(fileSystemWillUnmount(MeeGo::QmUSBMode::MountPath)));
    disconnect(priv, SIGNAL(error(const QString&)), this, SIGNAL(error(const QString&)));
    disconnect(priv, SIGNAL(supportedModesChanged(QList<MeeGo::QmUSBMode::Mode>)), this, SIGNAL(supportedModesChanged(QList<MeeGo::QmUSBMode::Mode>)));

    MEEGO_UNINITIALIZE(QmUSBMode);
}

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
void QmUSBMode::connectNotify(const QMetaMethod &signal) {
#else
void QmUSBMode::connectNotify(const char *signal) {
#endif
    MEEGO_PRIVATE(QmUSBMode)

    /* QObject::connect() needs to be thread-safe */
    QMutexLocker locker(&priv->connectMutex);

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    if (signal == QMetaMethod::fromSignal(&QmUSBMode::modeChanged) ||
        signal == QMetaMethod::fromSignal(&QmUSBMode::fileSystemWillUnmount)) {
#else
    if ((QLatin1String(signal) == QLatin1String(QMetaObject::normalizedSignature(SIGNAL(modeChanged(MeeGo::QmUSBMode::Mode))))) ||
        (QLatin1String(signal) == QLatin1String(QMetaObject::normalizedSignature(SIGNAL(fileSystemWillUnmount(MeeGo::QmUSBMode::MountPath)))))) {
#endif
        if (0 == priv->connectCount[SIGNAL_USB_MODE]) {
            QDBusConnection::systemBus().connect(USB_MODE_SERVICE,
                                                 USB_MODE_OBJECT,
                                                 USB_MODE_INTERFACE,
                                                 USB_MODE_SIGNAL_NAME,
                                                 priv,
                                                 SLOT(modeChanged(const QString&)));
        }
        priv->connectCount[SIGNAL_USB_MODE]++;
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    } else if (signal == QMetaMethod::fromSignal(&QmUSBMode::error)) {
#else
    } else if (QLatin1String(signal) == QLatin1String(QMetaObject::normalizedSignature(SIGNAL(error(const QString&))))) {
#endif
        if (0 == priv->connectCount[SIGNAL_USB_ERROR]) {
            QDBusConnection::systemBus().connect(USB_MODE_SERVICE,
                                                 USB_MODE_OBJECT,
                                                 USB_MODE_INTERFACE,
                                                 USB_MODE_ERROR_SIGNAL_NAME,
                                                 priv,
                                                 SLOT(didReceiveError(const QString&)));
        }
        priv->connectCount[SIGNAL_USB_ERROR]++;
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    } else if (signal == QMetaMethod::fromSignal(&QmUSBMode::supportedModesChanged)) {
#else
    } else if (QLatin1String(signal) == QLatin1String(QMetaObject::normalizedSignature(SIGNAL(supportedModesChanged(QList<MeeGo::QmUSBMode::Mode>))))) {
#endif
        if (0 == priv->connectCount[SIGNAL_USB_SUPPORTED_MODES]) {
            QDBusConnection::systemBus().connect(USB_MODE_SERVICE,
                                                 USB_MODE_OBJECT,
                                                 USB_MODE_INTERFACE,
                                                 USB_MODE_SUPPORTED_MODES_SIGNAL_NAME,
                                                 priv,
                                                 SLOT(supportedModesChanged(const QString&)));
        }
        priv->connectCount[SIGNAL_USB_SUPPORTED_MODES]++;
    }
}

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
void QmUSBMode::disconnectNotify(const QMetaMethod &signal) {
#else
void QmUSBMode::disconnectNotify(const char *signal) {
#endif
    MEEGO_PRIVATE(QmUSBMode)

    /* QObject::disconnect() needs to be thread-safe */
    QMutexLocker locker(&priv->connectMutex);

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    if (signal == QMetaMethod::fromSignal(&QmUSBMode::modeChanged) ||
        signal == QMetaMethod::fromSignal(&QmUSBMode::fileSystemWillUnmount)) {
#else
    if ((QLatin1String(signal) == QLatin1String(QMetaObject::normalizedSignature(SIGNAL(modeChanged(MeeGo::QmUSBMode::Mode))))) ||
        (QLatin1String(signal) == QLatin1String(QMetaObject::normalizedSignature(SIGNAL(fileSystemWillUnmount(MeeGo::QmUSBMode::MountPath)))))) {
#endif
        priv->connectCount[SIGNAL_USB_MODE]--;

        if (0 == priv->connectCount[SIGNAL_USB_MODE]) {
            QDBusConnection::systemBus().disconnect(USB_MODE_SERVICE,
                                                    USB_MODE_OBJECT,
                                                    USB_MODE_INTERFACE,
                                                    USB_MODE_SIGNAL_NAME,
                                                    priv,
                                                    SLOT(modeChanged(const QString&)));
        }
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    } else if (signal == QMetaMethod::fromSignal(&QmUSBMode::error)) {
#else
    } else if (QLatin1String(signal) == QLatin1String(QMetaObject::normalizedSignature(SIGNAL(error(const QString&))))) {
#endif
        priv->connectCount[SIGNAL_USB_ERROR]--;

        if (0 == priv->connectCount[SIGNAL_USB_ERROR]) {
            QDBusConnection::systemBus().disconnect(USB_MODE_SERVICE,
                                                    USB_MODE_OBJECT,
                                                    USB_MODE_INTERFACE,
                                                    USB_MODE_ERROR_SIGNAL_NAME,
                                                    priv,
                                                    SLOT(didReceiveError(const QString&)));
        }
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    } else if (signal == QMetaMethod::fromSignal(&QmUSBMode::supportedModesChanged)) {
#else
    } else if (QLatin1String(signal) == QLatin1String(QMetaObject::normalizedSignature(SIGNAL(supportedModesChanged(QList<MeeGo::QmUSBMode::Mode>))))) {
#endif
        priv->connectCount[SIGNAL_USB_SUPPORTED_MODES]++;

        if (0 == priv->connectCount[SIGNAL_USB_SUPPORTED_MODES]) {
            QDBusConnection::systemBus().disconnect(USB_MODE_SERVICE,
                                                    USB_MODE_OBJECT,
                                                    USB_MODE_INTERFACE,
                                                    USB_MODE_SUPPORTED_MODES_SIGNAL_NAME,
                                                    priv,
                                                    SLOT(supportedModesChanged(const QString&)));
        }
    }
}

QList<QmUSBMode::Mode> QmUSBMode::getSupportedModes() {
    MEEGO_PRIVATE(QmUSBMode);

    QList<QmUSBMode::Mode> supportedModes;
    QDBusReply<QString> usbModeReply = QDBusConnection::systemBus().call(
                                           QDBusMessage::createMethodCall(USB_MODE_SERVICE, USB_MODE_OBJECT, USB_MODE_INTERFACE,
                                                                          USB_MODE_LIST));
    if (usbModeReply.isValid()) {
        foreach (const QString &usbModeString, usbModeReply.value().split(", ")) {
            QmUSBMode::Mode usbMode = priv->stringToMode(usbModeString);
            if (usbMode != QmUSBMode::Undefined) {
                supportedModes.append(usbMode);
            }
        }
    }
    return supportedModes;
}

QmUSBMode::Mode QmUSBMode::getMode() {
    MEEGO_PRIVATE(QmUSBMode);

    QDBusReply<QString> usbModeReply = QDBusConnection::systemBus().call(
                                           QDBusMessage::createMethodCall(USB_MODE_SERVICE, USB_MODE_OBJECT, USB_MODE_INTERFACE,
                                                                          USB_MODE_STATE_REQUEST));
    if (usbModeReply.isValid()) {
        return priv->stringToMode(usbModeReply.value());
    }
    return QmUSBMode::Undefined;
}

bool QmUSBMode::setMode(QmUSBMode::Mode mode) {
    MEEGO_PRIVATE(QmUSBMode);

    // The OviSuite, MassStorage, ChargingOnly and SDK modes can be requested
    if (!(OviSuite == mode || MassStorage == mode || ChargingOnly == mode || SDK == mode || Developer == mode || 
         MTP == mode || Adb == mode || Diag == mode || Host == mode || ConnectionSharing == mode || Charger == mode)) {
        return false;
    }

    QString usbModeString = priv->modeToString(mode);
    if (usbModeString.isEmpty()) {
        return false;
    }

    QDBusMessage usbModeSetCall = QDBusMessage::createMethodCall(USB_MODE_SERVICE, USB_MODE_OBJECT, USB_MODE_INTERFACE, USB_MODE_STATE_SET);
    usbModeSetCall << usbModeString;

    (void)QDBusConnection::systemBus().call(usbModeSetCall, QDBus::NoBlock);
    return true;
}

bool QmUSBMode::setDefaultMode(QmUSBMode::Mode mode) {
    MEEGO_PRIVATE(QmUSBMode);

    // The OviSuite, MassStorage, ChargingOnly and Ask modes can be requested
    if (!(OviSuite == mode || MassStorage == mode || ChargingOnly == mode || SDK == mode || Developer == mode || 
         MTP == mode || Adb == mode || Diag == mode || Host == mode || Ask == mode || ConnectionSharing == mode ||
         Charger == mode )) {
        return false;
    }

    QString str = priv->modeToString(mode);
    if (str.isEmpty()) {
        return false;
    }

    QDBusInterface usbModed(USB_MODE_SERVICE, USB_MODE_OBJECT, USB_MODE_INTERFACE, QDBusConnection::systemBus());
    QDBusMessage result = usbModed.call(QLatin1String(USB_MODE_CONFIG_SET), str);

    return (result.type() != QDBusMessage::ErrorMessage);
}

QmUSBMode::Mode QmUSBMode::getDefaultMode() {
    MEEGO_PRIVATE(QmUSBMode);

    QDBusInterface usbModed(USB_MODE_SERVICE, USB_MODE_OBJECT, USB_MODE_INTERFACE, QDBusConnection::systemBus());
    QDBusMessage result = usbModed.call(QLatin1String(USB_MODE_CONFIG_GET));

    if (result.type() != QDBusMessage::ReplyMessage || result.arguments().size() != 1) {
        qDebug() << "Got error while requesting default USB mode: " << result.errorMessage();
        return QmUSBMode::Undefined;
    }

    QString mode = result.arguments().first().toString();
    return priv->stringToMode(mode);
}

QmUSBMode::MountOptionFlags QmUSBMode::mountStatus(QmUSBMode::MountPath mountPath) {
    QmUSBMode::MountOptionFlags mountOptions;
    const char *path = "/home/user/MyDocs";
    struct stat statbuf;

    if (QmUSBMode::DocumentDirectoryMount != mountPath) {
        /* We don't currently support other mount paths */
        goto out;
    }

    memset(&statbuf, 0, sizeof statbuf);

    if (0 != stat(path, &statbuf)) {
        /* Path not available */
        goto out;
    }

    if (S_ISDIR(statbuf.st_mode) && 0 == access(path, R_OK | W_OK)) {
        mountOptions |= QmUSBMode::ReadWriteMount;
        goto out;
    }

    if (S_ISDIR(statbuf.st_mode) && 0 == access(path, R_OK)) {
        mountOptions |= QmUSBMode::ReadOnlyMount;
        goto out;
    }

out:
    return mountOptions;
}

// private class

QmUSBModePrivate::QmUSBModePrivate(QObject *parent) : QObject(parent) {
    memset(connectCount, 0x0, sizeof (connectCount));
}

QmUSBModePrivate::~QmUSBModePrivate() {
}

QString QmUSBModePrivate::modeToString(QmUSBMode::Mode mode) {
    switch (mode) {
    case QmUSBMode::Connected:
        return USB_CONNECTED;
    case QmUSBMode::DataInUse:
        return DATA_IN_USE;
    case QmUSBMode::Disconnected:
        return USB_DISCONNECTED;
    case QmUSBMode::MassStorage:
        return MODE_MASS_STORAGE;
    case QmUSBMode::ChargingOnly:
        return MODE_CHARGING;
    case QmUSBMode::OviSuite:
        return MODE_OVI_SUITE;
    case QmUSBMode::Ask:
        return MODE_ASK;
    case QmUSBMode::Undefined:
        return MODE_UNDEFINED;
    case QmUSBMode::ModeRequest:
        return USB_CONNECTED_DIALOG_SHOW;
    case QmUSBMode::SDK:
        return MODE_WINDOWS_NET;
    case QmUSBMode::MTP:
	return MODE_MTP;
    case QmUSBMode::Adb:
	return MODE_ADB;
    case QmUSBMode::Diag:
	return MODE_DIAG;
    case QmUSBMode::Developer:
	return MODE_DEVELOPER;
    case QmUSBMode::ConnectionSharing:
	return MODE_CONNECTION_SHARING;
    case QmUSBMode::Host:
	return MODE_HOST;
    case QmUSBMode::Charger:
        return MODE_CHARGER;
    default:
        return "";
    }
}

QmUSBMode::Mode QmUSBModePrivate::stringToMode(const QString &str) {
    if (str == USB_CONNECTED) {
        return QmUSBMode::Connected;
    } else if (str == USB_DISCONNECTED) {
        return QmUSBMode::Disconnected;
    } else if (str == DATA_IN_USE) {
        return QmUSBMode::DataInUse;
    } else if (str == MODE_MASS_STORAGE) {
        return QmUSBMode::MassStorage;
    } else if (str == MODE_OVI_SUITE) {
        return QmUSBMode::OviSuite;
    } else if (str == MODE_CHARGING) {
        return QmUSBMode::ChargingOnly;
    } else if (str == MODE_ASK) {
        return QmUSBMode::Ask;
    } else if (str == MODE_UNDEFINED) {
        return QmUSBMode::Undefined;
    } else if (str == USB_CONNECTED_DIALOG_SHOW) {
        return QmUSBMode::ModeRequest;
    } else if (str == MODE_WINDOWS_NET) {
        return QmUSBMode::SDK;
    } else if (str == MODE_MTP) {
	return QmUSBMode::MTP;
    } else if (str == MODE_ADB) {
	return QmUSBMode::Adb;
    } else if (str == MODE_DIAG) {
	return QmUSBMode::Diag;
    } else if (str == MODE_DEVELOPER) {
	return QmUSBMode::Developer;
    } else if (str == MODE_CONNECTION_SHARING) {
	return QmUSBMode::ConnectionSharing;
    } else if (str == MODE_HOST) {
	return QmUSBMode::Host;
    } else if (str == MODE_CHARGER) {
        return QmUSBMode::Charger;
    } else {
        return QmUSBMode::Undefined;
    }
}

void QmUSBModePrivate::didReceiveError(const QString &errorCode) {
    emit error(errorCode);
}

void QmUSBModePrivate::modeChanged(const QString &mode) {
    if (mode == USB_PRE_UNMOUNT) {
        /* The pre-unmount signal only concerns MyDocs, for now */
        emit fileSystemWillUnmount(QmUSBMode::DocumentDirectoryMount);
    } else {
        emit modeChanged(stringToMode(mode));
    }
}

void QmUSBModePrivate::supportedModesChanged(const QString &usbModeReply) {
    QList<MeeGo::QmUSBMode::Mode> supportedModes;
    foreach (const QString &usbModeString, usbModeReply.split(", ")) {
        QmUSBMode::Mode usbMode = stringToMode(usbModeString);
        if (usbMode != QmUSBMode::Undefined) {
            supportedModes.append(usbMode);
        }
    }

    emit supportedModesChanged(supportedModes);
}

} // namespace MeeGo
