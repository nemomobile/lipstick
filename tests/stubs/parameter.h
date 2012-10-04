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
#ifndef PARAMETER_H
#define PARAMETER_H

class ParameterBase
{
public:
    virtual ~ParameterBase() {
        // Class needs to have at least one virtual function to be polymorphic
        // (and thus enable dynamic_cast)
    }
protected:
    ParameterBase() {
    }
};

template <typename T>
class Parameter : public ParameterBase
{
public:
    Parameter(T value) : ParameterBase(), data(value) {
    }
    T data;
};
#endif
