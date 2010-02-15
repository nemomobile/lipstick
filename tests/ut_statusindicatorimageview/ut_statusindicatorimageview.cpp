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

#include "ut_statusindicatorimageview.h"
#include "statusindicatorimageview.h"

#include <DuiApplication>
#include <DuiTheme>
#include <QPainter>

TestStatusIndicatorImageView::TestStatusIndicatorImageView(StatusIndicator *controller) :
    StatusIndicatorImageView(controller)
{
}

StatusIndicatorModel *TestStatusIndicatorImageView::getModel()
{
    return model();
}

void TestStatusIndicatorImageView::executeStyleChanged()
{
    applyStyle();
}

void TestStatusIndicatorImageView::callableDrawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    drawContents(painter, option);
}

StatusIndicatorImageStyle *TestStatusIndicatorImageView::modifiableStyle()
{
    StatusIndicatorImageStyleContainer &sc = style();
    const StatusIndicatorImageStyle *const_s = sc.operator ->();
    StatusIndicatorImageStyle *s = const_cast<StatusIndicatorImageStyle *>(const_s);
    return s;
}

// DuiTheme stubs
QHash<QPixmap *, QString> gDuiThemePixmapPixmaps;
QPixmap *DuiTheme::pixmapCopy(const QString &id, const QSize &)
{
    QPixmap *p = new QPixmap;
    gDuiThemePixmapPixmaps[p] = id;

    return p;
}

// QPainter stubs
QList<const QPixmap *> gQPainterPixmaps;
void QPainter::drawPixmap(const QPointF &point, const QPixmap &pixmap)
{
    Q_UNUSED(point);

    gQPainterPixmaps.append(&pixmap);
}

// QTimeLine stubs
int gQTimeLineDuration;
void QTimeLine::setDuration(int duration)
{
    gQTimeLineDuration = duration;
}
QPair<int, int> gQTimeLineFrameRange(-1, -1);
void QTimeLine::setFrameRange(int startFrame, int endFrame)
{
    gQTimeLineFrameRange = qMakePair(startFrame, endFrame);
}
int gQTimeLineUpdateInterval = -1;
void QTimeLine::setUpdateInterval(int interval)
{
    gQTimeLineUpdateInterval = interval;
}

QTimeLine::State gQTimeLineState = QTimeLine::NotRunning;
bool gQTimeLineStartedCalled = false;
bool gQTimeLineStoppedCalled = false;
void QTimeLine::start()
{
    gQTimeLineStartedCalled = true;
    gQTimeLineState = QTimeLine::Running;
}
void QTimeLine::stop()
{
    gQTimeLineStoppedCalled = true;
    gQTimeLineState = QTimeLine::NotRunning;
}
QTimeLine::State QTimeLine::state() const
{
    return gQTimeLineState;
}

void Ut_StatusIndicatorImageView::initTestCase()
{
    static int argc = 1;
    static char *app_name = (char *)"./ut_statusindicatorimageview";
    app = new DuiApplication(argc, &app_name);
}

void Ut_StatusIndicatorImageView::cleanupTestCase()
{
    delete app;
}

void Ut_StatusIndicatorImageView::init()
{
    gDuiThemePixmapPixmaps.clear();

    gQPainterPixmaps.clear();

    gQTimeLineDuration = -1;
    gQTimeLineFrameRange = qMakePair(-1, -1);
    gQTimeLineUpdateInterval = -1;
    gQTimeLineState = QTimeLine::NotRunning;
    gQTimeLineStartedCalled = false;
    gQTimeLineStoppedCalled = false;

    // Construct the test subject
    controller = new StatusIndicator;
    m_subject = new TestStatusIndicatorImageView(controller);
    m_subject->setModel(controller->model());
}

void Ut_StatusIndicatorImageView::cleanup()
{
    delete m_subject;
    delete controller;
}

void Ut_StatusIndicatorImageView::testImageListInitialized()
{
    m_subject->modifiableStyle()->setImageList("1 2");
    m_subject->executeStyleChanged();
    // Check that images are not loaded until they are used
    QCOMPARE(gDuiThemePixmapPixmaps.size(), 0);
}

void Ut_StatusIndicatorImageView::testSetAnimationFrame()
{
    // Test that the latest image list is used
    m_subject->modifiableStyle()->setImageList("3 4");
    m_subject->executeStyleChanged();
    m_subject->modifiableStyle()->setImageList("1 2");
    m_subject->executeStyleChanged();
    QPainter painter;

    // Check that correct images get loaded from the theme and drawn
    m_subject->setAnimationFrame(0);
    m_subject->callableDrawContents(&painter, NULL);
    QCOMPARE(gDuiThemePixmapPixmaps.size(), 1);
    QVERIFY(gDuiThemePixmapPixmaps.key("1") != NULL);
    QCOMPARE(gQPainterPixmaps.size(), 1);
    QCOMPARE(gQPainterPixmaps.at(0), gDuiThemePixmapPixmaps.key("1"));

    gQPainterPixmaps.clear();
    m_subject->setAnimationFrame(1);
    m_subject->callableDrawContents(&painter, NULL);
    QCOMPARE(gDuiThemePixmapPixmaps.size(), 2);
    QVERIFY(gDuiThemePixmapPixmaps.key("2") != NULL);
    QCOMPARE(gQPainterPixmaps.size(), 1);
    QCOMPARE(gQPainterPixmaps.at(0), gDuiThemePixmapPixmaps.key("2"));

    // Check that images are not loaded again if they are already loaded
    gDuiThemePixmapPixmaps.clear();
    m_subject->setAnimationFrame(0);
    m_subject->callableDrawContents(&painter, NULL);
    m_subject->setAnimationFrame(1);
    m_subject->callableDrawContents(&painter, NULL);
    QCOMPARE(gDuiThemePixmapPixmaps.size(), 0);
}

void Ut_StatusIndicatorImageView::testSetAnimationFrameToInvalid()
{
    m_subject->modifiableStyle()->setImageList("");
    m_subject->executeStyleChanged();
    QPainter painter;

    // Set animation frame to smaller than allowed
    m_subject->setAnimationFrame(-2);
    m_subject->callableDrawContents(&painter, NULL);
    QCOMPARE(gQPainterPixmaps.size(), 1);
    QCOMPARE(gQPainterPixmaps.at(0), gDuiThemePixmapPixmaps.key(""));
    QCOMPARE(gDuiThemePixmapPixmaps.size(), 1);
    QVERIFY(gDuiThemePixmapPixmaps.key("") != NULL);

    // Set animation frame to larger than allowed
    gQPainterPixmaps.clear();
    m_subject->setAnimationFrame(3);
    m_subject->callableDrawContents(&painter, NULL);
    QCOMPARE(gQPainterPixmaps.size(), 1);
    QCOMPARE(gQPainterPixmaps.at(0), gDuiThemePixmapPixmaps.key(""));
    QCOMPARE(gDuiThemePixmapPixmaps.size(), 1);
    QVERIFY(gDuiThemePixmapPixmaps.key("") != NULL);

    // Test the same with a longer image list
    gQPainterPixmaps.clear();
    gDuiThemePixmapPixmaps.clear();
    m_subject->modifiableStyle()->setImageList("1 2");
    m_subject->executeStyleChanged();

    // Set animation frame to smaller than allowed
    m_subject->setAnimationFrame(-2);
    m_subject->callableDrawContents(&painter, NULL);
    QCOMPARE(gQPainterPixmaps.size(), 1);
    QCOMPARE(gQPainterPixmaps.at(0), gDuiThemePixmapPixmaps.key("1"));
    QCOMPARE(gDuiThemePixmapPixmaps.size(), 1);
    QVERIFY(gDuiThemePixmapPixmaps.key("1") != NULL);

    // Set animation frame to larger than allowed
    gQPainterPixmaps.clear();
    m_subject->setAnimationFrame(3);
    m_subject->callableDrawContents(&painter, NULL);
    QCOMPARE(gQPainterPixmaps.size(), 1);
    QCOMPARE(gQPainterPixmaps.at(0), gDuiThemePixmapPixmaps.key("2"));
    QCOMPARE(gDuiThemePixmapPixmaps.size(), 2);
    QVERIFY(gDuiThemePixmapPixmaps.key("2") != NULL);
}

void Ut_StatusIndicatorImageView::testSetAnimationDuration()
{
    m_subject->modifiableStyle()->setImageList("1 2 3");
    m_subject->modifiableStyle()->setAnimationDuration(33);
    m_subject->executeStyleChanged();
    QCOMPARE(gQTimeLineDuration, 33);
    QCOMPARE(gQTimeLineFrameRange, qMakePair(0, 3));
    QCOMPARE(gQTimeLineUpdateInterval, 11);
}

void Ut_StatusIndicatorImageView::testValueChanged()
{
    m_subject->modifiableStyle()->setImageList("1 2 3");
    m_subject->executeStyleChanged();

    // Setting the value to half
    m_subject->getModel()->setValue(0.5);

    // The middle image should be painted
    QPainter painter;
    m_subject->callableDrawContents(&painter, NULL);
    QCOMPARE(gQPainterPixmaps.size(), 1);
    QCOMPARE(gQPainterPixmaps.at(0), gDuiThemePixmapPixmaps.key("2"));
}

void Ut_StatusIndicatorImageView::testValueChangedToInvalid()
{
    m_subject->modifiableStyle()->setImageList("1 2 3");
    m_subject->executeStyleChanged();

    QPainter painter;

    // Set value to too small
    m_subject->getModel()->setValue(-0.5);
    // The first image should be painted
    m_subject->callableDrawContents(&painter, NULL);
    QCOMPARE(gQPainterPixmaps.size(), 1);
    QCOMPARE(gQPainterPixmaps.at(0), gDuiThemePixmapPixmaps.key("1"));

    gQPainterPixmaps.clear();

    // Set value to too high
    m_subject->getModel()->setValue(1.5);
    // The last image should be painted
    m_subject->callableDrawContents(&painter, NULL);
    QCOMPARE(gQPainterPixmaps.size(), 1);
    QCOMPARE(gQPainterPixmaps.at(0), gDuiThemePixmapPixmaps.key("3"));
}

void Ut_StatusIndicatorImageView::testChangingAnimate()
{
    QCOMPARE(m_subject->getModel()->animate(), false);

    // Since animate is false by default, resetting it to false shouldn't have any effect
    m_subject->getModel()->setAnimate(false);
    QCOMPARE(gQTimeLineStartedCalled, false);
    QCOMPARE(gQTimeLineStoppedCalled, false);

    m_subject->getModel()->setAnimate(true);
    QCOMPARE(gQTimeLineStartedCalled, true);
    QCOMPARE(gQTimeLineStoppedCalled, false);

    gQTimeLineStartedCalled = false;

    // Setting the animate again to true shouldn't have any effect
    m_subject->getModel()->setAnimate(true);
    QCOMPARE(gQTimeLineStartedCalled, false);
    QCOMPARE(gQTimeLineStoppedCalled, false);

    m_subject->getModel()->setAnimate(false);
    QCOMPARE(gQTimeLineStartedCalled, false);
    QCOMPARE(gQTimeLineStoppedCalled, true);
}

QTEST_APPLESS_MAIN(Ut_StatusIndicatorImageView)
