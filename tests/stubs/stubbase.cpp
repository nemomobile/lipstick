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
#include <assert.h>
#include "stubbase.h"
#include "methodcall.h"

QList<MethodCall *> StubBase::stubCallHistory() const
{
    return _stubCallHistory;
}

void StubBase::stubReset() const
{
    foreach(ParameterBase * p, _stubReturnValues) {
        delete p;
    }

    foreach(MethodCall * p, _stubCallHistory) {
        delete p;
    }

    _stubReturnValues.clear();
    _stubCallCounts.clear();
    _stubCallHistory.clear();
}

int StubBase::stubCallCount(const QString &method) const
{
    return _stubCallCounts[method];
}

void StubBase::stubMethodEntered(const QString &methodName, QList<ParameterBase *> params) const
{
    MethodCall *method = new MethodCall(methodName, params, stubReturnValue(methodName));
    _stubCallCounts[methodName] = _stubCallCounts[methodName] + 1;
    _stubCallHistory.append(method);
}

void StubBase::stubMethodEntered(const QString &methodName) const
{
    MethodCall *method = new MethodCall(methodName, QList<ParameterBase *>(), stubReturnValue(methodName));
    _stubCallCounts[methodName] = _stubCallCounts[methodName] + 1;
    _stubCallHistory.append(method);
}

ParameterBase *StubBase::stubReturnValue(const QString &methodName) const
{
    ParameterBase *retVal = NULL;

    if (_stubReturnValues.contains(methodName))
        retVal = _stubReturnValues[methodName];
    return retVal;
}

StubBase::~StubBase()
{
    stubReset();
}

MethodCall &StubBase::stubLastCall() const
{
    return *(_stubCallHistory.last());
}

MethodCall &StubBase::stubLastCallTo(const QString &method) const
{
    for (int i = _stubCallHistory.count() - 1; i >= 0; i--) {
        if (_stubCallHistory.at(i)->name() == method) {
            return *(_stubCallHistory.at(i));
        }
    }
    qDebug() << "StubBase::lastCallTo: call not found to:" << method;
    return *((MethodCall *)0);
}

