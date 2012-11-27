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

#ifndef UT_EVENTEATER_H
#define UT_EVENTEATER_H

#include <QObject>

class EventEater;

class Ut_EventEater : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testButtonEvents_data();
    void testButtonEvents();
    void testWindowIsCreatedProperly();
    void testStackingLayerPropertyIsSet();
    void testWindowIsDestroyed();
    void testShow();
    void testHide();

private:
    EventEater *m_subject;
};

#endif
