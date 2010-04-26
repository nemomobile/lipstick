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

    // Return the number of times a method has been called
    int stubCallCount(const QString &method) const;

    // Set the return value for next call of methodName
    template <typename T>
    void stubSetReturnValue(const QString &methodName, T value) const;

    // Return the return value set for methodName
    template <typename T>
    T &stubReturnValue(const QString &methodName) const;



    // For use by stubs
    virtual ~StubBase();
    void stubReset() const;
    ParameterBase *stubReturnValue(const QString &methodName) const;
    void stubMethodEntered(const QString &methodName, QList<ParameterBase *> params) const;
    void stubMethodEntered(const QString &methodName) const;

private:
    mutable QMap<QString, ParameterBase *> _stubReturnValues;
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
T &StubBase::stubReturnValue(const QString &methodName) const
{
    if (! _stubReturnValues.contains(methodName)) {
        stubSetReturnValue<T>(methodName, T());
    }

    ParameterBase *base = _stubReturnValues[methodName];
    Parameter<T>* param = dynamic_cast<Parameter<T>*>(base);
    if (!param) {
        QString msg = QString("StubBase::") + __func__ + ": failed dynamic_cast, check that return value type matches the method; check also that you have used stubSetReturnValue(" + methodName + ")";
        qFatal(msg.toStdString().c_str());
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
