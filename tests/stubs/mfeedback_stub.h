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
#ifndef MFEEDBACK_STUB
#define MFEEDBACK_STUB

#include "mfeedback.h"
#include "../stubs/stubbase.h"


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MFeedbackStub : public StubBase
{
public:
    virtual void MFeedbackConstructor(QObject *parent);
    virtual void MFeedbackConstructor(const MFeedback &);
    virtual void MFeedbackConstructor(const QString &name, QObject *parent);
    virtual MFeedback &operator=(const MFeedback &other);
    virtual void MFeedbackDestructor();
    virtual void setName(const QString &name);
    virtual QString name() const;
    virtual void play() const;
};

// 2. IMPLEMENT STUB
void MFeedbackStub::MFeedbackConstructor(QObject *parent)
{
    Q_UNUSED(parent);

}
void MFeedbackStub::MFeedbackConstructor(const MFeedback &)
{

}
void MFeedbackStub::MFeedbackConstructor(const QString &name, QObject *parent)
{
    Q_UNUSED(name);
    Q_UNUSED(parent);

}
MFeedback &MFeedbackStub::operator=(const MFeedback &other)
{
    // just to get it compiling.. don't go using this!
    return *(new MFeedback(other));
}

void MFeedbackStub::MFeedbackDestructor()
{

}
void MFeedbackStub::setName(const QString &name)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QString & >(name));
    stubMethodEntered("setName", params);
}

QString MFeedbackStub::name() const
{
    stubMethodEntered("name");
    return stubReturnValue<QString>("name");
}

void MFeedbackStub::play() const
{
    stubMethodEntered("play");
}

// 3. CREATE A STUB INSTANCE
MFeedbackStub gDefaultMFeedbackStub;
MFeedbackStub *gMFeedbackStub = &gDefaultMFeedbackStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MFeedback::MFeedback(QObject *parent) : d_ptr(NULL)
{
    gMFeedbackStub->MFeedbackConstructor(parent);
}

MFeedback::MFeedback(const MFeedback &feed) : QObject(), d_ptr(NULL)
{
    gMFeedbackStub->MFeedbackConstructor(feed);
}

MFeedback::MFeedback(const QString &name, QObject *parent) : d_ptr(NULL)
{
    gMFeedbackStub->MFeedbackConstructor(name, parent);
}

MFeedback &MFeedback::operator=(const MFeedback &other)
{
    return gMFeedbackStub->operator=(other);
}

MFeedback::~MFeedback()
{
    gMFeedbackStub->MFeedbackDestructor();
}

void MFeedback::setName(const QString &name)
{
    gMFeedbackStub->setName(name);
}

QString MFeedback::name() const
{
    return gMFeedbackStub->name();
}

void MFeedback::play() const
{
    gMFeedbackStub->play();
}

#endif
