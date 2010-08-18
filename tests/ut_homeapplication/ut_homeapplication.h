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

#ifndef UT_HOMEAPPLICATION_H
#define UT_HOMEAPPLICATION_H

#include <QObject>

class HomeApplication;

class Ut_HomeApplication : public QObject
{
    Q_OBJECT

public:
    static QList<QString> validInterfaces;
    static QList<QString> serviceInterfaces;
    static QList<QString> asyncCallMethods;
    static QList< QList<QVariant> > asyncCallArguments;

private slots:
    // Called before the first testfunction is executed
    void initTestCase();
    // Called after the last testfunction was executed
    void cleanupTestCase();
    // Called before each testfunction is executed
    void init();
    // Called after every testfunction
    void cleanup();

    // Test the startup, so that the process stopping and d-bus
    // notifications work
    void testUpstartStartup();
    // Test the startup, so that the process stopping and d-bus
    // notifications don't happen when the --upstart switch is not given
    void testNonUpstartStartup();

    void testWhenNoEventListenerExistsXEventFilterReturnsMApplicationReturnValue();
    void testWhenEventFilterReturnsFalseXEventFilterReturnsMApplicationReturnValue();
    void testXEventFilterReturnsTrueWhenEventFilterReturnsTrue();
    void testXEventFilterReturnsTrueWhenThereAreTwoEventFiltersAndOnlyTheOtherReturnsTrue();
    void testSameXEventFilterCanBeAddedOnlyOnce();
    void testAddingNewEventFilterDuringEventProcessing();
    void testRemovingOfAlreadyCalledEventFilterDuringEventProcessing();
    void testRemovingOfTheEventFilterThatIsCurrentlyProcessingAnEvent();
    void testRemovingOfAnEventFilterThatHasNotYetProcessedAnEvent();

public:
    // The object being tested
    static HomeApplication *m_subject;
};

#endif
