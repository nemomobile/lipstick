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
#include "switcherviewbase.h"
#include "switcher.h"
#include "switcherbutton.h"
#include "transformlayoutanimation.h"
#include "mainwindow.h"

#include <MScene>
#include <MSceneManager>
#include <MViewCreator>
#include <MLayout>
#include <MTheme>
#include <MLocale>
#include <MApplication>
#include <MWindow>
#include <MDeviceProfile>
#include <QGraphicsLinearLayout>
#include <QPinchGesture>
#include <MPannableViewport>
#include <math.h>
#include <algorithm>
#include <cfloat>
#include <QGestureEvent>
#include <QPropertyAnimation>


SwitcherViewBase::SwitcherViewBase(Switcher *switcher) :
        MWidgetView(switcher), controller(switcher), mainLayout(new QGraphicsLinearLayout(Qt::Vertical)), pannedWidget(new MWidget), pinchedButtonPosition(-1), layoutAnimation(NULL), overpinch(false), animating(false)
{
    mainLayout->setContentsMargins(0, 0, 0, 0);
    switcher->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    switcher->setLayout(mainLayout);

    pannedLayout = new MLayout(pannedWidget);
    pannedLayout->setContentsMargins(0, 0, 0, 0);

    bounceAnimation = new QPropertyAnimation(this);
    bounceAnimation->setTargetObject(pannedWidget);
    bounceAnimation->setPropertyName("scale");
    bounceAnimation->setStartValue(1.0f);
    bounceAnimation->setEndValue(1.0f);
    connect(bounceAnimation, SIGNAL(finished()), this, SLOT(endBounce()));
    connect(bounceAnimation, SIGNAL(stateChanged(QAbstractAnimation::State,QAbstractAnimation::State)), this, SLOT(updateAnimationStatus()));

    connect(this, SIGNAL(animationStateChanged(bool)), switcher, SLOT(updateAnimationStatus(bool)));
}

SwitcherViewBase::~SwitcherViewBase()
{
    removeButtonsFromLayout();
}

void SwitcherViewBase::removeButtonsFromLayout()
{
    // Remove all buttons from the layout and set parents to null (do not destroy them)
    for (int i = 0, count = pannedLayout->count(); i < count; i++) {
        static_cast<SwitcherButton *>(pannedLayout->takeAt(0))->setParentItem(0);
    }
}

bool SwitcherViewBase::event(QEvent *e)
{
    // This stuff is necessary to receive touch events.
    if (e->type() == QEvent::TouchBegin) {
        e->setAccepted(true);

        return true;
    }

    return MWidgetView::event(e);
}

void SwitcherViewBase::setupModel()
{
    MWidgetView::setupModel();
    applySwitcherMode();
}

void SwitcherViewBase::applySwitcherMode()
{
    if (model()->switcherMode() == SwitcherModel::Detailview) {
        controller->setStyleName("DetailviewSwitcher");
    } else {
        controller->setStyleName("OverviewSwitcher");
    }
}

void SwitcherViewBase::updateData(const QList<const char*>& modifications)
{
    MWidgetView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == SwitcherModel::SwitcherMode) {
            applySwitcherMode();
        }
    }
}

int SwitcherViewBase::buttonIndex(const SwitcherButton *button) const
{
    if(button == NULL) {
        return -1;
    }

    QList<QSharedPointer<SwitcherButton> > buttons = model()->buttons();
    for (int i = 0; i < buttons.count(); ++i) {
        if (buttons.at(i) == button) {
            return i;
        }
    }

    return -1;
}

void SwitcherViewBase::calculateNearestButtonAt(const QPointF &centerPoint)
{
    float minDistance = FLT_MAX;
    SwitcherButton *closestButton = NULL;

    foreach (const QSharedPointer<SwitcherButton> &button, model()->buttons()) {
        QLineF vec(centerPoint, button->mapToItem(controller, button->rect().center()));

        float distance = vec.length();

        if(distance < minDistance) {
            minDistance   = distance;
            closestButton = button.data();
        }
    }

    pinchedButtonPosition = buttonIndex(closestButton);
}

void SwitcherViewBase::setViewportPhysicsEnabled(bool enabled)
{
    // When re-enabling, only enable the exact items that were disabled before
    static QList<QGraphicsItem*> items;

    if(!enabled) {
        MScene *scene = MainWindow::instance()->scene();
        items = scene->items(controller->mapToScene(controller->rect().center()));
    }

    foreach(QGraphicsItem *item, items) {
        if(MPannableViewport *vp = dynamic_cast<MPannableViewport *>(item)) {
            vp->physics()->setEnabled(enabled);
        }
    }

    if(enabled) {
        items.clear();
    }
}

void SwitcherViewBase::pinchBegin(const QPointF &centerPoint)
{
    // Prevent panning during the pinch
    setViewportPhysicsEnabled(false);

    calculateNearestButtonAt(centerPoint);

    foreach(const QSharedPointer<SwitcherButton> &button, model()->buttons()) {
        button->installSceneEventFilter(controller);
    }

}

void SwitcherViewBase::pinchUpdate(float scaleFactor)
{
    if(!layoutAnimation->isAnimating()) {
        if(scaleFactor == 1.0f) {
            return;
        }

        pinchGestureTargetMode = scaleFactor >= 1 ? SwitcherModel::Detailview : SwitcherModel::Overview;

        overpinch = pinchGestureTargetMode == model()->switcherMode();

        // Switch the mode and start the transition if needed
        if(model()->switcherMode() != pinchGestureTargetMode) {
            layoutAnimation->setManualControl(true);
            layoutAnimation->start();
            applyPinchGestureTargetMode();
        }
    }

    // Calculate the current animation progress based on the current scale factor
    qreal p = pinchGestureTargetMode == SwitcherModel::Detailview ?
              (scaleFactor - 1.0f) : (1.0f - scaleFactor);

    p = qBound(qreal(0.0), p * style()->pinchLength(), qreal(1));

    if(overpinch) {
        if(bounceAnimation->state() == QAbstractAnimation::Stopped) {
            setInwardBounceAnimation(model()->switcherMode() == SwitcherModel::Overview);
            bounceAnimation->setDirection(QAbstractAnimation::Forward);
            startBounceAnimation();
            bounceAnimation->pause();
        }

        bounceAnimation->setCurrentTime(p * bounceAnimation->duration() / 2);
    } else {
        layoutAnimation->setProgress(p);
    }
}

void SwitcherViewBase::pinchEnd()
{
    // Re-enable panning
    setViewportPhysicsEnabled(true);

    layoutAnimation->setManualControl(false);

    if(bounceAnimation->state() == QAbstractAnimation::Paused) {
        bounceAnimation->setDirection(QAbstractAnimation::Backward);
        bounceAnimation->resume();
    }

    // Cancel the transition if the pinch value plus twice the current pinching speed is less or equal to the threshold
    if(layoutAnimation->currentCurveValue() + layoutAnimation->speed() * 2.0f <= style()->pinchCancelThreshold()) {
        pinchGestureTargetMode = pinchGestureTargetMode == SwitcherModel::Detailview ? SwitcherModel::Overview : SwitcherModel::Detailview;
        layoutAnimation->cancelAnimation();
    }

    foreach (const QSharedPointer<SwitcherButton> &button, model()->buttons()) {
        button->setDown(false);
    }
}

void SwitcherViewBase::pinchGestureEvent(QGestureEvent *event, QPinchGesture *gesture)
{
    // For preventing acting on two GestureStarted events before receiving GestureFinished on the first,
    // which occurs sometimes (a bug in Qt).
    static bool gestureActive = false;

    // Finish any currently running animation before starting a new one
    if((layoutAnimation->isAnimating() && !layoutAnimation->manualControl()) || bounceAnimation->state() == QAbstractAnimation::Running) {
        return;
    }

    event->accept(gesture);

    static qreal pinchBeginLength = 0;

    switch(gesture->state()) {
    case Qt::GestureStarted:
        if(!gestureActive) {
            gestureActive = true;
            pinchBegin(controller->mapFromScene(gesture->centerPoint()));
            pinchBeginLength = QLineF(gesture->hotSpot(), gesture->centerPoint()).length() * 2;
        }

        break;
    case Qt::GestureUpdated:
        if(gestureActive) {
            QSize s = MDeviceProfile::instance()->resolution();
            qreal pinchDistance = s.width() * 3 / 4;
            qreal pinchLength = QLineF(gesture->hotSpot(), gesture->centerPoint()).length() * 2;
            qreal scale = qBound(qreal(-1), ((pinchLength - pinchBeginLength) / pinchDistance), qreal(1)) + 1.0f;

            pinchUpdate(scale);
        }
        break;
    case Qt::GestureFinished:
        if(gestureActive) {
            pinchEnd();
            gestureActive = false;
        }

        break;
    case Qt::GestureCanceled:
        gestureActive = false;
        break;
    default:
        break;
    }
}

bool SwitcherViewBase::sceneEventFilter(QGraphicsItem *watched, QEvent *event)
{
    bool filtered = false;

    if(event->type() == QEvent::GraphicsSceneMouseMove) {
        foreach(const QSharedPointer<SwitcherButton> &button, model()->buttons()) {
            if(button == watched) {
                filtered = true;
                break;
            }
        }
    }

    return filtered;
}

void SwitcherViewBase::endTransition()
{
    if(layoutAnimation->isCanceled()) {
        applyPinchGestureTargetMode();
    }

    layoutAnimation->stop();
}

void SwitcherViewBase::endBounce()
{
}

void SwitcherViewBase::applyPinchGestureTargetMode()
{
    model()->setSwitcherMode(pinchGestureTargetMode);
}

void SwitcherViewBase::setInwardBounceAnimation(bool i)
{
    // set the middle key value to either less than 1 when bouncing or zooming in overview mode,
    // or over 1 when zooming in detail mode
    bounceAnimation->setKeyValueAt(0.5f, 1.0f + (i ? -1.0f : 1.0f) * style()->bounceScale());
}

void SwitcherViewBase::startBounceAnimation()
{
    pannedWidget->setTransformOriginPoint(pannedWidget->mapFromParent(pannedWidget->parentWidget()->rect().center()));

    bounceAnimation->setDuration(style()->bounceDuration());
    bounceAnimation->setEasingCurve(style()->bounceCurve());
    bounceAnimation->start();
}

void SwitcherViewBase::runOverviewBounceAnimation()
{
    if(pinchGestureTargetMode == SwitcherModel::Overview) {
        setInwardBounceAnimation(true);
        startBounceAnimation();
    }
}

void SwitcherViewBase::updateAnimationStatus()
{
}

M_REGISTER_VIEW_NEW(SwitcherViewBase, Switcher)
