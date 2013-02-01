/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
#ifndef QMBATTERY_STUB
#define QMBATTERY_STUB

#include <qmbattery.h>
#include "stubbase.h"
/*
 * XXX: This stub contains only those methods which are 
 * used by the  BatteryBusinessLogic class ...
 */

/*
 * Declare stub
 */
class QmBatteryStub : public StubBase
{
public:
    virtual MeeGo::QmBattery::ChargerType getChargerType ();
    virtual MeeGo::QmBattery::ChargingState getChargingState ();
    virtual MeeGo::QmBattery::BatteryState getBatteryState ();
    virtual int getRemainingTalkTime (MeeGo::QmBattery::RemainingTimeMode mode);
    virtual int getRemainingIdleTime (MeeGo::QmBattery::RemainingTimeMode mode);
    virtual int getRemainingCapacityPct ();
    virtual int getMaxBars ();
    virtual MeeGo::QmBattery::State getState();
    virtual MeeGo::QmBattery::Level getLevel();
};

MeeGo::QmBattery::ChargingState
QmBatteryStub::getChargingState ()
{
    stubMethodEntered ("getChargingState");
    return stubReturnValue<MeeGo::QmBattery::ChargingState> ("getChargingState");
}

MeeGo::QmBattery::ChargerType
QmBatteryStub::getChargerType ()
{
    stubMethodEntered ("getChargerType");
    return stubReturnValue<MeeGo::QmBattery::ChargerType> ("getChargerType");
}


MeeGo::QmBattery::BatteryState
QmBatteryStub::getBatteryState ()
{
    stubMethodEntered ("getBatteryState");
    return stubReturnValue<MeeGo::QmBattery::BatteryState> ("getBatteryState");
}


int
QmBatteryStub::getRemainingTalkTime (MeeGo::QmBattery::RemainingTimeMode mode)
{
    QList<ParameterBase*> params;
    params.append (new Parameter<MeeGo::QmBattery::RemainingTimeMode> (mode));
    stubMethodEntered ("getRemainingTalkTime", params);
    return stubReturnValue<int> ("getRemainingTalkTime");
}

int
QmBatteryStub::getRemainingIdleTime (MeeGo::QmBattery::RemainingTimeMode mode)
{
    QList<ParameterBase*> params;
    params.append (new Parameter<MeeGo::QmBattery::RemainingTimeMode> (mode));
    stubMethodEntered ("getRemainingIdleTime", params);
    return stubReturnValue<int> ("getRemainingIdleTime");
}

int
QmBatteryStub::getRemainingCapacityPct ()
{
    stubMethodEntered ("getRemainingCapacityPct");
    return stubReturnValue<int> ("getRemainingCapacityPct");
}

int
QmBatteryStub::getMaxBars ()
{
    stubMethodEntered ("getMaxBars");
    return stubReturnValue<int> ("getMaxBars");
}

MeeGo::QmBattery::State QmBatteryStub::getState()
{
    stubMethodEntered ("getState");
    return stubReturnValue<MeeGo::QmBattery::State> ("getState");
}

MeeGo::QmBattery::Level QmBatteryStub::getLevel()
{
    stubMethodEntered ("getLevel");
    return stubReturnValue<MeeGo::QmBattery::Level> ("getLevel");
}


/*
 * Create a stub instance
 */
QmBatteryStub gDefaultQmBatteryStub;
QmBatteryStub *gQmBatteryStub = &gDefaultQmBatteryStub;

/*
 * Create a proxy which calls the stub
 */
namespace MeeGo
{

class QmBatteryPrivate
{
    /* Dummy class... */
};

QmBattery::QmBattery (QObject *parent)
{
    Q_UNUSED (parent);
    /*
     * Do nothing, we don't want to instantiate a real QmBattery...
     */
}

QmBattery::~QmBattery ()
{

}

QmBattery::ChargerType
QmBattery::getChargerType () const
{
    return gQmBatteryStub->getChargerType ();
}

QmBattery::ChargingState
QmBattery::getChargingState() const
{
    return gQmBatteryStub->getChargingState ();
}

QmBattery::BatteryState
QmBattery::getBatteryState () const
{
    return gQmBatteryStub->getBatteryState ();
}

int
QmBattery::getRemainingTalkTime (QmBattery::RemainingTimeMode mode) const
{
    return gQmBatteryStub->getRemainingTalkTime (mode);
}

int
QmBattery::getRemainingIdleTime (QmBattery::RemainingTimeMode mode) const
{
    return gQmBatteryStub->getRemainingIdleTime (mode);
}

int
QmBattery::getRemainingCapacityPct () const
{
    return gQmBatteryStub->getRemainingCapacityPct ();
}

int
QmBattery::getMaxBars () const
{
    return gQmBatteryStub->getMaxBars ();
}

QmBattery::State QmBattery::getState () const
{
    return gQmBatteryStub->getState();
}

QmBattery::Level QmBattery::getLevel() const
{
    return gQmBatteryStub->getLevel();
}

} /* namespace MeeGo */

#endif
