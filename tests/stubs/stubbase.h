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

#ifndef STUBBASE_H
#define STUBBASE_H

#include <QList>
#include <QVariant>
#include <QMap>
#include <QtDebug>
#include "parameter.h"
#include "methodcall.h"


class StubBase
{
public:
    // Return a list of method calls
    QList<MethodCall *> stubCallHistory() const;

    // Return a reference to the last method call
    MethodCall &stubLastCall() const;

    // Access parameters of the last method call
    template <typename T>
    T stubLastParameters(int number) const;

    // Access parameters of the last method call
    MethodCall &stubLastCallTo(const QString &method) const;

    // Return a list of calls to a particular method call
    QList<MethodCall *> stubCallsTo(const QString &method) const;

    // Return the number of times a method has been called
    int stubCallCount(const QString &method) const;

    // Set the return value for next call of methodName
    template <typename T>
    void stubSetReturnValue(const QString &methodName, T value) const;

    // Set the return value list for methodName calls
    template <typename T>
    void stubSetReturnValueList(const QString &methodName, QList<T> valueList) const;

    // Return the return value set for methodName
    template <typename T>
    T &stubReturnValue(const QString &methodName) const;

    template <typename T>
    T &stubReturnValueNoDefault(const QString &methodName) const;

    // For use by stubs
    virtual ~StubBase();
    virtual void stubReset() const;
    ParameterBase *stubReturnValue(const QString &methodName) const;
    void stubMethodEntered(const QString &methodName, QList<ParameterBase *> params) const;
    void stubMethodEntered(const QString &methodName) const;
    void stubResetReturnValueListIndex(const QString &methodName) const;

private:
    mutable QMap<QString, ParameterBase *> _stubReturnValues;
    mutable QMap<QString, QList<ParameterBase *> > _stubReturnValueLists;
    mutable QMap<QString, int> _stubReturnValueListCurrentIndex;
    mutable QMap<QString, int> _stubCallCounts;
    mutable QList<MethodCall *> _stubCallHistory;

};

template <typename T>
void StubBase::stubSetReturnValue(const QString &methodName, T value) const
{
    Parameter<T>* param = new Parameter<T>(value);
    _stubReturnValues[methodName] = param;
}

template <typename T>
void StubBase::stubSetReturnValueList(const QString &methodName, QList<T> valueList) const
{
    QList<ParameterBase *> newValueList;
    foreach(T value, valueList) {
        Parameter<T>* param = new Parameter<T>(value);
        newValueList.append(param);
    }
    _stubReturnValueLists[methodName] = newValueList;
    _stubReturnValueListCurrentIndex[methodName] = -1;
}

template <typename T>
T &StubBase::stubReturnValueNoDefault(const QString &methodName) const
{
    ParameterBase *base;
    if (_stubReturnValueLists.contains(methodName) && !_stubReturnValueLists[methodName].isEmpty() && _stubReturnValueListCurrentIndex[methodName] > -1) {
        base = _stubReturnValueLists[methodName].at(_stubReturnValueListCurrentIndex[methodName]);
    } else {
        if (! _stubReturnValues.contains(methodName)) {
            QString msg = QString("StubBase::") + __func__ + ": return value for method '" + methodName + "' not found; check that you have used stubSetReturnValue(" + methodName + ")";
            qFatal(qPrintable(msg));
        }

        base = _stubReturnValues[methodName];
    }

    Parameter<T>* param = dynamic_cast<Parameter<T>*>(base);
    if (!param) {
        QString msg = QString("StubBase::") + __func__ + ": failed dynamic_cast, check that return value type matches the method; check also that you have used stubSetReturnValue(" + methodName + ")";
        qFatal("%s", qPrintable(msg));
    }
    return param->data;
}


template <typename T>
T &StubBase::stubReturnValue(const QString &methodName) const
{
    ParameterBase *base;
    if (_stubReturnValueLists.contains(methodName) && !_stubReturnValueLists[methodName].isEmpty() && _stubReturnValueListCurrentIndex[methodName] > -1) {
        base = _stubReturnValueLists[methodName].at(_stubReturnValueListCurrentIndex[methodName]);
    } else {
        if (! _stubReturnValues.contains(methodName)) {
            stubSetReturnValue<T>(methodName, T());
        }

        base = _stubReturnValues[methodName];
    }

    Parameter<T>* param = dynamic_cast<Parameter<T>*>(base);
    if (!param) {
        QString msg = QString("StubBase::") + __func__ + ": failed dynamic_cast, check that return value type matches the method; check also that you have used stubSetReturnValue(" + methodName + ")";
        qFatal("%s", qPrintable(msg));
    }
    return param->data;
}

template <typename T>
T StubBase::stubLastParameters(int number) const
{
    MethodCall &call = stubLastCall();
    return call.parameter<T>(number);
}

#endif
