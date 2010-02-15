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
#ifndef _UT_SWITCHERBUTTONVIEW_
#define _UT_SWITCHERBUTTONVIEW_

#include <QtGui>
#include <QtTest/QtTest>
#include <X11/Xlib.h>
#include <X11/extensions/Xcomposite.h>
#include <QObject>
#include "switcherbutton.h"
#include "switcherbuttonview.h"

class DuiScalableImage;
class MainWindow;
class HomeApplication;

class TestSwitcherButtonView : public SwitcherButtonView
{
    DUI_VIEW(SwitcherButtonModel, SwitcherButtonStyle)

public:
    TestSwitcherButtonView(SwitcherButton &button);

    void mousePressRelease(const QPointF &p);
    void mousePress(const QPointF &p);
    void mouseMove(const QPointF &p);
    void mouseRelease(const QPointF &p);
    void mouseCancel();
    void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *item) const;

    QRectF closeRect() const {
        return SwitcherButtonView::closeRect();
    }

    QRectF buttonRect() const {
        return SwitcherButtonView::buttonRect();
    }

    SwitcherButtonStyle *modifiableStyle() {
        SwitcherButtonStyleContainer &sc = style();
        const SwitcherButtonStyle *const_s = sc.operator ->();
        SwitcherButtonStyle *s = const_cast<SwitcherButtonStyle *>(const_s);
        return s;
    }
};

class TestSwitcherButton : public SwitcherButton
{
public:
    TestSwitcherButton(const QString &title, DuiWidget *parent = NULL, Window window = 0);

    TestSwitcherButtonView *getView();

private:
    TestSwitcherButtonView *view;
};

class Ut_SwitcherButtonView : public QObject
{
    Q_OBJECT

public:
    static const DuiScalableImage *drawnScalableImage;

    // Switch for making the close timer timeout to occur immediately or not
    static bool timerImmediateTimeout;

    // For checking that the window pixmaps get freed
    static QList<Pixmap> allocatedPixmaps;
    static Pixmap lastPixmap;

    // Switch for making the XCompositeNameWindowPixmap stub return an X BadMatch error
    static bool xCompositeNameWindowPixmapCausesBadMatch;

    // The X error handler to be used
    static XErrorHandler xErrorHandler;

    // Whether a Damage object has been created
    static bool damageCreated;

    // Main window
    static MainWindow *mainWindow;

    // Previous QPainter opacity
    static qreal painterOpacity;

    // Previous QPainter text
    static QString painterText;

    // Opacity for the previous QPainter text
    static qreal painterTextOpacity;

private:
    HomeApplication *app;

    // SwitcherButton instance under testing
    TestSwitcherButton *button;

    // SwitcherButtonView instance under testing
    TestSwitcherButtonView *view;

signals:
    // Signal for notifying about window visibility changed
    void windowVisibilityChanged(Window window);

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();
    void testMousePressRelease();
    void testClosingWithTimeout();
    void testClosingWithoutTimeout();
    void testDragOutsideDoesNothing();
    void testDragInsideDoesNothing();
    void testXWindow();
    void testXWindowWithXError();
    void testTextOpacity();
};

#endif //_UT_SWITCHERBUTTONVIEW_
