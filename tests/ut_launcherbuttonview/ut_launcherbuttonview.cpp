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
#include <duiapplication.h>
#include <DuiScalableImage>
#include <QGraphicsSceneMouseEvent>
#include "ut_launcherbuttonview.h"
#include "launcherbuttonview.h"
#include "launcherbutton.h"
#include "launcherbuttonstyle.h"
#include "../stubs/duifeedback_stub.h"

QList<const DuiScalableImage *> Ut_LauncherButtonView::drawnScalableImages;
QList<QString> Ut_LauncherButtonView::availableThumbnails;

class TestLauncherButtonView : public LauncherButtonView
{
    DUI_VIEW(LauncherButtonModel, LauncherButtonStyle)
public:
    TestLauncherButtonView(LauncherButton *button) : LauncherButtonView(button) {}

    void drawContents(QPainter *p, const QStyleOptionGraphicsItem *i) const {
        LauncherButtonView::drawContents(p, i);
    }

    void setInvalidShortcutContainerImage(DuiScalableImage *i) {
        modifiableStyle()->setInvalidShortcutContainerImage(i);
    }

    void setShortcutContainerImage(DuiScalableImage *i) {
        modifiableStyle()->setShortcutContainerImage(i);
    }

    void sendMousePressEventToSelf() {
        QGraphicsSceneMouseEvent ev(QEvent::GraphicsSceneMousePress);
        this->mousePressEvent(&ev);
    }

    void sendMouseReleaseEventToSelf() {
        QGraphicsSceneMouseEvent ev(QEvent::GraphicsSceneMouseRelease);
        this->mouseReleaseEvent(&ev);
    }

    void setShortcutThumbnailSize(QSizeF sf) {
        modifiableStyle()->setShortcutThumbnailSize(sf);
    }

private:
    LauncherButtonStyle *modifiableStyle() {
        LauncherButtonStyleContainer &sc = style();
        const LauncherButtonStyle *const_s = sc.operator ->();
        LauncherButtonStyle *s = const_cast<LauncherButtonStyle *>(const_s);
        return s;
    }
};

// QPixmap stubs used by LauncherButtonView
bool QPixmap::load(const QString &fileName, const char *format, Qt::ImageConversionFlags flags)
{
    Q_UNUSED(format);
    Q_UNUSED(flags);
    if (Ut_LauncherButtonView::availableThumbnails.contains(fileName)) {
        return true;
    }
    return false;
}

// DuiScalableImage stubs used by LauncherButtonView
void DuiScalableImage::draw(const QRect &rect, QPainter *painter) const
{
    Q_UNUSED(rect);
    Q_UNUSED(painter);
    Ut_LauncherButtonView::drawnScalableImages.append(this);
}

bool duiButtonIconViewDrawCalled;
void DuiButtonIconView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    duiButtonIconViewDrawCalled = true;
}

bool duiButtonViewButtonMousePressEventCalled;
void DuiButtonView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    duiButtonViewButtonMousePressEventCalled = true;
}

// Tests
void Ut_LauncherButtonView::initTestCase()
{
    // Create a DuiAapplication
    static int argc = 1;
    static char *app_name = (char *)"./Ut_LauncherButtonView";
    app = new DuiApplication(argc, &app_name);
}

void Ut_LauncherButtonView::cleanupTestCase()
{
    // Destroy DuiApplication
    delete app;
}

void Ut_LauncherButtonView::init()
{
    drawnScalableImages.clear();
    availableThumbnails.clear();

    // Create a launcher button view
    button = new LauncherButton(0);
    view = new TestLauncherButtonView(button);
    button->setView(view);
}

void Ut_LauncherButtonView::cleanup()
{
    delete button;
    button = NULL;
    view = NULL;
}

void Ut_LauncherButtonView::testValidShortcut()
{
    QPainter p;

    // Set launcher button view style parameters
    DuiScalableImage *validContainerImage = new DuiScalableImage;
    view->setShortcutThumbnailSize(QSizeF(256, 256));
    view->setShortcutContainerImage(validContainerImage);

    // Create available thumbnail and configure launcher button model
    availableThumbnails.append("/home/root/.thumbnails/exists.png");
    button->model()->setTargetType("Link");
    button->model()->setThumbnail("/home/root/.thumbnails/exists.png");

    // Draw launcher button view and verify that correct container image was used.
    view->drawContents(&p, NULL);
    QCOMPARE(drawnScalableImages.count(), 1);
    QCOMPARE(drawnScalableImages.at(0), validContainerImage);
}

void Ut_LauncherButtonView::testInvalidShortcut()
{
    QPainter p;

    // Set launcher button view style parameters
    DuiScalableImage *validContainerImage = new DuiScalableImage;
    DuiScalableImage *invalidContainerImage = new DuiScalableImage;
    view->setShortcutContainerImage(validContainerImage);
    view->setInvalidShortcutContainerImage(invalidContainerImage);
    view->setShortcutThumbnailSize(QSizeF(256, 256));

    // Create available thumbnail and configure launcher button model
    availableThumbnails.append("/home/root/.thumbnails/exists.png");
    button->model()->setTargetType("Link");

    // Test with launcher button that has an unset thumbnail image
    view->drawContents(&p, NULL);
    QCOMPARE(drawnScalableImages.count(), 1);
    QCOMPARE(drawnScalableImages.at(0), invalidContainerImage);
    drawnScalableImages.clear();

    // Test with launcher button that has an invalid thumbnail image
    button->model()->setThumbnail("/home/root/.thumbnails/invalid.png");

    // Test with launcher button that has an unset thumbnail image
    view->drawContents(&p, NULL);
    QCOMPARE(drawnScalableImages.count(), 1);
    QCOMPARE(drawnScalableImages.at(0), invalidContainerImage);
}

void Ut_LauncherButtonView::testButtonPressedFeedbacks()
{
    QPainter p;

    button->setTargetType("Application");

    view->drawContents(&p, NULL);

    duiButtonIconViewDrawCalled = false;
    duiButtonViewButtonMousePressEventCalled = false;

    view->sendMousePressEventToSelf();
    view->drawContents(&p, NULL);

    // verify that parent draw from DuiButtonIconView (responsible for visual feedback) was called
    QVERIFY(duiButtonIconViewDrawCalled);
    // verify that mousepressevent is forwarded to DuiButtonView (responsible for audio/haptics feedback)
    QVERIFY(duiButtonViewButtonMousePressEventCalled);
}

QTEST_APPLESS_MAIN(Ut_LauncherButtonView)
