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
#ifndef STUBMETHOD_H
#define STUBMETHOD_H

#include <QString>

class MethodCall
{
public:
    MethodCall(const QString &name, QList<ParameterBase *> params, ParameterBase *returnValue)
        : _name(name),
          _params(params),
          _returnValue(returnValue) {
    }

    virtual ~MethodCall() {
        foreach(ParameterBase * p, _params) {
            delete p;
        }
    }

    QString name() {
        return _name;
    }

    QList<ParameterBase *> params() {
        return _params;
    }

    template <typename T>
    T parameter(int number) {
        if (number >= _params.count()) {
            QString msg = QString("MethodCall::") + __func__ + ": method " + _name + " does not have parameter #" + QString::number(number) + ". Check your test code.";
            qFatal(qPrintable(msg));
        }
        Parameter<T>* param = dynamic_cast<Parameter<T>* >(_params[number]);
        if (!param) {
            QString msg = QString("MethodCall::") + __func__ + ": failed dynamic_cast, check that parameter type matches parameter number";
            qFatal(qPrintable(msg));
        }
        return param->data;
    }

    template <typename T>
    T returnValue() {
        Parameter<T>* value = dynamic_cast<Parameter<T>*>(_returnValue);

        if (!value) {
            QString msg = QString("MethodCall::") + __func__ + ": failed dynamic_cast, check that type matches return value";
            qFatal(qPrintable(msg));
        }
        return value->data;
    }

    bool returnValueExists() {
        return (_returnValue != NULL);
    }

private:
    QString _name;
    QList<ParameterBase *> _params;
    ParameterBase *_returnValue;

};

#endif
