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

#ifndef UT_DESKTOPBACKGROUND_H
#define UT_DESKTOPBACKGROUND_H

#include <QtTest/QtTest>
#include <QObject>

class DesktopBackground;
class DesktopStyleContainer;
class MApplication;
class Ut_DesktopBackground : public QObject
{
    Q_OBJECT

private slots:
    // Called before the first testfunction is executed
    void initTestCase();
    // Called after the last testfunction was executed
    void cleanupTestCase();
    // Called before each testfunction is executed
    void init();
    // Called after every testfunction
    void cleanup();

    // test functions
    void testConstructionWithCorrectPixmaps();
    void testConstructionWithCorrectPixmapsWithCurrentOrientationChanging();
    void testConstructionWithIncorrectPortraitPixmap();
    void testConstructionWithIncorrectLandscapePixmap();
    void testConstructionWithIncorrectPixmaps();
    void testOrientationChangeAfterConstruction();
    void testPortraitImageModification();
    void testLandscapeImageModification();
    void testPortraitImageModifications();
    void testLandscapeImageModifications();
    void testGConfValueChanges();

signals:
    void portraitImageChanged();
    void landscapeImageChanged();
private:
    DesktopBackground* subject_;
    DesktopStyleContainer* style_;
    MApplication *app_;
};

#endif
