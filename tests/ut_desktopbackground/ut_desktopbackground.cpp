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

#include <MGConfItem>
#include <MApplication>

#include "desktopbackground.h"
#include "desktopstyle.h"
#include "ut_desktopbackground.h"
#include "mscenemanager_stub.h"
#include "mainwindow_stub.h"


static QMap<QString, QVariant> gGConfToFileMapping;
static QMap<QString, QPixmap*> gFileToPixmapMapping;
static QPixmap* gPortraitPixmap;
static QPixmap* gLandscapePixmap;
static QPixmap* gInvalidPixmap;
static const int REPEAT_COUNT = 4;
static const int STRESS_REPEAT_COUNT = 10;
static QSize PORTRAIT_SIZE(480, 864);
static QSize LANDSCAPE_SIZE(480, 864);
static const QString PORTRAIT_FILENAME = "portrait.png";
static const QString LANDSCAPE_FILENAME = "landscape.png";
static const QString PORTRAIT_GCONF_KEY =
    "/desktop/meego/background/portrait/picture_filename";
static const QString LANDSCAPE_GCONF_KEY =
    "/desktop/meego/background/landscape/picture_filename";

class LocalSceneManagerStub : public MSceneManagerStub
{
    virtual QSize visibleSceneSize(M::Orientation orientation) const
        {
            MSceneManagerStub::visibleSceneSize(orientation);
            if (orientation == M::Portrait) {
                return PORTRAIT_SIZE;
            } else {
                return LANDSCAPE_SIZE;
            }
        }
};

LocalSceneManagerStub localSceneManagerStub;

// MGConfItem stubs
QVariant MGConfItem::value() const
{
    return gGConfToFileMapping[key()];
}

void MGConfItem::set(const QVariant& val)
{
    gGConfToFileMapping[key()] = val;
}

// QPixmap stubs
bool QPixmap::load(const QString &fileName,
                   const char */*format*/,
                   Qt::ImageConversionFlags /*flags*/)
{
    QPixmap* tmp = gFileToPixmapMapping[fileName];
    if (tmp && (tmp != gInvalidPixmap)) {
        *this = *tmp;
        return true;
    }
    return false;
}

static void resetMappings()
{
    gGConfToFileMapping[PORTRAIT_GCONF_KEY] =
        QVariant(QString(PORTRAIT_FILENAME));
    gGConfToFileMapping[LANDSCAPE_GCONF_KEY] =
        QVariant(QString(LANDSCAPE_FILENAME));
    gFileToPixmapMapping[PORTRAIT_FILENAME] = gPortraitPixmap;
    gFileToPixmapMapping[LANDSCAPE_FILENAME] = gLandscapePixmap;
}

void Ut_DesktopBackground::initTestCase()
{
    resetMappings();
}

void Ut_DesktopBackground::cleanupTestCase()
{

}

void Ut_DesktopBackground::init()
{
    gMSceneManagerStub = &localSceneManagerStub;
    static int argc = 1;
    static char *app_name = (char *)"./ut_mainwindow";
    // an app is required before pixmaps can be created,
    // otherwise there's no paintdevice for the pixmaps
    app_ = new MApplication(argc, &app_name);
    gPortraitPixmap = new QPixmap(PORTRAIT_SIZE);
    gLandscapePixmap = new QPixmap(LANDSCAPE_SIZE);
    gInvalidPixmap = new QPixmap;
    style_ = new DesktopStyleContainer;
    resetMappings();
    subject_ = new DesktopBackground(NULL);
    connect(this, SIGNAL(portraitImageChanged()),
            subject_, SLOT(reloadPortraitImage()));
    connect(this, SIGNAL(landscapeImageChanged()),
            subject_, SLOT(reloadLandscapeImage()));
 }

void Ut_DesktopBackground::cleanup()
{
    delete subject_;
    // the pixmaps must be deleted before the app,
    // otherwise there's a leak
    delete gPortraitPixmap;
    delete gLandscapePixmap;
    delete gInvalidPixmap;
    delete style_;
    delete app_;
}

void Ut_DesktopBackground::testConstructionWithCorrectPixmaps()
{
    DesktopBackground testBackground(NULL);
    // background has been constructed with both pixmaps
    // and the size of the pixmaps match the scene sizes
    QPixmap pixmap = testBackground.backgroundImage(*style_);
    QCOMPARE(pixmap.isNull(), false);
    QCOMPARE(pixmap, *gPortraitPixmap);
    QCOMPARE(testBackground.portraitBackgroundImageConfig_.value().toString(),
             QString(PORTRAIT_FILENAME));
    QCOMPARE(testBackground.landscapeBackgroundImageConfig_.value().toString(),
             QString(LANDSCAPE_FILENAME));

}

static bool isEven(int i)
{
    return i%2;
}

void Ut_DesktopBackground::testConstructionWithCorrectPixmapsWithCurrentOrientationChanging()
{
    for (int i = 0; i < REPEAT_COUNT; ++i) {
        if (isEven(i)) {
            gMSceneManagerStub->stubSetReturnValue("orientation", M::Landscape);
        } else {
            gMSceneManagerStub->stubSetReturnValue("orientation", M::Portrait);
        }
        // background has been constructed with both pixmaps
        // and the size of the pixmaps match the scene sizes
        DesktopBackground testBackground(NULL);
        QPixmap pixmap = testBackground.backgroundImage(*style_);
        QCOMPARE(pixmap.isNull(), false);
        if (isEven(i)) {
            QCOMPARE(pixmap, *gLandscapePixmap);
        } else {
            QCOMPARE(pixmap, *gPortraitPixmap);
        }
        QCOMPARE(
            testBackground.portraitBackgroundImageConfig_.value().toString(),
                 QString(PORTRAIT_FILENAME));
        QCOMPARE(
            testBackground.landscapeBackgroundImageConfig_.value().toString(),
                 QString(LANDSCAPE_FILENAME));
    }
}

void Ut_DesktopBackground::testConstructionWithIncorrectPortraitPixmap()
{
    gMSceneManagerStub->stubSetReturnValue("orientation", M::Portrait);
    gFileToPixmapMapping[PORTRAIT_FILENAME] = gInvalidPixmap;
    // background gets constructed with a portrait pixmap the size of which
    // does _not_ match the scene size
    DesktopBackground testBackground(NULL);
    QPixmap pixmap = testBackground.backgroundImage(*style_);
    QCOMPARE(pixmap.isNull(), true);
    QCOMPARE(testBackground.portraitBackgroundImageConfig_.value().toString(),
             QString(""));
    QCOMPARE(testBackground.landscapeBackgroundImageConfig_.value().toString(),
             QString(LANDSCAPE_FILENAME));
    gFileToPixmapMapping[PORTRAIT_FILENAME] = gPortraitPixmap;

}

void Ut_DesktopBackground::testConstructionWithIncorrectLandscapePixmap()
{
    gMSceneManagerStub->stubSetReturnValue("orientation", M::Landscape);
    gFileToPixmapMapping[LANDSCAPE_FILENAME] = gInvalidPixmap;
    // background gets constructed with a landscape pixmap the size of which
    // does _not_ match the scene size
    DesktopBackground testBackground(NULL);
    QPixmap pixmap = testBackground.backgroundImage(*style_);
    QCOMPARE(pixmap.isNull(), true);
    QCOMPARE(testBackground.portraitBackgroundImageConfig_.value().toString(),
             QString(PORTRAIT_FILENAME));
    QCOMPARE(testBackground.landscapeBackgroundImageConfig_.value().toString(),
             QString(""));
    gFileToPixmapMapping[LANDSCAPE_FILENAME] = gLandscapePixmap;

}

void Ut_DesktopBackground::testConstructionWithIncorrectPixmaps()
{
    gFileToPixmapMapping[LANDSCAPE_FILENAME] = gInvalidPixmap;
    gFileToPixmapMapping[PORTRAIT_FILENAME] = gInvalidPixmap;
    // third gets constructed with both pixmaps invalid
    DesktopBackground testBackground(NULL);
    QPixmap pixmap = testBackground.backgroundImage(*style_);
    QCOMPARE(pixmap.isNull(), true);
    QCOMPARE(testBackground.portraitBackgroundImageConfig_.value().toString(),
             QString(""));
    QCOMPARE(testBackground.landscapeBackgroundImageConfig_.value().toString(),
             QString(""));
    gFileToPixmapMapping[LANDSCAPE_FILENAME] = gLandscapePixmap;
    gFileToPixmapMapping[PORTRAIT_FILENAME] = gPortraitPixmap;
}

void Ut_DesktopBackground::testOrientationChangeAfterConstruction()
{
    for (int i = 0; i < REPEAT_COUNT; ++i) {
        if (isEven(i)) {
            gMSceneManagerStub->stubSetReturnValue("orientation", M::Landscape);
            QPixmap pixmap = subject_->backgroundImage(*style_);
            QCOMPARE(pixmap, *gLandscapePixmap);
        } else {
            gMSceneManagerStub->stubSetReturnValue("orientation", M::Portrait);
            QPixmap pixmap = subject_->backgroundImage(*style_);
            QCOMPARE(pixmap, *gPortraitPixmap);
        }
    }
}

void Ut_DesktopBackground::testPortraitImageModification()
{
    gMSceneManagerStub->stubSetReturnValue("orientation", M::Portrait);
    QSignalSpy spy(subject_, SIGNAL(backgroundImageChanged()));
    QPixmap originalImage = subject_->backgroundImage(*style_);
    QPixmap changedPixmap(PORTRAIT_SIZE);
    changedPixmap.fill();
    gFileToPixmapMapping[PORTRAIT_FILENAME] = &changedPixmap;
    emit portraitImageChanged();
    QPixmap newPixmap = subject_->backgroundImage(*style_);
    QCOMPARE(newPixmap, changedPixmap);
    QCOMPARE(spy.count(), 1);
    gFileToPixmapMapping[PORTRAIT_FILENAME] = gPortraitPixmap;
    gMSceneManagerStub->stubSetReturnValue("orientation", M::Landscape);
    emit portraitImageChanged();
    QCOMPARE(spy.count(), 1);
    newPixmap = subject_->backgroundImage(*style_);
    QCOMPARE(newPixmap, *gLandscapePixmap);
}

static bool isOddAndThird(int i)
{
    return (!(i%2) && i%3);
}

static void setupStressTestValues(int i, M::Orientation orientationForTest)
{
    if (isEven(i)) {
        if (orientationForTest == M::Portrait) {
            gMSceneManagerStub->stubSetReturnValue("orientation", M::Landscape);
        } else {
            gMSceneManagerStub->stubSetReturnValue("orientation", M::Portrait);
        }
    } else {
        if (orientationForTest == M::Portrait) {
            gMSceneManagerStub->stubSetReturnValue("orientation", M::Portrait);
        } else {
            gMSceneManagerStub->stubSetReturnValue("orientation", M::Landscape);
        }
    }
    if (isOddAndThird(i)) {
        if (orientationForTest == M::Portrait) {
            gFileToPixmapMapping[PORTRAIT_FILENAME] = gInvalidPixmap;
        } else {
            gFileToPixmapMapping[LANDSCAPE_FILENAME] = gInvalidPixmap;
        }
    } else {
        if (orientationForTest == M::Portrait) {
            gFileToPixmapMapping[PORTRAIT_FILENAME] = gPortraitPixmap;
        } else {
            gFileToPixmapMapping[LANDSCAPE_FILENAME] = gPortraitPixmap;
        }
    }
}

static void testStressTestValues(int i, QSignalSpy& spy,
                                 QPixmap& originalImage,
                                 QPixmap& newImage,
                                 M::Orientation orientationToTest)
{
    if (isOddAndThird(i)) {
        QCOMPARE(spy.count(), 0);
        QCOMPARE(originalImage, newImage);
    } else if (isEven(i)) {
        QCOMPARE(spy.count(), 0);
        if (orientationToTest == M::Portrait) {
            QCOMPARE(newImage, *gLandscapePixmap);
        } else {
            QCOMPARE(newImage, *gPortraitPixmap);
        }
    } else {
        QCOMPARE(spy.count(), 1);
        if (orientationToTest == M::Portrait) {
            QCOMPARE(newImage, *gPortraitPixmap);
        } else {
            QCOMPARE(newImage, *gLandscapePixmap);
        }
    }
}

void Ut_DesktopBackground::testPortraitImageModifications()
{
    // stress it a bit
    gMSceneManagerStub->stubSetReturnValue("orientation", M::Portrait);
    QPixmap originalImage = subject_->backgroundImage(*style_);
    for (int i = 0; i < STRESS_REPEAT_COUNT; ++i) {
        QSignalSpy spy(subject_, SIGNAL(backgroundImageChanged()));
        setupStressTestValues(i, M::Portrait);
        emit portraitImageChanged();
        QPixmap newImage = subject_->backgroundImage(*style_);
        testStressTestValues(i, spy,
                             originalImage,
                             newImage,
                             M::Portrait);
        resetMappings();
    }
}

void Ut_DesktopBackground::testLandscapeImageModification()
{
    gMSceneManagerStub->stubSetReturnValue("orientation", M::Landscape);
    QSignalSpy spy(subject_, SIGNAL(backgroundImageChanged()));
    QPixmap originalImage = subject_->backgroundImage(*style_);
    QPixmap changedPixmap(LANDSCAPE_SIZE);
    changedPixmap.fill();
    gFileToPixmapMapping[LANDSCAPE_FILENAME] = &changedPixmap;
    emit landscapeImageChanged();
    QPixmap newPixmap = subject_->backgroundImage(*style_);
    QCOMPARE(newPixmap, changedPixmap);
    QCOMPARE(spy.count(), 1);
    gFileToPixmapMapping[LANDSCAPE_FILENAME] = gLandscapePixmap;
    gMSceneManagerStub->stubSetReturnValue("orientation", M::Portrait);
    emit landscapeImageChanged();
    QCOMPARE(spy.count(), 1);
    newPixmap = subject_->backgroundImage(*style_);
    QCOMPARE(newPixmap, *gPortraitPixmap);
}

void Ut_DesktopBackground::testLandscapeImageModifications()
{
    // stress it a bit
    gMSceneManagerStub->stubSetReturnValue("orientation", M::Landscape);
    QPixmap originalImage = subject_->backgroundImage(*style_);
    for (int i = 0; i < STRESS_REPEAT_COUNT; ++i) {
        QSignalSpy spy(subject_, SIGNAL(backgroundImageChanged()));
        setupStressTestValues(i, M::Landscape);
        emit landscapeImageChanged();
        QPixmap newImage = subject_->backgroundImage(*style_);
        testStressTestValues(i, spy,
                             originalImage,
                             newImage,
                             M::Landscape);
        resetMappings();
    }
}

void Ut_DesktopBackground::testGConfValueChanges()
{
    gMSceneManagerStub->stubSetReturnValue("orientation", M::Landscape);
    emit landscapeImageChanged();
    QCOMPARE(subject_->landscapeBackgroundImageConfig_.value().toString(),
             LANDSCAPE_FILENAME);
    gGConfToFileMapping[LANDSCAPE_GCONF_KEY] = QString();
    emit landscapeImageChanged();
    QCOMPARE(subject_->landscapeBackgroundImageConfig_.value().toString(),
             QString(""));
    gGConfToFileMapping[LANDSCAPE_GCONF_KEY] = LANDSCAPE_FILENAME;
    gFileToPixmapMapping[LANDSCAPE_FILENAME] = gInvalidPixmap;
    emit landscapeImageChanged();
    QCOMPARE(subject_->landscapeBackgroundImageConfig_.value().toString(),
             QString(""));
    gMSceneManagerStub->stubSetReturnValue("orientation", M::Portrait);
    emit portraitImageChanged();
    QCOMPARE(subject_->portraitBackgroundImageConfig_.value().toString(),
             PORTRAIT_FILENAME);
    gGConfToFileMapping[PORTRAIT_GCONF_KEY] = QString();
    emit portraitImageChanged();
    QCOMPARE(subject_->portraitBackgroundImageConfig_.value().toString(),
             QString(""));
    gGConfToFileMapping[PORTRAIT_GCONF_KEY] = PORTRAIT_FILENAME;
    gFileToPixmapMapping[PORTRAIT_FILENAME] = gInvalidPixmap;
    emit portraitImageChanged();
    QCOMPARE(subject_->portraitBackgroundImageConfig_.value().toString(),
             QString(""));
}

QTEST_APPLESS_MAIN(Ut_DesktopBackground)
