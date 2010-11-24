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
#ifndef _UT_SWITCHERBUTTONVIEW_
#define _UT_SWITCHERBUTTONVIEW_

#include <QtGui>
#include <QtTest/QtTest>
#include <X11/Xlib.h>
#include <X11/extensions/Xcomposite.h>
#include <QObject>
#include "switcherbutton.h"
#include "switcherbuttonview.h"

class MScalableImage;
class MainWindow;
class TestHomeApplication;

class Ut_SwitcherButtonStyle : public SwitcherButtonStyle
{
};

class Ut_SwitcherButtonStyleContainer : public SwitcherButtonStyleContainer
{
public:
    QString currentMode()
    {
        return MButtonStyleContainer::currentMode();
    }
};

class TestSwitcherButtonView : public SwitcherButtonView
{
    M_VIEW(SwitcherButtonModel, Ut_SwitcherButtonStyle)

public:
    TestSwitcherButtonView(SwitcherButton &button);

    SwitcherButtonStyle *modifiableStyle() {
        SwitcherButtonStyleContainer &sc = style();
        const SwitcherButtonStyle *const_s = sc.operator ->();
        SwitcherButtonStyle *s = const_cast<SwitcherButtonStyle *>(const_s);
        return s;
    }
    Ut_SwitcherButtonStyleContainer& styleContainer() {
        return style();
    }
};

class TestSwitcherButton : public SwitcherButton
{
public:
    TestSwitcherButton(QGraphicsItem *parent = NULL);

    TestSwitcherButtonView *getView();
    void emitDisplayEntered();
    void emitDisplayExited();

private:
    TestSwitcherButtonView *view;
};

class Ut_SwitcherButtonView : public QObject
{
    Q_OBJECT

public:
    static const MScalableImage *drawnScalableImage;

    // For checking whether a timer is started even if it has already been started
    static bool timerStarted;

    // For checking that the window pixmaps get freed
    static QList<Pixmap> allocatedPixmaps;
    static Pixmap lastPixmap;

    // Switch for making the XCompositeNameWindowPixmap stub return an X BadMatch error
    static bool xCompositeNameWindowPixmapCausesBadMatch;

    // The X error handler to be used
    static XErrorHandler xErrorHandler;

    // For catching XChangeProperty parameters
    static Display *xChangePropertyDisplay;
    static Window xChangePropertyWindow;
    static Atom xChangePropertyProperty;
    static Atom xChangePropertyType;
    static int xChangePropertyFormat;
    static int xChangePropertyMode;
    static unsigned char xChangePropertyData[16];
    static int xChangePropertyNElements;

    // Whether a Damage object has been created
    static bool damageCreated;
    static bool damageDestroyed;
    static unsigned long damageHandle;
    static Display *damageDisplay;

    // Subtract parameters
    static bool damageSubtracted;
    static unsigned long damageSubtractHandle;
    static Display *damageSubtractDisplay;

    // Main window
    static MainWindow *mainWindow;

    // To see whether update() was called for the view
    static bool viewUpdateCalled;

    // QPainter::DrawPixmap() data
    static QRectF drawPixmapRect;
    static QRectF drawPixmapSourceRect;

    // QPixmap data
    static int returnedPixmapWidth;
    static int returnedPixmapHeight;

private:
    TestHomeApplication *app;

    // SwitcherButton instance under testing
    TestSwitcherButton *button;

    // SwitcherButtonView instance under testing
    TestSwitcherButtonView *m_subject;

    // Sets up the tests dealing with handling the visibility notify XEvents
    XEvent setupVisibilityNotifyTest();

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();
    void testXWindow();
    void testViewModeChange();
    void testDamageEventForKnownDamage();
    void testDamageEventForUnknownDamage();
    void testEnterExitDisplay();
    void testXDamageSubtractWhenDisplayEntered();
    void testSignalConnections();
    void testDrawBackground_data();
    void testDrawBackground();
    void testWhenDrawingPixmapThrowsBadAllocThenDrawBackgroundSkipsDrawing();
    void testUpdateXWindowIconGeometryIfNecessary();
    void testUpdateXWindowIconGeometry();
    void testUpdateXWindowPixmap();
    void testDamageCreationUponConstruction();
    void testSwitcherButtonsVisibilityEventCausesUpdateToBeCalled();
    void testSwitcherButtonsVisibilityEventWithIncorrectTypeDoesNotCauseUpdateToBeCalled();
    void testSwitcherButtonsVisibilityEventWithIncorrectVisibilityStateDoesNotCauseUpdateToBeCalled();
    void testSwitcherButtonsVisibilityEventWithIncorrectVisibilityWindowDoesNotCauseUpdateToBeCalled();
    void testSwitcherButtonsVisibilityEventWithIncorrectVisibilitySendEventDoesNotCauseUpdateToBeCalled();
};

#endif //_UT_SWITCHERBUTTONVIEW_
