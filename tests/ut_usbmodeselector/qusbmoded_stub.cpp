/*
 * Copyright (C) 2015 Jolla Ltd.
 * Contact: Slava Monich <slava.monich@jolla.com>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Nemo Mobile nor the names of its contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <qusbmoded.h>

#include "usb_moded-dbus.h"
#include "usb_moded-modes.h"

// States (from usb_moded-dbus.h)
const QString QUsbMode::Mode::Connected(USB_CONNECTED);
const QString QUsbMode::Mode::DataInUse(DATA_IN_USE);
const QString QUsbMode::Mode::Disconnected(USB_DISCONNECTED);
const QString QUsbMode::Mode::ModeRequest(USB_CONNECTED_DIALOG_SHOW);

// Modes (from usb_moded-modes.h)
const QString QUsbMode::Mode::Undefined(MODE_UNDEFINED);
const QString QUsbMode::Mode::Ask(MODE_ASK);
const QString QUsbMode::Mode::MassStorage(MODE_MASS_STORAGE);
const QString QUsbMode::Mode::Developer(MODE_DEVELOPER);
const QString QUsbMode::Mode::MTP(MODE_MTP);
const QString QUsbMode::Mode::Host(MODE_HOST);
const QString QUsbMode::Mode::ConnectionSharing(MODE_CONNECTION_SHARING);
const QString QUsbMode::Mode::Diag(MODE_DIAG);
const QString QUsbMode::Mode::Adb(MODE_ADB);
const QString QUsbMode::Mode::PCSuite(MODE_PC_SUITE);
const QString QUsbMode::Mode::Charging(MODE_CHARGING);
const QString QUsbMode::Mode::Charger(MODE_CHARGER);

QUsbMode::QUsbMode(QObject* aParent) :
    QObject(aParent)
{
}

class QUsbModed::Private
{
public:
    QStringList iSupportedModes;
    QString iConfigMode;
    QString iCurrentMode;

    Private()
    {
        iSupportedModes.append(QUsbModed::Mode::ConnectionSharing);
        iSupportedModes.append(QUsbModed::Mode::Developer);
        iSupportedModes.append(QUsbModed::Mode::PCSuite);
        iSupportedModes.append(QUsbModed::Mode::Charging);
    }
};

QUsbModed::QUsbModed(QObject* aParent) :
    QUsbMode(aParent),
    iPrivate(new Private)
{
}

QUsbModed::~QUsbModed()
{
    delete iPrivate;
}

QStringList QUsbModed::supportedModes() const
{
    return iPrivate->iSupportedModes;
}

bool QUsbModed::available() const
{
    return true;
}

QString QUsbModed::currentMode() const
{
    return iPrivate->iCurrentMode;
}

QString QUsbModed::configMode() const
{
    return iPrivate->iConfigMode;
}

bool QUsbModed::setCurrentMode(QString aMode)
{
    if (iPrivate->iCurrentMode != aMode) {
        iPrivate->iCurrentMode = aMode;
        Q_EMIT currentModeChanged();
    }
    return true;
}

bool QUsbModed::setConfigMode(QString aMode)
{
    if (iPrivate->iConfigMode != aMode) {
        iPrivate->iConfigMode = aMode;
        Q_EMIT configModeChanged();
    }
    return true;
}

void QUsbModed::onServiceRegistered(QString)
{
}

void QUsbModed::onServiceUnregistered(QString)
{
}

void QUsbModed::onGetModesFinished(QDBusPendingCallWatcher*)
{
}

void QUsbModed::onGetConfigFinished(QDBusPendingCallWatcher*)
{
}

void QUsbModed::onGetModeRequestFinished(QDBusPendingCallWatcher*)
{
}

void QUsbModed::updateSupportedModes(QString)
{
}

void QUsbModed::setupCallFinished(int)
{
}

void QUsbModed::onSetModeFinished(QDBusPendingCallWatcher*)
{
}

void QUsbModed::onSetConfigFinished(QDBusPendingCallWatcher*)
{
}

void QUsbModed::onUsbConfigChanged(QString, QString, QString)
{
}

void QUsbModed::onUsbStateChanged(QString)
{
}

void QUsbModed::onUsbSupportedModesChanged(QString)
{
}
