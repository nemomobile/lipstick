/*!
 * @file qmsystemstate.cpp
 * @brief QmSystemState

   <p>
   Copyright (C) 2009-2011 Nokia Corporation

   @author Antonio Aloisio <antonio.aloisio@nokia.com>
   @author Ilya Dogolazky <ilya.dogolazky@nokia.com>
   @author Timo Olkkonen <ext-timo.p.olkkonen@nokia.com>
   @author Tuomo Tanskanen <ext-tuomo.1.tanskanen@nokia.com>
   @author Simo Piiroinen <simo.piiroinen@nokia.com>
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
#include "qmsystemstate.h"
#include "qmsystemstate_p.h"

#include "qmsysteminformation.h"

#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusReply>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QMetaMethod>
#endif

#include <QFile>
#include <QTextStream>

#define RUNSTATE_FILE "/var/lib/dsme/saved_state"

#define RUNSTATE_LOCAL "LOCAL"
#define RUNSTATE_TEST "TEST"
#define RUNSTATE_MALF "MALF"
#define RUNSTATE_FLASH "FLASH"
#define RUNSTATE_USER "USER"
#define RUNSTATE_SHUTDOWN "SHUTDOWN"
#define RUNSTATE_ACT_DEAD "ACT_DEAD"

#define BOOT_REASON_UNKNOWN           "unknown"
#define BOOT_REASON_SWDG_TIMEOUT     "swdg_to"
#define BOOT_REASON_SEC_VIOLATION    "sec_vio"
#define BOOT_REASON_32K_WDG_TIMEOUT  "32wd_to"
#define BOOT_REASON_POWER_ON_RESET   "por"
#define BOOT_REASON_POWER_KEY        "pwr_key"
#define BOOT_REASON_MBUS             "mbus"
#define BOOT_REASON_CHARGER          "charger"
#define BOOT_REASON_USB              "usb"
#define BOOT_REASON_SW_RESET         "sw_rst"
#define BOOT_REASON_RTC_ALARM        "rtc_alarm"
#define BOOT_REASON_NSU              "nsu"

namespace MeeGo {

QmSystemState::QmSystemState(QObject *parent)
             : QObject(parent) {
    MEEGO_INITIALIZE(QmSystemState);

    connect(priv, SIGNAL(systemStateChanged(MeeGo::QmSystemState::StateIndication)),
            this, SIGNAL(systemStateChanged(MeeGo::QmSystemState::StateIndication)));
}

QmSystemState::~QmSystemState() {
    MEEGO_PRIVATE(QmSystemState)

    disconnect(priv, SIGNAL(systemStateChanged(MeeGo::QmSystemState::StateIndication)),
               this, SIGNAL(systemStateChanged(MeeGo::QmSystemState::StateIndication)));

    MEEGO_UNINITIALIZE(QmSystemState);
}

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
void QmSystemState::connectNotify(const QMetaMethod &signal) {
#else
void QmSystemState::connectNotify(const char *signal) {
#endif
    MEEGO_PRIVATE(QmSystemState)

    /* QObject::connect() needs to be thread-safe */
    QMutexLocker locker(&priv->connectMutex);

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    if (signal == QMetaMethod::fromSignal(&QmSystemState::systemStateChanged)) {
#else
    if (QLatin1String(signal) == QLatin1String(QMetaObject::normalizedSignature(SIGNAL(systemStateChanged(MeeGo::QmSystemState::StateIndication))))) {
#endif
        if (0 == priv->connectCount[SIGNAL_SYSTEM_STATE]) {
            QDBusConnection::systemBus().connect(dsme_service,
                                                 dsme_sig_path,
                                                 dsme_sig_interface,
                                                 dsme_shutdown_ind,
                                                 priv,
                                                 SLOT(emitShutdown()));
            QDBusConnection::systemBus().connect(dsme_service,
                                                 dsme_sig_path,
                                                 dsme_sig_interface,
                                                 dsme_save_unsaved_data_ind,
                                                 priv,
                                                 SLOT(emitSaveData()));
            QDBusConnection::systemBus().connect(dsme_service,
                                                 dsme_sig_path,
                                                 dsme_sig_interface,
                                                 dsme_battery_empty_ind,
                                                 priv,
                                                 SLOT(emitBatteryShutdown()));
            QDBusConnection::systemBus().connect(dsme_service,
                                                 dsme_sig_path,
                                                 dsme_sig_interface,
                                                 dsme_state_req_denied_ind,
                                                 priv,
                                                 SLOT(emitShutdownDenied(QString, QString)));
            QDBusConnection::systemBus().connect(SYS_THERMALMANAGER_SERVICE,
                                                 SYS_THERMALMANAGER_PATH,
                                                 SYS_THERMALMANAGER_INTERFACE,
                                                 SYS_THERMALMANAGER_STATE_SIG,
                                                 priv,
                                                 SLOT(emitThermalShutdown(QString)));
        }
        priv->connectCount[SIGNAL_SYSTEM_STATE]++;
    }
}

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
void QmSystemState::disconnectNotify(const QMetaMethod &signal) {
#else
void QmSystemState::disconnectNotify(const char *signal) {
#endif
    MEEGO_PRIVATE(QmSystemState)

    /* QObject::disconnect() needs to be thread-safe */
    QMutexLocker locker(&priv->connectMutex);

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    if (signal == QMetaMethod::fromSignal(&QmSystemState::systemStateChanged)) {
#else
    if (QLatin1String(signal) == QLatin1String(QMetaObject::normalizedSignature(SIGNAL(systemStateChanged(MeeGo::QmSystemState::StateIndication))))) {
#endif
        priv->connectCount[SIGNAL_SYSTEM_STATE]--;

        if (0 == priv->connectCount[SIGNAL_SYSTEM_STATE]) {
            QDBusConnection::systemBus().disconnect(dsme_service,
                                                    dsme_sig_path,
                                                    dsme_sig_interface,
                                                    dsme_shutdown_ind,
                                                    priv,
                                                    SLOT(emitShutdown()));
            QDBusConnection::systemBus().disconnect(dsme_service,
                                                    dsme_sig_path,
                                                    dsme_sig_interface,
                                                    dsme_save_unsaved_data_ind,
                                                    priv,
                                                    SLOT(emitSaveData()));
            QDBusConnection::systemBus().disconnect(dsme_service,
                                                    dsme_sig_path,
                                                    dsme_sig_interface,
                                                    dsme_battery_empty_ind,
                                                    priv,
                                                    SLOT(emitBatteryShutdown()));
            QDBusConnection::systemBus().disconnect(dsme_service,
                                                    dsme_sig_path,
                                                    dsme_sig_interface,
                                                    dsme_state_req_denied_ind,
                                                    priv,
                                                    SLOT(emitShutdownDenied(QString, QString)));
            QDBusConnection::systemBus().disconnect(SYS_THERMALMANAGER_SERVICE,
                                                    SYS_THERMALMANAGER_PATH,
                                                    SYS_THERMALMANAGER_INTERFACE,
                                                    SYS_THERMALMANAGER_STATE_SIG,
                                                    priv,
                                                    SLOT(emitThermalShutdown(QString)));
        }
    }
}

bool QmSystemState::set(NextState nextState) {
    MEEGO_PRIVATE(QmSystemState)

    QString method;
    switch (nextState) {
        case Powerup:
            method = QString(dsme_req_powerup);
            break;
        case Reboot:
            method = QString(dsme_req_reboot);
            break;
        case ShuttingDown:
            method = QString(dsme_req_shutdown);
            break;
        default:
            return false;
    }
    QmIPCInterface *requestIf = priv->dsmeRequestIf;
/*!
 * @file qmsystemstate_p.h
 * @brief Contains QmSystemStatePrivate

   <p>
   Copyright (C) 2009-2011 Nokia Corporation

   @author Timo Olkkonen <ext-timo.p.olkkonen@nokia.com>

   @scope Private

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
#ifndef QMSYSTEMSTATE_P_H
#define QMSYSTEMSTATE_P_H

#include "qmsystemstate.h"
#include "qmipcinterface_p.h"
#include "dsme/dsme_dbus_if.h"
#include "msystemdbus_p.h"

#include <QMutex>

#define SIGNAL_SYSTEM_STATE 0

namespace MeeGo
{

    class QmSystemStatePrivate : public QObject
    {
        Q_OBJECT
        MEEGO_DECLARE_PUBLIC(QmSystemState)

    public:
        QmSystemStatePrivate() {
            dsmeRequestIf = new QmIPCInterface(
                           dsme_service,
                           dsme_req_path,
                           dsme_req_interface);
            connectCount[SIGNAL_SYSTEM_STATE] = 0;
        }

        ~QmSystemStatePrivate() {
            if (dsmeRequestIf) {
                delete dsmeRequestIf, dsmeRequestIf = 0;
            }
        }

        QMutex connectMutex;
        size_t connectCount[1];
        QmIPCInterface *dsmeRequestIf;

    Q_SIGNALS:

        void systemStateChanged(MeeGo::QmSystemState::StateIndication what);

    private Q_SLOTS:

        void emitShutdown() {
            emit systemStateChanged(QmSystemState::Shutdown);
        }

        void emitThermalShutdown(QString thermalState) {
            // TODO: hardcoded "fatal"
            if (thermalState == "fatal") {
                emit systemStateChanged(QmSystemState::ThermalStateFatal);
            }
        }

        void emitBatteryShutdown() {
            emit systemStateChanged(QmSystemState::BatteryStateEmpty);
        }

        void emitSaveData() {
            emit systemStateChanged(QmSystemState::SaveData);
        }

        void emitShutdownDenied(QString reqType, QString reason) {
            if (reason == "usb") {
                if (reqType == "shutdown") {
                    emit systemStateChanged(QmSystemState::ShutdownDeniedUSB);
                } else if (reqType == "reboot") {
                    emit systemStateChanged(QmSystemState::RebootDeniedUSB);
                }
            }
        }
    };
}
#endif // QMSYSTEMSTATE_P_H
    requestIf->callAsynchronously(method);
    return true;
}

QmSystemState::RunState QmSystemState::getRunState() {
    QFile file(RUNSTATE_FILE);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        QString stateStr = in.readLine();
        if (stateStr == RUNSTATE_LOCAL) {
            return RunState_Local;
        } else if (stateStr == RUNSTATE_TEST) {
            return RunState_Test;
        } else if (stateStr == RUNSTATE_MALF) {
            return RunState_Malf;
        } else if (stateStr == RUNSTATE_FLASH) {
            return RunState_Flash;
        } else if (stateStr == RUNSTATE_USER) {
            return RunState_User;
        } else if (stateStr == RUNSTATE_SHUTDOWN) {
            return RunState_Shutdown;
        } else if (stateStr == RUNSTATE_ACT_DEAD) {
            return RunState_ActDead;
        }
    }

    return RunState_Unknown;
}

QmSystemState::BootReason QmSystemState::getBootReason() {
    QmSystemState::BootReason bootReason = BootReason_Unknown;

    QmSystemInformation systemInformation;
    QString reasonStr = systemInformation.valueForKey("/component/boot-reason");

    if (reasonStr == BOOT_REASON_SWDG_TIMEOUT) {
        bootReason = BootReason_SwdgTimeout;
    } else if (reasonStr == BOOT_REASON_SEC_VIOLATION) {
        bootReason = BootReason_SecViolation;
    } else if (reasonStr == BOOT_REASON_32K_WDG_TIMEOUT) {
        bootReason = BootReason_Wdg32kTimeout;
    } else if (reasonStr == BOOT_REASON_POWER_ON_RESET) {
        bootReason = BootReason_PowerOnReset;
    } else if (reasonStr == BOOT_REASON_POWER_KEY) {
        bootReason = BootReason_PowerKey;
    } else if (reasonStr == BOOT_REASON_MBUS) {
        bootReason = BootReason_MBus;
    } else if (reasonStr == BOOT_REASON_CHARGER) {
        bootReason = BootReason_Charger;
    } else if (reasonStr == BOOT_REASON_USB) {
        bootReason = BootReason_Usb;
    } else if (reasonStr == BOOT_REASON_SW_RESET) {
        bootReason = BootReason_SWReset;
    } else if (reasonStr == BOOT_REASON_RTC_ALARM) {
        bootReason = BootReason_RTCAlarm;
    } else if (reasonStr == BOOT_REASON_NSU) {
        bootReason = BootReason_NSU;
    }

    return bootReason;
}

unsigned int QmSystemState::getPowerOnTimeInSeconds() {
    unsigned int result = 0;
    QDBusReply<int> powerontimerReply = QDBusConnection::systemBus().call(
                                            QDBusMessage::createMethodCall(SYS_POWERONTIMER_SERVICE,
                                                                           SYS_POWERONTIMER_PATH,
                                                                           SYS_POWERONTIMER_INTERFACE,
                                                                           SYS_POWERONTIMER_TIME_GET));
    if (!powerontimerReply.isValid()) {
        return result;
    }

    result = powerontimerReply.value();
    return result;
}

} // MeeGo namespace
