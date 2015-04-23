/*!
 * @file qmusbmode.h
 * @brief Contains QmUSBMode

   <p>
   @copyright (C) 2009-2011 Nokia Corporation
   @license LGPL Lesser General Public License

   @author Timo Olkkonen <ext-timo.p.olkkonen@nokia.com>
   @author Matias Muhonen <ext-matias.muhonen@nokia.com>

   @scope Internal

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
#ifndef QMUSBMODE_H
#define QMUSBMODE_H

#include "system_global.h"
#include <QObject>

QT_BEGIN_HEADER

namespace MeeGo
{

class QmUSBModePrivate;

/*!
 * @scope Internal
 * @brief Provides a way to set and get the mode of the USB (Universal Serial Bus) on the device.
 *
 * When the USB state is changed, the modeChanged signal is emitted.
 * First, when the USB cable is plugged in, the QmUSBMode::Connected mode is signaled.
 * If there is a default USB mode other than QmUSBMode::Ask set, it will be the next USB mode.
 * Otherwise, QmUSBMode::ModeRequest is emitted, for which one should respond by calling setMode(QmUSBMode::Mode).
 * If no USB mode is set in 30 seconds, the USB mode will be set to QmUSBMode::ChargingOnly.
 *
 * The USB state can be also set by calling setMode(QmUSBMode::Mode).
 *
 * Typically, the default mode is either set to QmUSBMode::OviSuite or QmUSBMode::MassStorage, which is then
 * used when the USB cable is plugged in. The other option is to set the default mode to QmUSBMode::Ask,
 * listen for the QmUSBMode::ModeRequest mode, and then set the QmUSBMode::MassStorage or QmUSBMode::OviSuite mode.
 *
 * Switching the USB mode from QmUSBMode::MassStorage to QmUSBMode::OviSuite (and vice versa) is not supported
 * without re-plugging the USB cable.
 */
class QmUSBMode : public QObject
{
    Q_OBJECT

public:
    QmUSBMode(QObject *parent = 0);
    ~QmUSBMode();

    /*!
     * @brief The mode of the USB (Universal Serial Bus) on the device.
     */
    enum Mode {
        Connected = 0,    //!< Signaled when the USB cable is plugged in.
        DataInUse,        //!< Signaled when the mass storage is in use and the chosen USB mode can cause data loss, mode change denied.
        Disconnected,     //!< Signaled when the USB cable is unplugged.
        MassStorage,      //!< Mass storage mode.
        ChargingOnly,     //!< Charging only mode.
        OviSuite,         //!< Ovi suite mode. Requires an MTP (media transfer protocol) client on the USB host.
        ModeRequest,      //!< Signaled as an indication that the USB mode should be set with setMode(QmUSBMode::Mode).
        Ask,              //!< Waiting for the USB mode to be set with setMode(QmUSBMode::Mode).
        Undefined,        //!< Unknown/error.
        SDK,              //!< Windows network mode. Allows USB networking on the device.
	Developer,	  //!< Developer mode. Allows USB networking
	MTP,		  //!< MTP mode. Allows for generic MTP
	Adb,		  //!< adb mode. Allows Android Debug Bridge
	Diag,		  //!< diag mode. Allows Qualcomm Diag
	ConnectionSharing, //!< Cellular connection sharing/tethering.
	Host,		  //!< host mode. Allows switching to host mode
        Charger,         //!<Dedicated charger is connected.
    };

    /*!
     * @brief Gets the list of supported USB modes.
     * @return The list of supported USB modes
     */
    QList<QmUSBMode::Mode> getSupportedModes();

    /*!
     * @brief Gets the current USB mode.
     * @return Returns the current USB mode
     */
    QmUSBMode::Mode getMode();

    /*!
     * @brief Sets the USB mode. Note that calling setMode is non-blocking, so the method returns immediately.
     * If the USB mode change succeeded, the modeChanged signal is emitted.
     * @param mode The USB mode to be set. The OviSuite, MassStorage, ChargingOnly and SDK modes can be requested.
     * @return True if a valid mode was requested, false otherwise
     */
    bool setMode(QmUSBMode::Mode mode);

    /*!
     * @brief Sets the default USB mode.
     * @param mode The new default USB mode. The OviSuite, MassStorage, ChargingOnly, Ask and SDK modes can be set
     * @return True if successful, false otherwise
     */
    bool setDefaultMode(QmUSBMode::Mode mode);

    /*!
     * @brief Gets the default USB mode.
     * @return The default USB mode
     */
    QmUSBMode::Mode getDefaultMode();

    /*!
     * @brief Mount path
     */
    enum MountPath {
        /*!
         * @brief Mount for user content.
         *
         * The document directory (/home/user/MyDocs) is available as a mount point
         * depending on the USB mode:
         * 1) If the USB cable has been unplugged, the document directory will be
         *    available regardless of the chosen USB mode.
         * 2) If the USB cable has been plugged in, the document directory will not be
         *    available in the mass storage mode.
         */
        DocumentDirectoryMount = 0
    };

    /*!
     * @brief Mount options
     */
    enum MountOption {
        ReadOnlyMount  =  0x0000001, //!< Read only.
        ReadWriteMount =  0x0000002  //!< Read/write.
    };
    Q_DECLARE_FLAGS(MountOptionFlags, MountOption)

    /*!
     * @brief Gets the status of a mount.
     * @return The mount status as MountOptionFlags
     */
    QmUSBMode::MountOptionFlags mountStatus(QmUSBMode::MountPath mountPath);

Q_SIGNALS:

    /*!
     * @brief This signal is emitted when the USB mode is changed.
     * The signal can be emitted in the following conditions:
     * \li The USB cable is plugged in/unplugged (QmUSBMode::Connected, QmUSBMode::Disconnected)
     * \li USB mode change was denied (QmUSBMode::DataInUse)
     * \li No default USB mode is set and a USB cable is connected (QmUSBMode::ModeRequest)
     * \li The USB mode was changed (QmUSBMode::MassStorage, QmUSBMode::ChargingOnly, QmUSBMode::OviSuite)
     * \li An error occurred changing the USB Mode (QmUSBMode::Undefined)
     * @param mode The current mode.
     */
    void modeChanged(MeeGo::QmUSBMode::Mode mode);

    /*!
     * @brief This signal is emitted before a file system is being unmounted.
     * Applications can use the signal as an indication that a certain mount path
    *  will be unavailable soon.
     * @param mountPath The mount which is going to be unmounted
     */
    void fileSystemWillUnmount(MeeGo::QmUSBMode::MountPath mountPath);

    /*!
     * @brief This signal is emitted if there was an error changing the USB mode.
     * @param errorCode an error code representing the error. The following error codes are supported:
     * \li \c qtn_usb_filessystem_inuse The file system is in use.
     * \li \c mount_failed Mounting a file system failed.
     */
    void error(const QString &errorCode);

    /*!
     * @brief This signal is emitted when the supported USB modes have changed.
     * @param supportedModes The currently supported modes.
     */
    void supportedModesChanged(QList<MeeGo::QmUSBMode::Mode> supportedModes);

protected:
    void connectNotify(const QMetaMethod &signal);
    void disconnectNotify(const QMetaMethod &signal);

private:
    Q_DISABLE_COPY(QmUSBMode);
    MEEGO_DECLARE_PRIVATE(QmUSBMode);
};

}

QT_END_HEADER

#endif // QMUSBMODE_H
