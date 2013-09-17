/***************************************************************************
**
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

#ifndef VOLUMECONTROL_H
#define VOLUMECONTROL_H

#include <QTimer>
#include <QObject>
#include "lipstickglobal.h"

class HomeWindow;
class PulseAudioControl;
class VolumeKeyListener;
class MGConfItem;

namespace ResourcePolicy {
    class ResourceSet;
}

/*!
 * \class VolumeControl
 *
 * \brief Shows a window for displaying the volume level.
 *
 * Creates a transparent window which can be used to show
 * the current volume level.
 */
class LIPSTICK_EXPORT VolumeControl : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int volume READ volume NOTIFY volumeChanged)
    Q_PROPERTY(int maximumVolume READ maximumVolume NOTIFY maximumVolumeChanged)
    Q_PROPERTY(bool windowVisible READ windowVisible WRITE setWindowVisible NOTIFY windowVisibleChanged)
    Q_PROPERTY(bool showVolumeWarning READ showVolumeWarning WRITE setShowVolumeWarning NOTIFY showVolumeWarningChanged)

public:
    /*!
     * Creates a volume controller.
     *
     * \param parent the parent object
     */
    explicit VolumeControl(QObject *parent = 0);

    /*!
     * Destroys the volume controller.
     */
    virtual ~VolumeControl();

    /*!
     * Returns the current mode.
     *
     * \return the current mode
     */
    QString volumeMode() const;

    /*!
     * Returns the current volume.
     *
     * \return the current volume
     */
    int volume() const;

    /*!
     * Returns the maximum volume.
     *
     * \return the maximum volume
     */
    int maximumVolume() const;

    /*!
     * Returns whether the volume window is visible or not.
     *
     * \return \c true if the volume window is visible, \c false otherwise
     */
    bool windowVisible() const;

    /*!
     * Sets the visibility of the volume window.
     *
     * \param visible \c true if the volume window should be visible, \c false otherwise
     */
    void setWindowVisible(bool visible);

    /*!
     * Returns whether the high volume should be show to user.
     *
     * \return \c true if warning should be shown \c false otherwise
     */
    bool showVolumeWarning() const;

    /*!
     * Sets the visibility of the high volume warning.
     *
     * \param showWarning \c true if the warning should be shown, \c false is user want's to suppress it for ever
     */
    void setShowVolumeWarning(bool showWarning);

    //! \reimp
    virtual bool eventFilter(QObject *watched, QEvent *event);
    //! \reimp_end

signals:
    //! Sent when the mode has changed.
    void volumeModeChanged();

    //! Sent when the volume has changed.
    void volumeChanged();

    //! Sent when the maximum volume has changed.
    void maximumVolumeChanged();

    //! Sent when the visibility of the volume window has changed.
    void windowVisibleChanged();

    //! Sent when the visibility of the high volume warning.
    void showVolumeWarningChanged();

    /*!
     * Sent when main volume is set to so high that it can hurt hearing
     *
     * \param safeLevel Highest level for volumume that does not risk hurting hearing
     */
    void highVolume(int safeLevel);

    /*!
     * Sent when user needs to be warned about long listening time.
     *
     * \param listeningTime listening time in minutes
     */
    void longListeningTime(int listeningTime);

public slots:
    //! Acquires access to hardware keys
    void acquireKeys();

    //! Releases access to hardware keys
    void releaseKeys();

private slots:
    //! Sets the volume and maximum volume
    void setVolume(int volume, int maximumVolume);

    //! An internal slot to handle the case when we got the hardware volume keys resource
    void hwKeyResourceAcquired();

    //! An internal slot to handle the case when we lost the hardware volume keys resource
    void hwKeyResourceLost();

    //! Changes the current volume by the amount set in volumeChange
    void changeVolume();

    //! Stops any key repeat in progress
    void stopKeyRepeat();

private:
    //! The volume control window
    HomeWindow *window;

    //! PulseAudio volume controller
    PulseAudioControl *pulseAudioControl;

    //! A resource object for access to the volume keys
    ResourcePolicy::ResourceSet *hwKeyResource;

    //! Whether to react to volume key presses
    bool hwKeysAcquired;

    //! The current volume
    int volume_;

    //! The maximum volume
    int maximumVolume_;

    //! Volume change executed when calling changeVolume()
    int volumeChange;

    //! Timer for differentiating between a key release and a repress
    QTimer keyReleaseTimer;

    //! Timer for the key repeat delay
    QTimer keyRepeatDelayTimer;

    //! Timer for the key repeat
    QTimer keyRepeatTimer;

    //! Stores high volume warning visibility
    MGConfItem *volumeWarning;

#ifdef UNIT_TEST
    friend class Ut_VolumeControl;
#endif
};

#endif // VOLUMECONTROL_H
