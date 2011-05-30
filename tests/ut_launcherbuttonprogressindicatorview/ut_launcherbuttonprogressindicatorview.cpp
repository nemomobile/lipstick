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

#include <QtTest/QtTest>
#include <MApplication>
#include "ut_launcherbuttonprogressindicatorview.h"

#include "launcherbuttonprogressindicator.h"

Q_DECLARE_METATYPE(QAbstractAnimation::State);

bool gIsOnDisplay;
bool MWidget::isOnDisplay() const
{
    return gIsOnDisplay;
}

bool gMWidgetViewUpdateCalled;
void MWidgetView::update(const QRectF &)
{
    gMWidgetViewUpdateCalled = true;
}

static const QSize VALID_PIXMAP_SIZE = QSize(78,78);
static const QSize INVALID_PIXMAP_SIZE = QSize(50,50);
QHash<QString, QPixmap *> loadedPixmaps;
QHash<const QPixmap *, QPair<QRectF, QRectF> > gPixmapsDrawn;
const QPixmap *MTheme::pixmap(const QString &id, const QSize &)
{
    QPixmap *pixmap;
    // Whether pixmap is valid, workaround for bug #258902
    if (id.section('_', 1, 1).toInt() == VALID_PIXMAP_SIZE.height()) {
        pixmap = new QPixmap(VALID_PIXMAP_SIZE);
    } else {
        pixmap = new QPixmap(INVALID_PIXMAP_SIZE);
    }

    if (loadedPixmaps.contains(id)) {
        QPixmap *oldPixmap = loadedPixmaps.value(id);
        MTheme::releasePixmap(oldPixmap);
        loadedPixmaps.remove(id);
    }
    loadedPixmaps.insert(id, pixmap);
    return pixmap;
}

int gPixmapReleaseCount;
void MTheme::releasePixmap(const QPixmap *const_pixmap)
{
    const QString &frameName = loadedPixmaps.key(const_cast<QPixmap *>(const_pixmap));
    QPixmap *pixmap = loadedPixmaps.value(frameName);
    if (!frameName.isNull()) {
        gPixmapsDrawn.remove(const_pixmap);
        loadedPixmaps.remove(frameName);
        delete pixmap;
        gPixmapReleaseCount++;
    }
}

void QPainter::drawPixmap(const QRectF &target, const QPixmap &pixmap, const QRectF &source)
{
    gPixmapsDrawn.insert(&pixmap, QPair<QRectF, QRectF>(target, source));
}

void Ut_LauncherButtonProgressIndicatorView::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_launcherbuttonprogressindicatorview" };
    app = new MApplication(argc, app_name);
}

void Ut_LauncherButtonProgressIndicatorView::cleanupTestCase()
{
    delete app;
}

void Ut_LauncherButtonProgressIndicatorView::init()
{
    gIsOnDisplay = true;
    gPixmapReleaseCount = 0;
    gMWidgetViewUpdateCalled = false;

    controller = new LauncherButtonProgressIndicator;
    controller->setModel(new LauncherButtonProgressIndicatorModel);
    m_subject = new TestLauncherButtonProgressIndicatorView(controller);
    controller->setView(m_subject);
    controller->setRange(0, 100);

    m_subject->modifiableStyle()->setBaseImageName("");
}

void Ut_LauncherButtonProgressIndicatorView::cleanup()
{
    delete controller;

}

void Ut_LauncherButtonProgressIndicatorView::testUpdateDataForStyleModes_data()
{
     QTest::addColumn<LauncherButtonModel::State>("state");
     QTest::addColumn<QString>("mode");

     QTest::newRow("Downloading") << LauncherButtonModel::Downloading << "downloading";
     QTest::newRow("Installing") << LauncherButtonModel::Installing << "installing";
     QTest::newRow("Launching") << LauncherButtonModel::Launching << "launching";
}

void Ut_LauncherButtonProgressIndicatorView::testUpdateDataForStyleModes()
{
    QFETCH(LauncherButtonModel::State, state);
    QFETCH(QString, mode);

    m_subject->model()->setIndicatorState(state);

    QCOMPARE(m_subject->style().currentMode(), mode);
}

void Ut_LauncherButtonProgressIndicatorView::testAnimationPauseAndResumeOnVisibilityChanges_data()
{
     QTest::addColumn<bool>("initialUnknownDuration");
     QTest::addColumn<bool>("initialVisibility");
     QTest::addColumn<bool>("unknownDuration");
     QTest::addColumn<bool>("visibility");
     QTest::addColumn<QAbstractAnimation::State>("expectedAnimationState");

     QTest::newRow("Animation paused when comes unvisible") << true << true << true << false << QPropertyAnimation::Paused;
     QTest::newRow("Animation resumed when comes visible") << true << false << true << true << QPropertyAnimation::Running;
     QTest::newRow("Animation paused when changed to \"known\" duration") << true << true << false << true << QPropertyAnimation::Paused;
     QTest::newRow("Animation resumed when changed to \"unknown\" duration") << false << true << true << true << QPropertyAnimation::Running;
}

void Ut_LauncherButtonProgressIndicatorView::testAnimationPauseAndResumeOnVisibilityChanges()
{

    QFETCH(bool, initialUnknownDuration);
    QFETCH(bool, initialVisibility);
    QFETCH(bool, unknownDuration);
    QFETCH(bool, visibility);
    QFETCH(QAbstractAnimation::State, expectedAnimationState);

    controller->setUnknownDuration(initialUnknownDuration);
    controller->setVisible(initialVisibility);

    controller->setUnknownDuration(unknownDuration);
    controller->setVisible(visibility);

    QCOMPARE(m_subject->positionAnimation->state(), expectedAnimationState);
}

void Ut_LauncherButtonProgressIndicatorView::testAnimationPausedWhenDisplayExited()
{
    controller->setUnknownDuration(true);
    controller->setVisible(true);

    QVERIFY(disconnect(controller, SIGNAL(displayExited()), m_subject, SLOT(pauseAnimation())));
    m_subject->pauseAnimation();

   QCOMPARE(m_subject->positionAnimation->state(), QPropertyAnimation::Paused);
}

void Ut_LauncherButtonProgressIndicatorView::testAnimationResumedWhenDisplayEntered()
{
    controller->setUnknownDuration(true);
    controller->setVisible(true);
    m_subject->pauseAnimation();

    QVERIFY(disconnect(controller, SIGNAL(displayEntered()), m_subject, SLOT(resumeAnimation())));
    m_subject->resumeAnimation();

    QCOMPARE(m_subject->positionAnimation->state(), QPropertyAnimation::Running);
}

void Ut_LauncherButtonProgressIndicatorView::testApplyAnimationStyle()
{
    const static int expectedPeriod = 2000;
    const static int expectedFrames = 20;
    m_subject->modifiableStyle()->setPeriod(expectedPeriod);
    m_subject->modifiableStyle()->setNumberOfFrames(expectedFrames);

    m_subject->applyStyle();

    QCOMPARE( m_subject->positionAnimation->duration(), expectedPeriod);
    QCOMPARE(m_subject->positionAnimation->startValue().toInt(), 0);
    QCOMPARE(m_subject->positionAnimation->endValue().toInt(), expectedFrames);
}

void Ut_LauncherButtonProgressIndicatorView::testLoadingFramePixmaps()
{
    const static int expectedFrames = 20;
    const static int expectedBaseImageSize = 78;
    const static QString expectedBaseImageName("spinnerbaseimage");
    m_subject->modifiableStyle()->setNumberOfFrames(expectedFrames);
    m_subject->modifiableStyle()->setBaseImageSize(expectedBaseImageSize);
    m_subject->modifiableStyle()->setBaseImageName(expectedBaseImageName);

    m_subject->applyStyle();

    QCOMPARE(m_subject->animationPixmaps.count(), 20);
    QCOMPARE(loadedPixmaps.count(), 20);

    for (int i = 1; i <= loadedPixmaps.keys().count(); i++) {
        QString expectedFrameName = QString("%1_%2_%3").arg(expectedBaseImageName)\
                                                    .arg(expectedBaseImageSize)\
                                                    .arg(i);
        QCOMPARE(loadedPixmaps.keys().contains(expectedFrameName), QBool(true));
    }
}

void Ut_LauncherButtonProgressIndicatorView::testNotLoadingFramePixmapsWhenBaseImageIsEmpty()
{
    m_subject->modifiableStyle()->setBaseImageName("");

    m_subject->applyStyle();

    QCOMPARE(m_subject->animationPixmaps.count(), 0);
    QCOMPARE(loadedPixmaps.count(), 0);
}

void Ut_LauncherButtonProgressIndicatorView::testReleasingFramePixmapsWhenLoadingNew()
{
    m_subject->modifiableStyle()->setNumberOfFrames(10);
    m_subject->modifiableStyle()->setBaseImageSize(78);
    m_subject->modifiableStyle()->setBaseImageName("spinnerbaseimage");

    // Load pixmaps first time
    m_subject->applyStyle();

    // Load pixmaps again
    m_subject->applyStyle();

    QCOMPARE(gPixmapReleaseCount, 10);
    QCOMPARE(m_subject->animationPixmaps.count(), 10);
}

void Ut_LauncherButtonProgressIndicatorView::testReleasingFramePixmapsInDestructor()
{
    m_subject->modifiableStyle()->setNumberOfFrames(10);
    m_subject->modifiableStyle()->setBaseImageSize(78);
    m_subject->modifiableStyle()->setBaseImageName("spinnerbaseimage");

    m_subject->applyStyle();

    delete controller;
    controller = NULL;

    QCOMPARE(loadedPixmaps.count(), 0);
}

void Ut_LauncherButtonProgressIndicatorView::testSettingCurrentFrame_data()
{
    QTest::addColumn<int>("amountOfFrames");
    QTest::addColumn<int>("previousCurrentFrame");
    QTest::addColumn<int>("newValue");

    QTest::addColumn<int>("expectedCurrentFrame");
    QTest::addColumn<bool>("expectedCurrentFrameChanged");

    QTest::newRow("Good frame index") << 10 << 5 << 80 << 7 << true;
    QTest::newRow("Min frame index") << 10 << 5 << 0 << 0 << true;
    QTest::newRow("Max frame index") << 10 << 5 << 100 << 9 << true;
    QTest::newRow("Same frame set twice") << 10 << 5 << 60 << 5 << false;
    QTest::newRow("Too big frame index") << 10 << 5 << 110 << 5 << false;
    QTest::newRow("Too small frame index") << 10 << 5 << -10 << 5 << false;
}

void Ut_LauncherButtonProgressIndicatorView::testSettingCurrentFrame()
{
    QFETCH(int, amountOfFrames);
    QFETCH(int, previousCurrentFrame);
    QFETCH(int, newValue);

    QFETCH(int, expectedCurrentFrame);
    QFETCH(bool, expectedCurrentFrameChanged);

    m_subject->modifiableStyle()->setNumberOfFrames(amountOfFrames);
    m_subject->modifiableStyle()->setBaseImageSize(78);
    m_subject->modifiableStyle()->setBaseImageName("spinnerbaseimage");
    controller->model()->setValue(-1);
    m_subject->setCurrentFrame(previousCurrentFrame);
    gMWidgetViewUpdateCalled = false;

    controller->model()->setValue(newValue);

    QCOMPARE(m_subject->currentFrameIndex, expectedCurrentFrame);
    QCOMPARE(gMWidgetViewUpdateCalled, expectedCurrentFrameChanged);
}

void Ut_LauncherButtonProgressIndicatorView::testDrawingFrameWhenUnknownDuration()
{
    const static int expectedFrames = 20;
    const static int expectedBaseImageSize = 78;
    const static QString expectedBaseImageName("spinnerbaseimage");
    m_subject->modifiableStyle()->setNumberOfFrames(expectedFrames);
    m_subject->modifiableStyle()->setBaseImageSize(expectedBaseImageSize);
    m_subject->modifiableStyle()->setBaseImageName(expectedBaseImageName);
    controller->setUnknownDuration(true);
    m_subject->applyStyle();

    int currentFrame = 5;
    int pixmapIndex = currentFrame + 1;
    QString expectedFrame = QString("%1_%2_%3").arg(expectedBaseImageName)\
                                                    .arg(expectedBaseImageSize)\
                                                    .arg(pixmapIndex);
    m_subject->setCurrentFrame(currentFrame);

    QPainter painter;
    m_subject->drawContents(&painter, NULL);

    QCOMPARE(gPixmapsDrawn.count(), 1);

    QCOMPARE(gPixmapsDrawn.contains(loadedPixmaps.value(expectedFrame)), true);
    QRectF targetRect = gPixmapsDrawn.value(loadedPixmaps.value(expectedFrame)).first;
    QRectF sourceRect = gPixmapsDrawn.value(loadedPixmaps.value(expectedFrame)).second;
    QCOMPARE(targetRect, QRectF(QPointF(0, 0), m_subject->size()));
    QCOMPARE(sourceRect, QRectF(QPointF(0, 0), VALID_PIXMAP_SIZE));

    // Test draw with another frame
    currentFrame = 6;
    pixmapIndex = currentFrame + 1;
    expectedFrame = QString("%1_%2_%3").arg(expectedBaseImageName)\
                                                    .arg(expectedBaseImageSize)\
                                                    .arg(pixmapIndex);
    m_subject->setCurrentFrame(currentFrame);

    m_subject->drawContents(&painter, NULL);

    QCOMPARE(gPixmapsDrawn.count(), 2);
    QCOMPARE(gPixmapsDrawn.contains(loadedPixmaps.value(expectedFrame)), true);
}

void Ut_LauncherButtonProgressIndicatorView::testDrawingFramesWhenKnownDuration()
{
    const static int expectedFrames = 20;
    const static int expectedBaseImageSize = 78;
    const static QString expectedBaseImageName("spinnerbaseimage");
    m_subject->modifiableStyle()->setNumberOfFrames(expectedFrames);
    m_subject->modifiableStyle()->setBaseImageSize(expectedBaseImageSize);
    m_subject->modifiableStyle()->setBaseImageName(expectedBaseImageName);
    controller->setUnknownDuration(false);
    m_subject->applyStyle();

    int currentFrame = 6;
    int expectedDraws = currentFrame + 1;
    m_subject->setCurrentFrame(currentFrame);

    QPainter painter;
    m_subject->drawContents(&painter, NULL);

    for (int i = 1; i <= expectedDraws; i++) {
        QString expectedFrame = QString("%1_%2_%3").arg(expectedBaseImageName)\
                                                    .arg(expectedBaseImageSize)\
                                                    .arg(i);

        QCOMPARE(gPixmapsDrawn.contains(loadedPixmaps.value(expectedFrame)), true);
    }
    QCOMPARE(gPixmapsDrawn.count(), expectedDraws);
}

QTEST_APPLESS_MAIN(Ut_LauncherButtonProgressIndicatorView)
