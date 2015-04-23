/*!
 * @file qmdisplaystate.cpp
 * @brief QmDisplayState

   <p>
   Copyright (C) 2009-2011 Nokia Corporation

   @author Antonio Aloisio <antonio.aloisio@nokia.com>
   @author Ilya Dogolazky <ilya.dogolazky@nokia.com>
   @author Timo Olkkonen <ext-timo.p.olkkonen@nokia.com>
   @author Ustun Ergenoglu <ext-ustun.ergenoglu@nokia.com>
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
#include "qmdisplaystate.h"
#include "qmdisplaystate_p.h"

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusReply>
#include <QMetaMethod>

namespace MeeGo {

QmDisplayState::QmDisplayState(QObject *parent)
              : QObject(parent) {
     MEEGO_INITIALIZE(QmDisplayState);

     connect(priv, SIGNAL(displayStateChanged(MeeGo::QmDisplayState::DisplayState)),
             this, SIGNAL(displayStateChanged(MeeGo::QmDisplayState::DisplayState)));
}

QmDisplayState::~QmDisplayState() {
    MEEGO_PRIVATE(QmDisplayState)

    disconnect(priv, SIGNAL(displayStateChanged(MeeGo::QmDisplayState::DisplayState)),
               this, SIGNAL(displayStateChanged(MeeGo::QmDisplayState::DisplayState)));

    MEEGO_UNINITIALIZE(QmDisplayState);
}

void QmDisplayState::connectNotify(const QMetaMethod &signal) {
    MEEGO_PRIVATE(QmDisplayState)

    /* QObject::connect() needs to be thread-safe */
    QMutexLocker locker(&priv->connectMutex);

    if (signal == QMetaMethod::fromSignal(&QmDisplayState::displayStateChanged)) {
        if (0 == priv->connectCount[SIGNAL_DISPLAY_STATE]) {
            QDBusConnection::systemBus().connect(MCE_SERVICE,
                                                 MCE_SIGNAL_PATH,
                                                 MCE_SIGNAL_IF,
                                                 MCE_DISPLAY_SIG,
                                                 priv,
                                                 SLOT(slotDisplayStateChanged(const QString&)));
        }
        priv->connectCount[SIGNAL_DISPLAY_STATE]++;
    }
}

void QmDisplayState::disconnectNotify(const QMetaMethod &signal) {
    MEEGO_PRIVATE(QmDisplayState)

    /* QObject::disconnect() needs to be thread-safe */
    QMutexLocker locker(&priv->connectMutex);

    if (signal == QMetaMethod::fromSignal(&QmDisplayState::displayStateChanged)) {
        priv->connectCount[SIGNAL_DISPLAY_STATE]--;

        if (0 == priv->connectCount[SIGNAL_DISPLAY_STATE]) {
            QDBusConnection::systemBus().disconnect(MCE_SERVICE,
                                                    MCE_SIGNAL_PATH,
                                                    MCE_SIGNAL_IF,
                                                    MCE_DISPLAY_SIG,
                                                    priv,
                                                    SLOT(slotDisplayStateChanged(const QString&)));
        }
    }
}

QmDisplayState::DisplayState QmDisplayState::get() const {
    QmDisplayState::DisplayState state = Unknown;
    QDBusReply<QString> displayStateReply = QDBusConnection::systemBus().call(
                                                QDBusMessage::createMethodCall(MCE_SERVICE, MCE_REQUEST_PATH, MCE_REQUEST_IF,
                                                                               MCE_DISPLAY_STATUS_GET));
    if (!displayStateReply.isValid()) {
        return state;
    }

    QString stateStr = displayStateReply.value();

    if (stateStr == MCE_DISPLAY_DIM_STRING) {
        state = Dimmed;
    } else if (stateStr == MCE_DISPLAY_ON_STRING) {
        state = On;
    } else if (stateStr == MCE_DISPLAY_OFF_STRING) {
        state = Off;
    }
    return state;
}

bool QmDisplayState::set(QmDisplayState::DisplayState state) {
    QString method;

    switch (state) {
        case Off:
            method = QString(MCE_DISPLAY_OFF_REQ);
            break;
        case Dimmed:
            method = QString(MCE_DISPLAY_DIM_REQ);
            break;
        case On:
            method = QString(MCE_DISPLAY_ON_REQ);
            break;
        default:
            return false;
    }

    QDBusMessage displayStateSetCall = QDBusMessage::createMethodCall(MCE_SERVICE, MCE_REQUEST_PATH, MCE_REQUEST_IF, method);
    (void)QDBusConnection::systemBus().call(displayStateSetCall, QDBus::NoBlock);
    return true;
}

int QmDisplayState::getMaxDisplayBrightnessValue() {

    int ret = -1;

    QList<QVariant> argumentList;
    argumentList << QVariant::fromValue(QDBusObjectPath(MAX_BRIGHTNESS_KEY));

    QDBusMessage getConfig = QDBusMessage::createMethodCall(MCE_SERVICE,
                                                            MCE_REQUEST_PATH,
                                                            MCE_REQUEST_IF,
                                                            MCE_GET_CONFIG);
    getConfig.setArguments(argumentList);

    QDBusReply<QDBusVariant> getMaxDisplayBrightnessValueReply = QDBusConnection::systemBus().call(getConfig);

    if (getMaxDisplayBrightnessValueReply.isValid())
        ret = getMaxDisplayBrightnessValueReply.value().variant().toInt();

    return ret;
}

int QmDisplayState::getDisplayBrightnessValue() {

    int ret = -1;

    QList<QVariant> argumentList;
    argumentList << QVariant::fromValue(QDBusObjectPath(BRIGHTNESS_KEY));

    QDBusMessage getConfig = QDBusMessage::createMethodCall(MCE_SERVICE,
                                                            MCE_REQUEST_PATH,
                                                            MCE_REQUEST_IF,
                                                            MCE_GET_CONFIG);
    getConfig.setArguments(argumentList);

    QDBusReply<QDBusVariant> getDisplayBrightnessValueReply = QDBusConnection::systemBus().call(getConfig);

    if (getDisplayBrightnessValueReply.isValid())
        ret = getDisplayBrightnessValueReply.value().variant().toInt();

    return ret;
}

int QmDisplayState::getDisplayBlankTimeout() {

    int ret = -1;

    QList<QVariant> argumentList;
    argumentList << QVariant::fromValue(QDBusObjectPath(BLANK_TIMEOUT_KEY));

    QDBusMessage getConfig = QDBusMessage::createMethodCall(MCE_SERVICE,
                                                            MCE_REQUEST_PATH,
                                                            MCE_REQUEST_IF,
                                                            MCE_GET_CONFIG);
    getConfig.setArguments(argumentList);

    QDBusReply<QDBusVariant> getDisplayBlankTimeoutReply = QDBusConnection::systemBus().call(getConfig);

    if (getDisplayBlankTimeoutReply.isValid())
        ret = getDisplayBlankTimeoutReply.value().variant().toInt();

    return ret;
}

int QmDisplayState::getDisplayDimTimeout() {

    int ret = -1;

    QList<QVariant> argumentList;
    argumentList << QVariant::fromValue(QDBusObjectPath(DIM_TIMEOUT_KEY));

    QDBusMessage getConfig = QDBusMessage::createMethodCall(MCE_SERVICE,
                                                            MCE_REQUEST_PATH,
                                                            MCE_REQUEST_IF,
                                                            MCE_GET_CONFIG);
    getConfig.setArguments(argumentList);

    QDBusReply<QDBusVariant> getDisplayDimTimeoutReply = QDBusConnection::systemBus().call(getConfig);

    if (getDisplayDimTimeoutReply.isValid())
        ret = getDisplayDimTimeoutReply.value().variant().toInt();

    return ret;
}

bool QmDisplayState::getBlankingWhenCharging() {

    int val = -1;

    QList<QVariant> argumentList;
    argumentList << QVariant::fromValue(QDBusObjectPath(BLANKING_CHARGING_KEY));

    QDBusMessage getConfig = QDBusMessage::createMethodCall(MCE_SERVICE,
                                                            MCE_REQUEST_PATH,
                                                            MCE_REQUEST_IF,
                                                            MCE_GET_CONFIG);
    getConfig.setArguments(argumentList);

    QDBusReply<QDBusVariant> getDisplayDimTimeoutReply = QDBusConnection::systemBus().call(getConfig);

    if (getDisplayDimTimeoutReply.isValid())
        val = getDisplayDimTimeoutReply.value().variant().toInt();

    // check if blanking is not inhibited during charging.
    bool ret = (val == 0);
    return ret;
}

void QmDisplayState::setDisplayBrightnessValue(int brightness) {

    if ((1 > brightness) || (brightness > getMaxDisplayBrightnessValue())) {
        return;
    }

    QList<QVariant> argumentList;
    argumentList << QVariant::fromValue(QDBusObjectPath(BRIGHTNESS_KEY));
    argumentList << QVariant::fromValue(QDBusVariant(brightness));

    QDBusMessage setConfig = QDBusMessage::createMethodCall(MCE_SERVICE,
                                                            MCE_REQUEST_PATH,
                                                            MCE_REQUEST_IF,
                                                            MCE_SET_CONFIG);
    setConfig.setArguments(argumentList);

    (void)QDBusConnection::systemBus().call(setConfig, QDBus::NoBlock);
}

void QmDisplayState::setDisplayBlankTimeout(int timeout) {

    // Get the list of possible values for blanking timeout.
    QList<QVariant> getArgumentList;
    getArgumentList << QVariant::fromValue(QDBusObjectPath(POSSIBLE_BLANK_LIST_KEY));

    QDBusMessage getConfig = QDBusMessage::createMethodCall(MCE_SERVICE,
                                                            MCE_REQUEST_PATH,
                                                            MCE_REQUEST_IF,
                                                            MCE_GET_CONFIG);
    getConfig.setArguments(getArgumentList);

    QDBusReply<QDBusVariant> getPossibleDisplayBlankTimeoutsReply = QDBusConnection::systemBus().call(getConfig);

    // Check if the timeout value is in the list of possible values.
    if (getPossibleDisplayBlankTimeoutsReply.isValid()) {
        const QDBusArgument possibleValues = getPossibleDisplayBlankTimeoutsReply.value().variant().value<QDBusArgument>();
        if (possibleValues.currentType() == QDBusArgument::ArrayType) {
            possibleValues.beginArray();
            int value;
            while (!possibleValues.atEnd()) {
                possibleValues >> value;
                if (value == timeout) {
                    // Set the timeout value.
                    QList<QVariant> argumentList;
                    argumentList << QVariant::fromValue(QDBusObjectPath(BLANK_TIMEOUT_KEY));
                    argumentList << QVariant::fromValue(QDBusVariant(timeout));

                    QDBusMessage setConfig = QDBusMessage::createMethodCall(MCE_SERVICE,
                                                                            MCE_REQUEST_PATH,
                                                                            MCE_REQUEST_IF,
                                                                            MCE_SET_CONFIG);
                    setConfig.setArguments(argumentList);

                    (void)QDBusConnection::systemBus().call(setConfig, QDBus::NoBlock);
                }
            }
            possibleValues.endArray();
        }
    }
}

void QmDisplayState::setDisplayDimTimeout(int timeout) {

    // Get the list of possible values for dimming timeout.
    QList<QVariant> getArgumentList;
    getArgumentList << QVariant::fromValue(QDBusObjectPath(POSSIBLE_DIM_LIST_KEY));

    QDBusMessage getConfig = QDBusMessage::createMethodCall(MCE_SERVICE,
                                                            MCE_REQUEST_PATH,
                                                            MCE_REQUEST_IF,
                                                            MCE_GET_CONFIG);
    getConfig.setArguments(getArgumentList);

    QDBusReply<QDBusVariant> getPossibleDisplayDimTimeoutsReply = QDBusConnection::systemBus().call(getConfig);

    // Check if the timeout value is in the list of possible values.
    if (getPossibleDisplayDimTimeoutsReply.isValid()) {
        const QDBusArgument possibleValues = getPossibleDisplayDimTimeoutsReply.value().variant().value<QDBusArgument>();
        if (possibleValues.currentType() == QDBusArgument::ArrayType) {
            possibleValues.beginArray();
            int value;
            while (!possibleValues.atEnd()) {
                possibleValues >> value;
                if (value == timeout) {
                    // Set the timeout value.
                    QList<QVariant> argumentList;
                    argumentList << QVariant::fromValue(QDBusObjectPath(DIM_TIMEOUT_KEY));
                    argumentList << QVariant::fromValue(QDBusVariant(timeout));

                    QDBusMessage setConfig = QDBusMessage::createMethodCall(MCE_SERVICE,
                                                                            MCE_REQUEST_PATH,
                                                                            MCE_REQUEST_IF,
                                                                            MCE_SET_CONFIG);
                    setConfig.setArguments(argumentList);

                    (void)QDBusConnection::systemBus().call(setConfig, QDBus::NoBlock);
                }
            }
            possibleValues.endArray();
        }
    }
}

void QmDisplayState::setBlankingWhenCharging(bool blanking) {

    int b = (blanking ? 0 : 1);

    QList<QVariant> argumentList;
    argumentList << QVariant::fromValue(QDBusObjectPath(BLANKING_CHARGING_KEY));
    argumentList << QVariant::fromValue(QDBusVariant(b));

    QDBusMessage setConfig = QDBusMessage::createMethodCall(MCE_SERVICE,
                                                            MCE_REQUEST_PATH,
                                                            MCE_REQUEST_IF,
                                                            MCE_SET_CONFIG);
    setConfig.setArguments(argumentList);

    (void)QDBusConnection::systemBus().call(setConfig, QDBus::NoBlock);
}

bool QmDisplayState::setBlankingPause(void) {
    QDBusMessage blankingPauseCall = QDBusMessage::createMethodCall(MCE_SERVICE, MCE_REQUEST_PATH, MCE_REQUEST_IF,
                                                                    MCE_PREVENT_BLANK_REQ);
    (void)QDBusConnection::systemBus().call(blankingPauseCall, QDBus::NoBlock);
    return true;
}

bool QmDisplayState::cancelBlankingPause(void) {
    QDBusMessage cancelBlankingPauseCall = QDBusMessage::createMethodCall(MCE_SERVICE, MCE_REQUEST_PATH, MCE_REQUEST_IF,
                                                                          MCE_CANCEL_PREVENT_BLANK_REQ);
    (void)QDBusConnection::systemBus().call(cancelBlankingPauseCall, QDBus::NoBlock);
    return true;
}

} //MeeGo namespace
