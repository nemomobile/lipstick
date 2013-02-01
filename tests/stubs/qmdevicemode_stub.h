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
#ifndef QMDEVICEMODE_STUB
#define QMDEVICEMODE_STUB

#include <qmdevicemode.h>
#include "stubbase.h"
/*
 * XXX: This stub contains only those methods which are 
 * used by the  DeviceModeBusinessLogic class ...
 */

/*
 * Declare stub
 */
class QmDeviceModeStub : public StubBase
{
public:
    virtual MeeGo::QmDeviceMode::PSMState getPSMState (); 
    virtual void setPSMState (MeeGo::QmDeviceMode::PSMState state);
    virtual int getPSMBatteryMode ();
    virtual void setPSMBatteryMode (int percentages);
    virtual MeeGo::QmDeviceMode::DeviceMode getMode();
    virtual bool setMode(MeeGo::QmDeviceMode::DeviceMode mode);
    virtual void connectNotify(const char *signal);
    virtual void disconnectNotify(const char *signal);
};

MeeGo::QmDeviceMode::PSMState
QmDeviceModeStub::getPSMState ()
{
    stubMethodEntered ("getPSMState");
    return stubReturnValue<MeeGo::QmDeviceMode::PSMState> ("getPSMState");
}

void
QmDeviceModeStub::setPSMState (MeeGo::QmDeviceMode::PSMState state)
{
    QList<ParameterBase*> params;
    params.append (new Parameter<MeeGo::QmDeviceMode::PSMState> (state));
    stubMethodEntered ("setPSMState", params);
}

int
QmDeviceModeStub::getPSMBatteryMode ()
{
    stubMethodEntered ("getPSMBatteryMode");
    return stubReturnValue<int> ("getPSMBatteryMode");
}

void
QmDeviceModeStub::setPSMBatteryMode (int percentages)
{
    QList<ParameterBase*> params;
    params.append (new Parameter<int> (percentages));
    stubMethodEntered ("setPSMBatteryMode", params);
}

bool QmDeviceModeStub::setMode (MeeGo::QmDeviceMode::DeviceMode mode)
{
    QList<ParameterBase*> params;
    params.append(new Parameter<MeeGo::QmDeviceMode::DeviceMode> (mode));
    stubMethodEntered ("setMode", params);
    return stubReturnValue<bool> ("setMode");
}

void QmDeviceModeStub::connectNotify(const char *signal)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<const char *>(signal));
  stubMethodEntered("connectNotify",params);
}


void QmDeviceModeStub::disconnectNotify(const char *signal)
{
  QList<ParameterBase*> params;
  params.append( new Parameter<const char *>(signal));
  stubMethodEntered("disconnectNotify",params);
}

MeeGo::QmDeviceMode::DeviceMode
QmDeviceModeStub::getMode ()
{
    stubMethodEntered ("getMode");
    return stubReturnValue<MeeGo::QmDeviceMode::DeviceMode> ("getMode");
}

/*
 * Create a stub instance
 */
QmDeviceModeStub gDefaultQmDeviceModeStub;
QmDeviceModeStub *gQmDeviceModeStub = &gDefaultQmDeviceModeStub;

/*
 * Create a proxy which calls the stub
 */
namespace MeeGo
{

QmDeviceMode::QmDeviceMode (QObject *parent)
{
    Q_UNUSED (parent);
    /*
     * Do nothing, we don't want to instantiate a real QmDeviceMode...
     */
}

QmDeviceMode::~QmDeviceMode ()
{
    priv_ptr = 0;
}

QmDeviceMode::PSMState
QmDeviceMode::getPSMState () const
{
    return gQmDeviceModeStub->getPSMState ();
}

bool
QmDeviceMode::setPSMState (MeeGo::QmDeviceMode::PSMState state)
{
    gQmDeviceModeStub->setPSMState (state);
    return true;
}

int
QmDeviceMode::getPSMBatteryMode ()
{
    return gQmDeviceModeStub->getPSMBatteryMode ();
}

bool
QmDeviceMode::setPSMBatteryMode (int percentages)
{
    gQmDeviceModeStub->setPSMBatteryMode (percentages);
    return true;
}

QmDeviceMode::DeviceMode QmDeviceMode::getMode() const
{
    return gQmDeviceModeStub->getMode();
}

bool QmDeviceMode::setMode(QmDeviceMode::DeviceMode mode)
{
    return gQmDeviceModeStub->setMode(mode);
}

void QmDeviceMode::connectNotify(const char *signal)
{
	 gQmDeviceModeStub->connectNotify(signal);
}
void QmDeviceMode::disconnectNotify(const char *signal)
{
	gQmDeviceModeStub->disconnectNotify(signal);
}
} /* namespace MeeGo */

#endif
