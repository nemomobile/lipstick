/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of mhome.
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


#ifndef STATUSINDICATORMODEL_H_
#define STATUSINDICATORMODEL_H_

#include <MWidgetModel>
#include <QVariant>

class StatusIndicatorModel : public MWidgetModel
{
    Q_OBJECT
    M_MODEL(StatusIndicatorModel)
    M_MODEL_PROPERTY(QVariant, value, Value, true, QVariant())
    M_MODEL_PROPERTY(bool, animate, Animate, true, false)
};

class ProfileStatusIndicatorModel : public StatusIndicatorModel
{
    M_MODEL(ProfileStatusIndicatorModel)
};

class PhoneNetworkSignalStrengthStatusIndicatorModel : public StatusIndicatorModel
{
    M_MODEL(PhoneNetworkSignalStrengthStatusIndicatorModel)
};

class PhoneNetworkStatusIndicatorModel : public StatusIndicatorModel
{
    M_MODEL(PhoneNetworkStatusIndicatorModel)
};

class BatteryStatusIndicatorModel : public StatusIndicatorModel
{
    M_MODEL(BatteryStatusIndicatorModel)
};

class ClockAlarmStatusIndicatorModel : public StatusIndicatorModel
{
    M_MODEL(ClockAlarmStatusIndicatorModel)
};

class BluetoothStatusIndicatorModel : public StatusIndicatorModel
{
    M_MODEL(BluetoothStatusIndicatorModel)
};

class InternetConnectionStatusIndicatorModel : public StatusIndicatorModel
{
    M_MODEL(InternetConnectionStatusIndicatorModel)
};

class GPSStatusIndicatorModel : public StatusIndicatorModel
{
    M_MODEL(GPSStatusIndicatorModel)
};

#endif /* STATUSINDICATORMODEL_H_ */
