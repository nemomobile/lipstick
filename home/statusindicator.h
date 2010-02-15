/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of duihome.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#ifndef STATUSINDICATOR_H
#define STATUSINDICATOR_H

#include <DuiWidgetController>
#include <QSharedPointer>
#include "statusindicatormodel.h"

class ApplicationContext;
class ContextItem;

/*!
 * StatusIndicator is a widget for displaying either image or text based status
 * indicators in the status indicator area of the home screen.
 */
class StatusIndicator : public DuiWidgetController
{
    Q_OBJECT
    DUI_CONTROLLER(StatusIndicator)

    Q_PROPERTY(QVariant value READ value WRITE setValue)

public:
    /*!
     * Constructs a StatusIndicator.
     *
     * \param parent parent DuiWidget
     */
    explicit StatusIndicator(DuiWidget *parent = NULL);

    /*!
     * \brief Destructor
     */
    virtual ~StatusIndicator();

protected:
    /*!
     * Sets the value of the indicator. This will be used by the dynamic
     * indicators to change their appearance based on some value. The type of the value
     * is determined by the indicator type. For indicators that display
     * an image from a list of images, the value is a float 0 to 1 which
     * is used to determine the used image. For indicators that display text,
     * the value is the string to display.
     */
    void setValue(QVariant v);

    /*!
     * Gets the value of the indicator. \see setValue
     */
    QVariant value() const;

    //! Updates the animation status of the views
    void updateAnimationStatus();

    //! Whether or not the view should animate if possible
    bool animateIfPossible;

private:
    //! \reimp
    virtual void enterDisplayEvent();
    virtual void exitDisplayEvent();
    //! \reimp_end

    /*!
     * Enables or disables model updates.
     *
     * \param modelUpdatesEnabled \c true if the model should be updated, \c false otherwise
     */
    void setModelUpdatesEnabled(bool modelUpdatesEnabled);

    //! Whether the model should be updated or not
    bool modelUpdatesEnabled;

    //! The current value of the status indicator
    QVariant currentValue;
};

/*!
 * A status indicator for showing the phone network signal strength
 */
class PhoneNetworkSignalStrengthStatusIndicator : public StatusIndicator
{
    Q_OBJECT
    DUI_CONTROLLER(PhoneNetworkSignalStrengthStatusIndicator)

public:
    /*!
     * Constructs a PhoneNetworkSignalStrengthStatusIndicator.
     *
     * \param context the application context to get status information from
     * \param parent parent DuiWidget
     */
    explicit PhoneNetworkSignalStrengthStatusIndicator(ApplicationContext &context, DuiWidget *parent = NULL);

    virtual ~PhoneNetworkSignalStrengthStatusIndicator();

public slots:
    void signalStrengthChanged();

private:
    ContextItem *signalStrength;
};

/*!
 * A status indicator for showing the battery charge level or
 * battery charging animation.
 */
class BatteryStatusIndicator : public StatusIndicator
{
    Q_OBJECT
    DUI_CONTROLLER(BatteryStatusIndicator)

public:
    /*!
     * Constructs a BatteryStatusIndicator.
     *
     * \param context the application context to get status information from
     * \param parent parent DuiWidget
     */
    explicit BatteryStatusIndicator(ApplicationContext &context, DuiWidget *parent = NULL);

    virtual ~BatteryStatusIndicator();

public slots:
    void batteryLevelChanged();
    void batteryChargingChanged();

private:
    ContextItem *batteryLevel;
    ContextItem *batteryCharging;
};

/*!
 * A status indicator for showing whether a clock alarm has been set.
 */
class ClockAlarmStatusIndicator : public StatusIndicator
{
    Q_OBJECT
    DUI_CONTROLLER(ClockAlarmStatusIndicator)

public:
    /*!
     * Constructs a ClockAlarmStatusIndicator.
     *
     * \param context the application context to get status information from
     * \param parent parent DuiWidget
     */
    explicit ClockAlarmStatusIndicator(ApplicationContext &context, DuiWidget *parent = NULL);

    virtual ~ClockAlarmStatusIndicator();

public slots:
    void clockAlarmChanged();

private:
    ContextItem *clockAlarm;
};

/*!
 * A status indicator for showing whether the bluetooth connection
 * is activated.
 */
class BluetoothStatusIndicator : public StatusIndicator
{
    Q_OBJECT
    DUI_CONTROLLER(BluetoothStatusIndicator)

public:
    /*!
     * Constructs a BluetoothStatusIndicator.
     *
     * \param context the application context to get status information from
     * \param parent parent DuiWidget
     */
    explicit BluetoothStatusIndicator(ApplicationContext &context,
                                      DuiWidget *parent = NULL);

    virtual ~BluetoothStatusIndicator();

public slots:
    void bluetoothChanged();

private:
    ContextItem *bluetooth;
};

/*!
 * A status indicator for showing the internet connection status.
 * Displays either a WiFi signal strength or a 3G symbol if 3G
 * connection is in use.
 */
class InternetConnectionStatusIndicator : public StatusIndicator
{
    Q_OBJECT
    DUI_CONTROLLER(InternetConnectionStatusIndicator)

public:
    explicit InternetConnectionStatusIndicator(ApplicationContext &context, DuiWidget *parent = NULL);

    virtual ~InternetConnectionStatusIndicator();

public slots:
    void wifiSignalStrengthChanged();
    void internetConnectionChanged();

private:
    ContextItem *internetConnection;
    ContextItem *wifiSignalStrength;
};


/*!
 * A status indicator for showing the used phone network name
 */
class PhoneNetworkStatusIndicator : public StatusIndicator
{
    Q_OBJECT
    DUI_CONTROLLER(PhoneNetworkStatusIndicator)

public:
    /*!
     * Constructs a PhoneNetworkStatusIndicator.
     *
     * \param context the application context to get status information from
     * \param parent parent DuiWidget. If parent is NULL, the creating client must take the responsibility to delete the indicator.
     */
    explicit PhoneNetworkStatusIndicator(ApplicationContext &context, DuiWidget *parent = NULL);

    virtual ~PhoneNetworkStatusIndicator();

public slots:
    void phoneNetworkChanged();

private:
    QSharedPointer<ContextItem> networkName;
};

#endif // STATUSINDICATOR_H
