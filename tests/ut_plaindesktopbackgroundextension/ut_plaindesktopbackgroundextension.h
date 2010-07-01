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

#ifndef UT_PLAINDESKTOPBACKGROUNDEXTENSION_H
#define UT_PLAINDESKTOPBACKGROUNDEXTENSION_H

#include <QtTest/QtTest>
#include <QObject>
#include <QMap>
#include "mdesktopbackgroundextensioninterface.h"
#include "windowinfo.h"

class MApplication;
class MockPaintDevice;
class PlainDesktopBackgroundExtension;

class Ut_PlainDesktopBackgroundExtension : public QObject, public MDesktopInterface
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

    // Test cases
    void testInitialize();
    void testWidget();
    void testDrawBackground();
    void testUpdatePixmaps();
    void testUpdatePixmapsFails();
    void testSetDefocusFactor();
    void testSetDefocused();
    void testUpdateDesktop();
    void testUpdateDesktopNoDesktop();

signals:
    void setDefocusFactor(qreal blurFactor);
    void updateLandscapePixmap();
    void updatePortraitPixmap();
    void pixmapUpdated();

private:
    // MApplication
    MApplication *app;
    // Extension being tested
    PlainDesktopBackgroundExtension *extension;
    // Painter
    QPainter *painter;
    // Paint device
    MockPaintDevice *paintDevice;
    // Whether update() was called
    bool updateCalled;
    // Current orientation angle
    M::OrientationAngle currentOrientationAngle;

    virtual void update();
    virtual M::OrientationAngle orientationAngle();
};

#endif
