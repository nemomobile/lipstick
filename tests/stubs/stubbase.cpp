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

    foreach(QList<ParameterBase *> valueList, _stubReturnValueLists) {
        foreach(ParameterBase *p, valueList) {
            delete p;
        }
    }

    _stubReturnValueLists.clear();
    _stubReturnValueListCurrentIndex.clear();
    _stubReturnValues.clear();
    _stubCallCounts.clear();
    _stubCallHistory.clear();
}

void StubBase::stubResetReturnValueListIndex(const QString &methodName) const
{
    if (_stubReturnValueListCurrentIndex.contains(methodName)) {
        _stubReturnValueListCurrentIndex[methodName] = -1;
    }
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
    if (_stubReturnValueLists.contains(methodName) && !_stubReturnValueLists[methodName].isEmpty()) {
        _stubReturnValueListCurrentIndex[methodName]++;
        // List has been iterated to end, start from beginning
        if (_stubReturnValueListCurrentIndex[methodName] >= _stubReturnValueLists[methodName].count()) {
            _stubReturnValueListCurrentIndex[methodName] = 0;
        }

        retVal = _stubReturnValueLists[methodName].at(_stubReturnValueListCurrentIndex[methodName]);
    } else if (_stubReturnValues.contains(methodName)) {
        retVal = _stubReturnValues[methodName];
    }

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
    int i = _stubCallHistory.count() - 1;
    bool found = false;

    for (; i >= 0; i--) {
        if (_stubCallHistory.at(i)->name() == method) {
            found = true;
            break;
        }
    }

    if (!found) {
        QString msg = QString("StubBase::") + __func__ + ": no calls found to '" + method + "'";
        qFatal("%s", qPrintable(msg));
    }

    return *(_stubCallHistory.at(i));
}

QList<MethodCall *> StubBase::stubCallsTo(const QString &method) const
{
    QList<MethodCall *> calls;
    for (int i = 0; i < _stubCallHistory.count(); i++) {
        if (_stubCallHistory.at(i)->name() == method) {
            calls.append(_stubCallHistory.at(i));
        }
    }
    return calls;
}
