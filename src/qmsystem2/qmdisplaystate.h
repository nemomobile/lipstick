/*!
 * @file qmdisplaystate.h
 * @brief Contains QmDisplayState which provides information and actions on device display state.

   <p>
   @copyright (C) 2009-2011 Nokia Corporation
   @license LGPL Lesser General Public License

   @author Antonio Aloisio <antonio.aloisio@nokia.com>
   @author Ilya Dogolazky <ilya.dogolazky@nokia.com>
   @author Raimo Vuonnala <raimo.vuonnala@nokia.com>
   @author Timo Olkkonen <ext-timo.p.olkkonen@nokia.com>
   @author Timo Rongas <ext-timo.rongas.nokia.com>
   @author Ustun Ergenoglu <ext-ustun.ergenoglu@nokia.com>

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
#ifndef QMDISPLAYSTATE_H
#define QMDISPLAYSTATE_H
#include "system_global.h"
#include <QtCore/qobject.h>

QT_BEGIN_HEADER

namespace MeeGo {

class QmDisplayStatePrivate;

/*!
 * @scope Nokia Meego
 *
 * @class QmDisplayState
 * @brief QmDisplayState Provides information and actions on device display state.
 */
class MEEGO_SYSTEM_EXPORT QmDisplayState : public QObject
{
    Q_OBJECT
    Q_ENUMS(DisplayState)
    Q_PROPERTY(DisplayState state READ get WRITE set)

public:
    //! Possible states for device display
    enum DisplayState
    {
        Off = -1,   //!< Display is off
        Dimmed = 0, //!< Display is dimmed
        On = 1,      //!< Display is on
        Unknown      //!< Display state is unknown
    };

public:
    /*!
     * Constructor
     * @param parent The parent object
     */
    QmDisplayState(QObject *parent = 0);
    ~QmDisplayState();

    /*!
     * @brief Gets the current display state
     * @return Current display state
     */
    DisplayState get() const;

    /*!
     * @brief Sets the current display state.
     * @param state Display state new set
     * @return True if a valid display state was requested, false otherwise
     */
    bool set(DisplayState state);

    /*!
     * @brief Requests not to blank the display. Must be repeated every 60 seconds to renew request.
     * Also prevents suspending.
     * @return False if a blanking pause could not be requested
     */
    bool setBlankingPause(void);

    /*!
     * @brief Cancels an earlier setBlankingPause() call.
     * @return False if a blanking pause cancellation could not be requested
     */
    bool cancelBlankingPause(void);

    /*!
     * @brief Gets the maximum brightness value that can be set.
     * @return The maximum brightness value. -1 is returned in case
     * of an error during retrieving value
     */
    int getMaxDisplayBrightnessValue();

    /*!
     * @brief Gets the current brightness values of the display.
     * @return The current brightness values of the display. -1, in case of
     * an error.
     */
    int getDisplayBrightnessValue();

    /*!
     * @brief Gets the current display blanking timeout in seconds.
     * @return The current display blanking timeout in second. -1, in case of
     * an error.
     */
    int getDisplayBlankTimeout();

    /*!
     * @brief Gets the current display dim timeout in seconds.
     * @return The current display dimming timeout in seconds. -1, in case of
     * an error.
     */
    int getDisplayDimTimeout();

    /*!
     * @brief Gets the blanking state when charging.
     * @return True if blanking is not inhibited during charging, false
     * otherwise.
     */
    bool getBlankingWhenCharging();

    /*!
     * @brief Sets the display brightness value.
     * @param brightness New brightness value to set. Must be between 1 and
     * #getMaxDisplayBrightnessValue() .
     */
    void setDisplayBrightnessValue(int brightness);

    /*!
     * @brief Sets the display blanking timeout.
     * @param timeout Timeout to set
     */
    void setDisplayBlankTimeout(int timeout);

    /*!
     * @brief Sets the display dimming timeout.
     * @param timeout Timeout to set
     */
    void setDisplayDimTimeout(int timeout);

    /*!
     * @brief Sets if the screen should be blanked during charging.
     * @param blanking True if screen should be blanked during charging,
     * #false otherwise
     */
    void setBlankingWhenCharging(bool blanking);

Q_SIGNALS:
    /*!
     * @brief Sent when display state has changed.
     * @param state Current display state
     */
    void displayStateChanged(MeeGo::QmDisplayState::DisplayState state);

protected:
    void connectNotify(const QMetaMethod &signal);
    void disconnectNotify(const QMetaMethod &signal);

private:
    Q_DISABLE_COPY(QmDisplayState)
    MEEGO_DECLARE_PRIVATE(QmDisplayState)
};

} //MeeGo namespace

QT_END_HEADER

#endif /* QMDISPLAYSTATE_H */

// End of file
