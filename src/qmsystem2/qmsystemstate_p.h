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
