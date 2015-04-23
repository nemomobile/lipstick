/*!
 * @file qmsystemstate.h
 * @brief Contains QmSystemState which provides information and actions on device state.

   <p>
   @copyright (C) 2009-2011 Nokia Corporation
   @license LGPL Lesser General Public License

   @author Antonio Aloisio <antonio.aloisio@nokia.com>
   @author Ilya Dogolazky <ilya.dogolazky@nokia.com>
   @author Raimo Vuonnala <raimo.vuonnala@nokia.com>
   @author Timo Olkkonen <ext-timo.p.olkkonen@nokia.com>
   @author Timo Rongas <ext-timo.rongas.nokia.com>
   @author Tuomo Tanskanen <ext-tuomo.1.tanskanen@nokia.com>
   @author Simo Piiroinen <simo.piiroinen@nokia.com>
   @author Matias Muhonen <ext-matias.muhonen@nokia.com>

   @scope Nokia Meego

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
#ifndef QMSYSTEMSTATE_H
#define QMSYSTEMSTATE_H

#include "system_global.h"
#include <QtCore/qobject.h>

QT_BEGIN_HEADER

namespace MeeGo {

class QmSystemStatePrivate;

/*!
 * @scope Nokia Meego
 *
 * @class QmSystemState
 * @brief QmSystemState provides information and actions on device state.
 */
class MEEGO_SYSTEM_EXPORT QmSystemState : public QObject
{
    Q_OBJECT
    Q_ENUMS(NextState)
    Q_ENUMS(StateIndication)
    Q_ENUMS(RunState)
    Q_ENUMS(BootReason)

public:
    //! States the user can set
    enum NextState
    {
        Powerup = 0,        //!< Powerup from acting dead
        Reboot,             //!< Reboot
        ShuttingDown        //!< Shutdown
    };

    //! State indication
    enum StateIndication
    {
        Shutdown = 0, 		//!< Normal shutdown
        ThermalStateFatal,	//!< Shutdown due to thermal state
        BatteryStateEmpty,	//!< Shutdown due battery empty within few seconds
        SaveData,           //!< Save data
        RebootDeniedUSB,    //!< Reboot denied because USB is connected in mass storage mode
        ShutdownDeniedUSB   //!< Shutdown denied because USB is connected in mass storage mode
    };

    enum RunState
    {
        RunState_User = 0,
        RunState_ActDead,
        RunState_Test,       //!< Similar to Local, but CMT tests are run differently
        RunState_Local,      //!< Used when the device is mounted to a jig for R&D purposes
        RunState_Malf,
        RunState_Flash,
        RunState_Shutdown,
        RunState_Unknown
    };

    enum BootReason
    {
        BootReason_Unknown = -1,
        BootReason_SwdgTimeout = 0,    //!< Security watchdog timeout
        BootReason_SecViolation,       //!< Security violation
        BootReason_Wdg32kTimeout,      //!< 32k watchdog timeout
        BootReason_PowerOnReset,       //!< Power on reset issued by the HW
        BootReason_PowerKey,           //!< Power key pressed
        BootReason_MBus,               //!< MBus
        BootReason_Charger,            //!< Charger plugged in
        BootReason_Usb,                //!< Usb charger plugged in
        BootReason_SWReset,            //!< SW reset issued by the system
        BootReason_RTCAlarm,           //!< Real Time Clock Alarm
        BootReason_NSU                 //!< Software update
    };

public:
    /*!
     * @brief Constructor
     * @param parent The possible parent object
     */
    QmSystemState(QObject *parent = 0);
    ~QmSystemState();

    /*!
     * @brief Requests the system to move into state @c nextState.
     * @description The system state change may not actually change when the function returns;
     *              use the systemStateChanged(MeeGo::QmSystemState::StateIndication) signal
     *              to track the system state changes.
     * @credential dsme::DeviceStateControl Resource token required to set the device state (shutdown/reboot/powerup).
     * @param nextState New state
     * @return True if a valid state was requested, false otherwise
     */
    bool set(NextState nextState);

    /*!
     * @brief Gets the current run state.
     * @return The current run state
     */
    RunState getRunState();

    /*!
     * @brief Gets the current boot reason.
     * @return The current boot reason
     */
    BootReason getBootReason();

    /*!
     * @brief Gets the accumulated value of the power on counter in seconds, which represents the total time the device has been powered on.
     * @return The total time in seconds the device has been powered on
     */
    unsigned int getPowerOnTimeInSeconds();

Q_SIGNALS:
    /*!
     * @brief Sent when device state indication has been received.
     * @param what Received state indication type
     */
    void systemStateChanged(MeeGo::QmSystemState::StateIndication what);

protected:
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    void connectNotify(const QMetaMethod &signal);
    void disconnectNotify(const QMetaMethod &signal);
#else
    void connectNotify(const char *signal);
    void disconnectNotify(const char *signal);
#endif

private:
    Q_DISABLE_COPY(QmSystemState)
    MEEGO_DECLARE_PRIVATE(QmSystemState)
};

} // MeeGo namespace

QT_END_HEADER

#endif // QMSYSTEMSTATE_H

// End of file

