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
#ifndef DUIFEEDBACK_STUB
#define DUIFEEDBACK_STUB

#include "duifeedback.h"
#include "../stubs/stubbase.h"


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiFeedbackStub : public StubBase
{
public:
    virtual void DuiFeedbackConstructor(QObject *parent);
    virtual void DuiFeedbackConstructor(const DuiFeedback &);
    virtual void DuiFeedbackConstructor(const QString &name, QObject *parent);
    virtual DuiFeedback &operator=(const DuiFeedback &other);
    virtual void DuiFeedbackDestructor();
    virtual void setName(const QString &name);
    virtual QString name() const;
    virtual void play() const;
};

// 2. IMPLEMENT STUB
void DuiFeedbackStub::DuiFeedbackConstructor(QObject *parent)
{
    Q_UNUSED(parent);

}
void DuiFeedbackStub::DuiFeedbackConstructor(const DuiFeedback &)
{

}
void DuiFeedbackStub::DuiFeedbackConstructor(const QString &name, QObject *parent)
{
    Q_UNUSED(name);
    Q_UNUSED(parent);

}
DuiFeedback &DuiFeedbackStub::operator=(const DuiFeedback &other)
{
    // just to get it compiling.. don't go using this!
    return *(new DuiFeedback(other));
}

void DuiFeedbackStub::DuiFeedbackDestructor()
{

}
void DuiFeedbackStub::setName(const QString &name)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(name));
    stubMethodEntered("setName", params);
}

QString DuiFeedbackStub::name() const
{
    stubMethodEntered("name");
    return stubReturnValue<QString>("name");
}

void DuiFeedbackStub::play() const
{
    stubMethodEntered("play");
}

// 3. CREATE A STUB INSTANCE
DuiFeedbackStub gDefaultDuiFeedbackStub;
DuiFeedbackStub *gDuiFeedbackStub = &gDefaultDuiFeedbackStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiFeedback::DuiFeedback(QObject *parent) : d_ptr(NULL)
{
    gDuiFeedbackStub->DuiFeedbackConstructor(parent);
}

DuiFeedback::DuiFeedback(const DuiFeedback &feed) : QObject(), d_ptr(NULL)
{
    gDuiFeedbackStub->DuiFeedbackConstructor(feed);
}

DuiFeedback::DuiFeedback(const QString &name, QObject *parent) : d_ptr(NULL)
{
    gDuiFeedbackStub->DuiFeedbackConstructor(name, parent);
}

DuiFeedback &DuiFeedback::operator=(const DuiFeedback &other)
{
    return gDuiFeedbackStub->operator=(other);
}

DuiFeedback::~DuiFeedback()
{
    gDuiFeedbackStub->DuiFeedbackDestructor();
}

void DuiFeedback::setName(const QString &name)
{
    gDuiFeedbackStub->setName(name);
}

QString DuiFeedback::name() const
{
    return gDuiFeedbackStub->name();
}

void DuiFeedback::play() const
{
    gDuiFeedbackStub->play();
}

#endif
