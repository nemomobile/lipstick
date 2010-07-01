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

#include <MApplication>
#include <MTheme>
#include <QPaintEngine>
#include <QPainter>
#include <QGraphicsPixmapItem>
#include "ut_plaindesktopbackgroundextension.h"
#include "plaindesktopbackgroundextension.h"
#include "plaindesktopbackgroundstyle.h"
#include "plaindesktopbackgroundpixmap.h"
#include "windowinfo_stub.h"

// PlainDesktopBackgroundPixmap stubs
QPixmap *PlainDesktopBackgroundPixmapConstructorPixmap;
QPixmap *PlainDesktopBackgroundPixmapConstructorDefocusedPixmap;
QString PlainDesktopBackgroundPixmapConstructorName;
QString PlainDesktopBackgroundPixmapConstructorDefaultName;
int PlainDesktopBackgroundPixmapConstructorBlurRadius;
qreal PlainDesktopBackgroundPixmapConstructorBrightness;
QString PlainDesktopBackgroundPixmapConstructorPixmapName;
PlainDesktopBackgroundPixmap::PlainDesktopBackgroundPixmap(const QString &name, const QString &defaultName, int blurRadius, qreal brightness) :
        pixmapFromTheme_(NULL)
{
    PlainDesktopBackgroundPixmapConstructorName = name;
    PlainDesktopBackgroundPixmapConstructorDefaultName = defaultName;
    PlainDesktopBackgroundPixmapConstructorBlurRadius = blurRadius;
    PlainDesktopBackgroundPixmapConstructorBrightness = brightness;
    pixmapFromFile_ = QSharedPointer<QPixmap>(PlainDesktopBackgroundPixmapConstructorPixmap);
    defocusedPixmap_ = QSharedPointer<QPixmap>(PlainDesktopBackgroundPixmapConstructorDefocusedPixmap);
    pixmapName_ = PlainDesktopBackgroundPixmapConstructorPixmapName;
}

PlainDesktopBackgroundPixmap::~PlainDesktopBackgroundPixmap()
{
    PlainDesktopBackgroundPixmapConstructorPixmap = NULL;
    PlainDesktopBackgroundPixmapConstructorDefocusedPixmap = NULL;
}

void PlainDesktopBackgroundPixmap::createDefocusedPixmap()
{
}

QPixmap *PlainDesktopBackgroundPixmap::createDefocusedPixmap(const QPixmap &, int, qreal)
{
    return NULL;
}

const QPixmap *PlainDesktopBackgroundPixmap::pixmap() const
{
    return pixmapFromFile_.data();
}

const QPixmap *PlainDesktopBackgroundPixmap::defocusedPixmap() const
{
    return defocusedPixmap_.data();
}

QString PlainDesktopBackgroundPixmap::pixmapName() const
{
    return pixmapName_;
}

// Mock Paint Engine
class MockPaintEngine : public QPaintEngine
{
    bool begin(QPaintDevice *pdev) {
        Q_UNUSED(pdev);
        return true;
    }
    bool end() {
        return true;
    }
    void updateState(const QPaintEngineState &state) {
        Q_UNUSED(state);
    }
    void drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr);
    Type type() const {
        return QPaintEngine::User;
    }
};

void MockPaintEngine::drawPixmap(const QRectF &, const QPixmap &, const QRectF &)
{
}

// Mock Paint Device
class MockPaintDevice : public QPaintDevice
{
public:
    MockPaintDevice() : engine(new MockPaintEngine) {}
    ~MockPaintDevice();

    QPaintEngine *paintEngine() const {
        return engine;
    }

    int metric(PaintDeviceMetric metric) const;
private:
    QPaintEngine *engine;
};

MockPaintDevice::~MockPaintDevice()
{
    delete engine;
}

int MockPaintDevice::metric(PaintDeviceMetric metric) const
{
    switch (metric) {
    case QPaintDevice::PdmWidth:
        return 1000;
    case QPaintDevice::PdmHeight:
        return 1000;
    case QPaintDevice::PdmDpiY:
    case QPaintDevice::PdmDpiX:
        return 300;
    default:
        return 0;
    }
    return 0;
}

// QPixmap stubs
QString QPixmapLoadFileName;
bool QPixmapLoadReturnValue;
bool QPixmap::load(const QString & fileName, const char *, Qt::ImageConversionFlags)
{
    QPixmapLoadFileName = fileName;
    return QPixmapLoadReturnValue;
}

// QTimeLine stubs
int QTimeLineDuration;
int QTimeLineUpdateInterval;
QTimeLine::Direction QTimeLineDirection;
QTimeLine::State QTimeLineState;
void QTimeLine::setDuration(int duration)
{
    QTimeLineDuration = duration;
}

void QTimeLine::setUpdateInterval(int interval)
{
    QTimeLineUpdateInterval = interval;
}

QTimeLine::Direction QTimeLine::direction() const
{
    return QTimeLineDirection;
}

void QTimeLine::toggleDirection()
{
    QTimeLineDirection = QTimeLineDirection == QTimeLine::Forward ? QTimeLine::Backward : QTimeLine::Forward;
}

void QTimeLine::resume()
{
    QTimeLineState = QTimeLine::Running;
}

QTimeLine::State QTimeLine::state() const
{
    return QTimeLineState;
}

// MTheme stubs
QString MThemePixmapId;
const QPixmap *MThemePixmapDefaultValue;
const QPixmap *MThemePixmapReturnValue;
const QPixmap *MTheme::pixmap(const QString &id, const QSize &)
{
    MThemePixmapId = id;
    return MThemePixmapReturnValue;
}

void MTheme::releasePixmap(const QPixmap *)
{
}

// QPainter stubs
qreal QPainterOpacity;
QRectF QPainterDrawPixmapTargetRect;
const QPixmap *QPainterDrawPixmapPixmap;
QRectF QPainterDrawPixmapSourceRect;
void QPainter::setOpacity(qreal opacity)
{
    QPainterOpacity = opacity;
}

void QPainter::drawPixmap(const QRectF &targetRect, const QPixmap &pixmap, const QRectF &sourceRect)
{
    QPainterDrawPixmapSourceRect = sourceRect;
    QPainterDrawPixmapPixmap = &pixmap;
    QPainterDrawPixmapTargetRect = QPainterDrawPixmapTargetRect.united(targetRect);
}

// MGConfItem stubs
QMap<QString, QVariant> MGConfItemValueForKey;
QVariant MGConfItem::value() const
{
    return MGConfItemValueForKey.value(key());
}

void MGConfItem::set(const QVariant &val)
{
    MGConfItemValueForKey.insert(key(), val);
}

// Helper for getting the style
PlainDesktopBackgroundStyle *modifiablePlainDesktopBackgroundStyle(M::Orientation orientation)
{
    return const_cast<PlainDesktopBackgroundStyle *>(static_cast<const PlainDesktopBackgroundStyle *> (MTheme::style("PlainDesktopBackgroundStyle", "", "", "", orientation, NULL)));
}

void Ut_PlainDesktopBackgroundExtension::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_plaindesktopbackgroundextension" };
    app = new MApplication(argc, app_name);
    paintDevice = new MockPaintDevice;
    painter = new QPainter(paintDevice);
    MThemePixmapDefaultValue = new QPixmap;
}

void Ut_PlainDesktopBackgroundExtension::cleanupTestCase()
{
    delete painter;
    delete paintDevice;
    delete app;
    delete MThemePixmapDefaultValue;
}

void Ut_PlainDesktopBackgroundExtension::init()
{
    updateCalled = false;
    currentOrientationAngle = M::Angle0;
    PlainDesktopBackgroundPixmapConstructorPixmap = NULL;
    PlainDesktopBackgroundPixmapConstructorDefocusedPixmap = NULL;
    PlainDesktopBackgroundPixmapConstructorPixmapName.clear();
    MThemePixmapReturnValue = MThemePixmapDefaultValue;
    MGConfItemValueForKey.clear();
    QTimeLineDirection = QTimeLine::Backward;
    QTimeLineState = QTimeLine::NotRunning;
    QPainterDrawPixmapTargetRect = QRectF();
    extension = new PlainDesktopBackgroundExtension;
    connect(this, SIGNAL(setDefocusFactor(qreal)), extension, SLOT(setDefocusFactor(qreal)));
    connect(this, SIGNAL(updateLandscapePixmap()), extension, SLOT(updateLandscapePixmap()));
    connect(this, SIGNAL(updatePortraitPixmap()), extension, SLOT(updatePortraitPixmap()));
    connect(this, SIGNAL(pixmapUpdated()), extension, SLOT(updateDesktop()));
}

void Ut_PlainDesktopBackgroundExtension::cleanup()
{
    delete extension;
}

void Ut_PlainDesktopBackgroundExtension::testInitialize()
{
    PlainDesktopBackgroundStyle *landscapeStyle = modifiablePlainDesktopBackgroundStyle(M::Landscape);
    landscapeStyle->setDefocusDuration(3);
    landscapeStyle->setDefocusUpdateInterval(4);

    QCOMPARE(extension->initialize(""), true);
    QCOMPARE(QTimeLineDuration, landscapeStyle->defocusDuration());
    QCOMPARE(QTimeLineUpdateInterval, landscapeStyle->defocusUpdateInterval());
}

void Ut_PlainDesktopBackgroundExtension::testWidget()
{
    QCOMPARE(extension->widget(), (QGraphicsWidget *)NULL);
}

void Ut_PlainDesktopBackgroundExtension::testDrawBackground()
{
    QCOMPARE(extension->initialize(""), true);
    extension->setDesktopInterface(*this);
    QRectF br(10, 10, 50, 30);

    // Set up the pixmaps
    PlainDesktopBackgroundPixmapConstructorPixmap = new QPixmap(1, 1);
    PlainDesktopBackgroundPixmapConstructorDefocusedPixmap = new QPixmap(1, 1);
    const QPixmap *landscapePixmap = PlainDesktopBackgroundPixmapConstructorPixmap;
    emit updateLandscapePixmap();

    PlainDesktopBackgroundPixmapConstructorPixmap = new QPixmap(1, 1);
    PlainDesktopBackgroundPixmapConstructorDefocusedPixmap = new QPixmap(1, 1);
    const QPixmap *portraitPixmap = PlainDesktopBackgroundPixmapConstructorPixmap;
    emit updatePortraitPixmap();

    // Verify that the correct pixmaps are drawn in each orientation
    currentOrientationAngle = M::Angle0;
    extension->drawBackground(painter, br);
    QCOMPARE(QPainterDrawPixmapPixmap, landscapePixmap);

    currentOrientationAngle = M::Angle90;
    extension->drawBackground(painter, br);
    QCOMPARE(QPainterDrawPixmapPixmap, portraitPixmap);

    currentOrientationAngle = M::Angle180;
    extension->drawBackground(painter, br);
    QCOMPARE(QPainterDrawPixmapPixmap, landscapePixmap);

    currentOrientationAngle = M::Angle270;
    extension->drawBackground(painter, br);
    QCOMPARE(QPainterDrawPixmapPixmap, portraitPixmap);

    // Check that the extension doesn't draw outside the bounding rectangle
    QVERIFY(QPainterDrawPixmapTargetRect.isEmpty() || QPainterDrawPixmapTargetRect == br || br.contains(QPainterDrawPixmapTargetRect));
}

void Ut_PlainDesktopBackgroundExtension::testUpdatePixmaps()
{
    // Set up the style for default values
    PlainDesktopBackgroundStyle *landscapeStyle = modifiablePlainDesktopBackgroundStyle(M::Landscape);
    PlainDesktopBackgroundStyle *portraitStyle = modifiablePlainDesktopBackgroundStyle(M::Portrait);
    landscapeStyle->setBlurRadius(27);
    landscapeStyle->setBrightness(0.2);
    landscapeStyle->setDefaultBackgroundImage("landscapeDefault");
    portraitStyle->setDefaultBackgroundImage("portraitDefault");

    // Set up GConf values
    QString landscapeName("landscape");
    QString portraitName("landscape");
    MGConfItemValueForKey.insert("/desktop/meego/background/landscape/picture_filename", landscapeName);
    MGConfItemValueForKey.insert("/desktop/meego/background/portrait/picture_filename", portraitName);
    QCOMPARE(extension->initialize(""), true);
    extension->setDesktopInterface(*this);

    // Set up the pixmaps
    PlainDesktopBackgroundPixmapConstructorPixmap = new QPixmap(1, 1);
    PlainDesktopBackgroundPixmapConstructorDefocusedPixmap = new QPixmap(1, 1);
    emit updateLandscapePixmap();
    QCOMPARE(PlainDesktopBackgroundPixmapConstructorName, landscapeName);
    QCOMPARE(PlainDesktopBackgroundPixmapConstructorDefaultName, landscapeStyle->defaultBackgroundImage());
    QCOMPARE(PlainDesktopBackgroundPixmapConstructorBlurRadius, landscapeStyle->blurRadius());
    QCOMPARE(PlainDesktopBackgroundPixmapConstructorBrightness, landscapeStyle->brightness());
    QVERIFY(updateCalled);

    updateCalled = false;
    PlainDesktopBackgroundPixmapConstructorPixmap = new QPixmap(1, 1);
    PlainDesktopBackgroundPixmapConstructorDefocusedPixmap = new QPixmap(1, 1);
    emit updatePortraitPixmap();
    QCOMPARE(PlainDesktopBackgroundPixmapConstructorName, portraitName);
    QCOMPARE(PlainDesktopBackgroundPixmapConstructorDefaultName, portraitStyle->defaultBackgroundImage());
    QCOMPARE(PlainDesktopBackgroundPixmapConstructorBlurRadius, landscapeStyle->blurRadius());
    QCOMPARE(PlainDesktopBackgroundPixmapConstructorBrightness, landscapeStyle->brightness());
    QVERIFY(updateCalled);

    MTheme::releaseStyle(landscapeStyle);
    MTheme::releaseStyle(portraitStyle);
}

void Ut_PlainDesktopBackgroundExtension::testUpdatePixmapsFails()
{
    // Set up GConf values
    QCOMPARE(extension->initialize(""), true);

    // Set up the pixmaps
    PlainDesktopBackgroundPixmapConstructorPixmapName = "testPixmap";
    PlainDesktopBackgroundPixmapConstructorPixmap = new QPixmap(1, 1);
    PlainDesktopBackgroundPixmapConstructorDefocusedPixmap = new QPixmap(1, 1);
    emit updateLandscapePixmap();

    // Set up the pixmaps again so that they fail
    PlainDesktopBackgroundPixmapConstructorPixmapName = "unexpectedPixmap";
    PlainDesktopBackgroundPixmapConstructorPixmap = new QPixmap(1, 1);
    PlainDesktopBackgroundPixmapConstructorDefocusedPixmap = new QPixmap(1, 1);
    emit updateLandscapePixmap();

    // Check that the GConf key was set to the previous value
    QCOMPARE(MGConfItemValueForKey.value("/desktop/meego/background/landscape/picture_filename"), QVariant("testPixmap"));

    // Set up the pixmaps again using an empty name (which is valid and should use the default)
    MGConfItemValueForKey.insert("/desktop/meego/background/landscape/picture_filename", QString());
    PlainDesktopBackgroundPixmapConstructorPixmapName = QString();
    PlainDesktopBackgroundPixmapConstructorPixmap = new QPixmap(1, 1);
    PlainDesktopBackgroundPixmapConstructorDefocusedPixmap = new QPixmap(1, 1);
    emit updateLandscapePixmap();

    // Check that the GConf key was not set to the previous value
    QCOMPARE(MGConfItemValueForKey.value("/desktop/meego/background/landscape/picture_filename"), QVariant(QString()));
}

void Ut_PlainDesktopBackgroundExtension::testSetDefocusFactor()
{
    QCOMPARE(extension->initialize(""), true);
    extension->setDesktopInterface(*this);

    // Set up the pixmaps
    PlainDesktopBackgroundPixmapConstructorPixmap = new QPixmap(1, 1);
    PlainDesktopBackgroundPixmapConstructorDefocusedPixmap = new QPixmap(1, 1);
    emit updateLandscapePixmap();

    // This should call MDesktopInterface::update() and painting (called by update()) should use the blur factor
    qreal blurFactor = 0.5;
    emit setDefocusFactor(blurFactor);
    QVERIFY(updateCalled);
    QCOMPARE(QPainterOpacity, blurFactor);

    // Setting the same blur factor again should not result in updates
    updateCalled = false;
    emit setDefocusFactor(blurFactor);
    QVERIFY(!updateCalled);
}

void Ut_PlainDesktopBackgroundExtension::testSetDefocused()
{
    // Test that the timeline direction is set and the timeline is resumed
    extension->setDefocused(true);
    QCOMPARE(QTimeLineDirection, QTimeLine::Forward);
    QCOMPARE(QTimeLineState, QTimeLine::Running);

    // Test that the timeline is resumed if it's not running even if the direction doesn't change
    QTimeLineState = QTimeLine::NotRunning;
    extension->setDefocused(true);
    QCOMPARE(QTimeLineDirection, QTimeLine::Forward);
    QCOMPARE(QTimeLineState, QTimeLine::Running);

    // Test that the timeline direction is set when it changes and the timeline is resumed
    QTimeLineState = QTimeLine::NotRunning;
    extension->setDefocused(false);
    QCOMPARE(QTimeLineDirection, QTimeLine::Backward);
    QCOMPARE(QTimeLineState, QTimeLine::Running);

    // Test that the timeline is resumed if it's not running even if the direction doesn't change
    QTimeLineState = QTimeLine::NotRunning;
    extension->setDefocused(false);
    QCOMPARE(QTimeLineDirection, QTimeLine::Backward);
    QCOMPARE(QTimeLineState, QTimeLine::Running);
}

void Ut_PlainDesktopBackgroundExtension::testUpdateDesktop()
{
    QCOMPARE(extension->initialize(""), true);
    extension->setDesktopInterface(*this);
    emit pixmapUpdated();
    QVERIFY(updateCalled);
}

void Ut_PlainDesktopBackgroundExtension::testUpdateDesktopNoDesktop()
{
    QCOMPARE(extension->initialize(""), true);
    emit pixmapUpdated();
    QVERIFY(!updateCalled);
}

void Ut_PlainDesktopBackgroundExtension::update()
{
    updateCalled = true;
    QRectF br(10, 10, 50, 30);
    extension->drawBackground(painter, br);
}

M::OrientationAngle Ut_PlainDesktopBackgroundExtension::orientationAngle()
{
    return currentOrientationAngle;
}

QTEST_APPLESS_MAIN(Ut_PlainDesktopBackgroundExtension)
