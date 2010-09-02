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

#include "switcherview.h"
#include "switcher.h"
#include "switcherbutton.h"
#include "transformlayoutanimation.h"
#include "pagedpanning.h"
#include "mainwindow.h"

#include <MSceneManager>
#include <MViewCreator>
#include <MLayout>
#include <MTheme>
#include <MLocale>
#include <MApplication>
#include <MWindow>
#include <MFlowLayoutPolicy>
#include <MAbstractLayoutPolicyStyle>
#include <MLinearLayoutPolicy>
#include <MGridLayoutPolicy>
#include <MDeviceProfile>
#include <QTimeLine>
#include <QGraphicsLinearLayout>
#include <QPinchGesture>
#include <MPositionIndicator>
#include "pagepositionindicatorview.h"
#include <math.h>
#include <algorithm>
#include "pagedviewport.h"
#include <QGestureEvent>
#include <QPropertyAnimation>

static const qreal HALF_PI = M_PI / 2.0;
static const qreal MAX_Z_VALUE = 1.0;


SwitcherView::SwitcherView(Switcher *switcher) :
        MWidgetView(switcher), controller(switcher), mainLayout(new QGraphicsLinearLayout(Qt::Vertical)), pannedWidget(new MWidget), viewport(new PagedViewport),
        overviewStyle(0)
{
    mainLayout->setContentsMargins(0, 0, 0, 0);
    switcher->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    switcher->setLayout(mainLayout);

    connect(viewport, SIGNAL(pageChanged(int)), this, SLOT(updateFocusedButton(int)));
    connect(viewport, SIGNAL(panningStopped()), this, SLOT(panningStopped()));
    connect(MainWindow::instance()->sceneManager(), SIGNAL(orientationChanged(M::Orientation)), this, SLOT(updateButtons()));

    // We have custom values for this view port in the style
    viewport->setObjectName("SwitcherViewport");

    mainLayout->addItem(viewport);
    pannedLayout = new MLayout(pannedWidget);
    pannedLayout->setContentsMargins(0, 0, 0, 0);

    layoutAnimation = new TransformLayoutAnimation(pannedLayout);
    layoutAnimation->setObjectName("SwitcherLayoutAnimation");
    connect(layoutAnimation, SIGNAL(willFinish()), this, SLOT(applyPinchGestureTargetMode()));
    connect(layoutAnimation, SIGNAL(finished()), this, SLOT(runOverviewBounceAnimation()));

    bounceAnimation = new QPropertyAnimation(pannedWidget, "scale", this);
    bounceAnimation->setStartValue(1.0f);
    bounceAnimation->setEndValue(1.0f);

    overviewPolicy = new MGridLayoutPolicy(pannedLayout);
    overviewPolicy->setObjectName("OverviewPolicy");

    detailPolicy = new MLinearLayoutPolicy(pannedLayout, Qt::Horizontal);
    detailPolicy->setObjectName("DetailviewPolicy");

    viewport->setAutoRange(false);
    viewport->setWidget(pannedWidget);

    viewport->positionIndicator()->setObjectName("SwitcherOverviewPageIndicator");

    focusedSwitcherButton = 0;
    pinchedButtonPosition = -1;
}

SwitcherView::~SwitcherView()
{
    removeButtonsFromLayout();

    if(overviewStyle) {
        MTheme::releaseStyle(overviewStyle);
    }
}

bool SwitcherView::event(QEvent *e)
{
    // This stuff is necessary to receive touch events.
    if (e->type() == QEvent::TouchBegin) {
        e->setAccepted(true);
        return true;
    }
    return MWidgetView::event(e);
}

void SwitcherView::panningStopped()
{
    if (model()->switcherMode() == SwitcherModel::Detailview) {
        for (int i = 0; i < model()->buttons().count(); i++) {
            model()->buttons().at(i)->model()->setViewMode(i == focusedSwitcherButton ? SwitcherButtonModel::Large : SwitcherButtonModel::Medium);
        }
    }
}

void SwitcherView::setupModel()
{
    MWidgetView::setupModel();
    applySwitcherMode();
}

void SwitcherView::applySwitcherMode()
{
    if (model()->switcherMode() == SwitcherModel::Detailview) {
        pannedLayout->setPolicy(detailPolicy);
        controller->setObjectName("DetailviewSwitcher");
    } else {
        pannedLayout->setPolicy(overviewPolicy);
        controller->setObjectName("OverviewSwitcher");
    }

    updateButtonModesAndPageCount();
}

void SwitcherView::updateData(const QList<const char*>& modifications)
{
    MWidgetView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == SwitcherModel::Buttons) {
            updateButtons();
        } else if (member == SwitcherModel::SwitcherMode) {
            applySwitcherMode();
        } else if (member == SwitcherModel::TopmostWindow) {
            repositionSwitcher();
        }
    }
}

int SwitcherView::buttonsPerPage() const
{
    return style()->columnsPerPage() * style()->rowsPerPage();
}

void SwitcherView::repositionSwitcher()
{
    Window topmost = model()->topmostWindow();
    for (int i = 0; i < model()->buttons().count(); ++i) {
        if (topmost == model()->buttons().at(i)->xWindow()) {
            uint buttonPos = i;
            int targetPage = 0;
            if (model()->switcherMode() == SwitcherModel::Overview) {
                targetPage = buttonPos / buttonsPerPage();
            } else {
                targetPage = buttonPos;
            }
            viewport->panToPage(targetPage);
            break;
        }
    }
}

void SwitcherView::updateButtons()
{
    if(overviewStyle) {
        MTheme::releaseStyle(overviewStyle);
    }

    // Cache the overview style because addButtonInOverviewPolicy needs information from this style even though we might be in detailview mode
    overviewStyle = static_cast<const SwitcherStyle*>(MTheme::style("SwitcherStyle", "OverviewSwitcher", "", "", MainWindow::instance()->orientation()));

    focusedSwitcherButton = std::min(focusedSwitcherButton, model()->buttons().size() - 1);
    focusedSwitcherButton = std::max(focusedSwitcherButton, 0);

    removeButtonsFromLayout();

    /* Recreate the GridLayoutPolicy to keep the pannedWidget's size correct.
       TODO: Can hopefully be removed, pending on bug 165683 */
    MGridLayoutPolicy* tmp;

    tmp = overviewPolicy;

    overviewPolicy = new MGridLayoutPolicy(pannedLayout);
    overviewPolicy->setSpacing(0);
    overviewPolicy->setObjectName("OverviewPolicy");

    // Add widgets from the model to the layout
    foreach (QSharedPointer<SwitcherButton> button, model()->buttons()) {
        detailPolicy->addItem(button.data());
        addButtonInOverviewPolicy(button);
    }

    updateButtonModesAndPageCount();

    delete tmp;
}

void SwitcherView::updateButtonModesAndPageCount()
{
    int pages = 0;
    qreal range = 0;

    int buttonCount = model()->buttons().count();
    if (buttonCount > 0) {
        if (model()->switcherMode() == SwitcherModel::Detailview) {
            for (int i = 0; i < model()->buttons().count(); i++) {
                QSharedPointer<SwitcherButton> button = model()->buttons().at(i);
                button->setObjectName("DetailviewButton");
                button->model()->setViewMode(i == focusedSwitcherButton ? SwitcherButtonModel::Large : SwitcherButtonModel::Medium);
            }

            qreal buttonWidth = model()->buttons().first()->preferredSize().width();
            pages = buttonCount;
            range = (buttonCount - 1) * (buttonWidth + (geometry().width() - buttonWidth) / 4) + geometry().width();
        } else {
            SwitcherButtonModel::ViewModeType mode = buttonCount < 3 ? SwitcherButtonModel::Large : SwitcherButtonModel::Medium;
            foreach (QSharedPointer<SwitcherButton> button, model()->buttons()) {
                button->setObjectName("OverviewButton");
                button->model()->setViewMode(mode);
            }

            pages = ceilf((qreal)buttonCount / buttonsPerPage());
            range = pages * geometry().width();
        }
    }

    // First update the page count
    viewport->updatePageCount(pages);

    // Then set the range - this starts the integration and pans
    // the view to the correct page in case we were on the last
    // page and closed the last button
    viewport->setRange(QRectF(0, 0, range, 0));

    updateContentsMarginsAndSpacings();
}

void SwitcherView::updateFocusedButton(int currentPage)
{
    if (model()->switcherMode() == SwitcherModel::Detailview) {
        // Just a sanity check that we don't requst a non-existent element
        if (currentPage >= 0 && currentPage < model()->buttons().count()) {
            focusedSwitcherButton = currentPage;
        }
    } else {
        // Focus on 1st button of the snapped page
        int pos = currentPage * buttonsPerPage();
        // Just a sanity check that we don't request a non-existent element
        if (pos >= 0 && pos < model()->buttons().count()) {
            focusedSwitcherButton = pos;
        }
    }
}

void SwitcherView::updateContentsMarginsAndSpacings()
{
    if (style()->columnsPerPage() > 0 && style()->rowsPerPage() > 0 && model()->buttons().count() > 0) {
        updateDetailViewContentsMarginsAndSpacings();
        updateOverviewContentsMarginsAndSpacings();
    } else {
        detailPolicy->setContentsMargins(0, 0, 0, 0);
        overviewPolicy->setContentsMargins(0, 0, 0, 0);
    }
}

void SwitcherView::updateDetailViewContentsMarginsAndSpacings()
{
    SwitcherButton *button = model()->buttons().first().data();

    // The leftmost and the rightmost items need extra space on their sides to get them centered: the rest need spacing between them
    qreal horizontalMargin = (geometry().width() - button->preferredSize().width()) / 2;
    qreal verticalMargin = (geometry().height() - button->preferredSize().height()) / 2;
    detailPolicy->setContentsMargins(horizontalMargin, verticalMargin, horizontalMargin, verticalMargin);
    detailPolicy->setHorizontalSpacing(horizontalMargin / 2);
}

void SwitcherView::updateOverviewContentsMarginsAndSpacings()
{
    SwitcherButton *button = model()->buttons().first().data();
    int buttonCount = model()->buttons().count();
    int colsPerPage = style()->columnsPerPage();
    int rows = style()->rowsPerPage();

    // Calculate margins for the overview
    qreal topMargin, bottomMargin, leftMargin, rightMargin;
    overviewPolicy->getContentsMargins(&leftMargin, &topMargin, &rightMargin, &bottomMargin);

    int columns = qMin(overviewPolicy->columnCount(), style()->columnsPerPage());
    qreal effectiveButtonsWidth = (columns * button->preferredSize().width()) + ((columns - 1) * style()->buttonHorizontalSpacing());

    leftMargin = (geometry().width() - effectiveButtonsWidth) / 2;
    rightMargin = leftMargin;

    /*
       If the last page is missing buttons i.e. it is not full, we add extra margins to it
       so that the paging works
     */
    if (buttonCount > buttonsPerPage()) {
        int lastPageButtonCount = buttonCount % buttonsPerPage();
        if (lastPageButtonCount > 0 && lastPageButtonCount < style()->columnsPerPage()) {
            // Compensate the missing buttons
            int missingButtonCount = style()->columnsPerPage() - lastPageButtonCount;
            rightMargin = leftMargin + button->preferredSize().width() * missingButtonCount;
            // Also add the spacings
            rightMargin += missingButtonCount * style()->buttonHorizontalSpacing();
        }
    }

    if (buttonCount < 3) {
        // One or two buttons fit on a single row so just center them vertically
        topMargin = (geometry().height() - button->preferredSize().height()) / 2;
        bottomMargin = topMargin;
    }

    overviewPolicy->setContentsMargins(leftMargin, topMargin, rightMargin, bottomMargin);

    // Add horizontal spacing for all columns except the last one
    for (int column = 0; column < overviewPolicy->columnCount(); column++) {
        overviewPolicy->setColumnSpacing(column, (column % colsPerPage) != (colsPerPage - 1) ? style()->buttonHorizontalSpacing() : (leftMargin * 2));
    }

    // Add vertical spacing for all rows except the last one
    for (int row = 0; row < rows - 1; row++) {
        overviewPolicy->setRowSpacing(row, style()->buttonVerticalSpacing());
    }
}

void SwitcherView::addButtonInOverviewPolicy(QSharedPointer<SwitcherButton> button)
{
    int colsPerPage = overviewStyle->columnsPerPage();
    int rows = overviewStyle->rowsPerPage();

    if (rows > 0 && colsPerPage > 0) {
        int location = model()->buttons().indexOf(button);
        int page = location / (colsPerPage * rows);
        int row = (location / colsPerPage) % rows;
        int column = (location % colsPerPage) + (page * colsPerPage);

        overviewPolicy->addItem(button.data(), row, column);
    }
}

void SwitcherView::removeButtonsFromLayout()
{
    // Remove all buttons from the layout and set parents to null (do not destroy them)
    for (int i = 0, count = pannedLayout->count(); i < count; i++) {
        static_cast<SwitcherButton *>(pannedLayout->takeAt(0))->setParentItem(0);
    }
}

int SwitcherView::buttonIndex(const SwitcherButton* button) const
{
    QList<QSharedPointer<SwitcherButton> > buttons = model()->buttons();
    for (int i = 0; i < buttons.count(); ++i) {
        if (buttons.at(i) == button) {
            return i;
        }
    }
    return -1;
}

SwitcherButton *SwitcherView::buttonAt(QPointF centerPoint) const
{
    QList<QGraphicsItem*> items = MainWindow::instance()->items(centerPoint.x(), centerPoint.y());
    foreach(QGraphicsItem* item, items) {
        SwitcherButton *button = dynamic_cast<SwitcherButton*>(item);
        if(button) {
            return button;
        }
    }
    return NULL;
}

void SwitcherView::calculateNearestButtonAt(QPointF centerPoint)
{
    const SwitcherButton *button = 0;
    if((button = buttonAt(centerPoint))) {
        pinchedButtonPosition = buttonIndex(button);
        return;
    }

    qreal nearestDistance = 0.0;
    QList<QSharedPointer<SwitcherButton> > buttons = model()->buttons();
    uint buttonsInPage = buttonsPerPage();
    uint fullPages = buttons.count() / buttonsInPage;
    uint extraButtons = buttons.count() - (fullPages * buttonsInPage);
    uint currentPage = viewport->currentPage();

    // If the current page is not a full page then go through how many ever buttons there are on the page
    uint loopCounter = currentPage + 1 > fullPages ? extraButtons : buttonsInPage;

    // Loop from first button in page to number of buttons in page
    for(uint buttonPosition = buttonsInPage*currentPage;buttonPosition < buttonsInPage*currentPage + loopCounter;buttonPosition++) {
        button = buttons.at(buttonPosition).data();

        // Translate the button pos(scene coordinates) to viewport coordinates
        QPointF buttonPosViewport = button->mapToItem(viewport, button->pos());

        // Scene coordinates for buttons on pages greater than 1 are more than viewport coordinate diemensions(device width/height)
        //Hence points are translated to first page where centerPoint is supposedly delivered
        QPointF buttonPosTranslatedToFirstPage(buttonPosViewport);

        // In portrait mode centerpoint is still delivered as if origin is device topLeft when kept in landscape.
        // Scene cooridnates are however measured as if origin is at device bottomLeft when kept in landscape.
        QPointF centerPointTranslated(centerPoint);
        if(MainWindow::instance()->orientation() == M::Landscape) {
                buttonPosTranslatedToFirstPage.setX(buttonPosTranslatedToFirstPage.x() - currentPage*viewport->size().width());
            } else {
                buttonPosTranslatedToFirstPage.setX(buttonPosTranslatedToFirstPage.x() - currentPage*viewport->size().width());
                centerPointTranslated.setX(viewport->size().width() - centerPoint.y());
                centerPointTranslated.setY(centerPoint.x());
        }

        // Calculate the distance between button center point and center point of pinch
        QLineF line(centerPointTranslated, buttonPosTranslatedToFirstPage);
        qreal distance = line.length();

        // if calculated distance is less than previous least distance then this button is nearer
        if(nearestDistance == 0.0 || distance < nearestDistance) {
            nearestDistance = distance;
            pinchedButtonPosition = buttonPosition;
        }
    }
}

void SwitcherView::pinchGestureEvent(QGestureEvent *event, QPinchGesture* gesture)
{
    /*! Due to bug 174335, each gesture event is delivered twice with two different
        QPinchGesture instances. Only one is needed, and the other one would break calculating the pinch
        speed, so have only one gesture active at a time */
    static QPinchGesture *activeGesture = 0;

    if(gesture->state() != Qt::GestureStarted && gesture != activeGesture) {
        return;
    }

    /*! Finish any currently running animation before starting a new one */
    if((layoutAnimation->isAnimating() && !layoutAnimation->manualControl()) || bounceAnimation->state() == QAbstractAnimation::Running) {
        return;
    }

    switch(gesture->state()) {
    case Qt::GestureStarted:
        {
            activeGesture = gesture;
            viewport->setEnabled(false);
            calculateNearestButtonAt(gesture->centerPoint());

            foreach (const QSharedPointer<SwitcherButton> &button, model()->buttons()) {
                button->installSceneEventFilter(controller);
            }
            break;
        }
    case Qt::GestureUpdated:
        {
            if(!layoutAnimation->isAnimating()) {
                pinchGestureTargetMode = gesture->scaleFactor() >=1 ? SwitcherModel::Detailview : SwitcherModel::Overview;

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
                      (gesture->scaleFactor() - 1.0f) :
                      (1.0f - gesture->scaleFactor());

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

            break;
        }
    case Qt::GestureFinished:
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
        viewport->setEnabled(true);
        activeGesture = 0;

        foreach (const QSharedPointer<SwitcherButton> &button, model()->buttons()) {
            button->setDown(false);
        }
        break;

    default:
        break;
    }
    event->accept(gesture);
}

bool SwitcherView::sceneEventFilter(QGraphicsItem *watched, QEvent *event)
{
    bool filtered = false;

    if (event->type() == QEvent::GraphicsSceneMouseMove) {
        foreach (const QSharedPointer<SwitcherButton> &button, model()->buttons()) {
            if (button == watched) {
                filtered = true;
                break;
            }
        }
    }

    return filtered;
}

void SwitcherView::applyPinchGestureTargetMode()
{
    model()->setSwitcherMode(pinchGestureTargetMode);
    viewport->setPage(pinchGestureTargetMode == SwitcherModel::Detailview ?
                      pinchedButtonPosition :
                      pinchedButtonPosition / buttonsPerPage());

    if(!layoutAnimation->manualControl()) {
        layoutAnimation->stop();
    }
}

void SwitcherView::setInwardBounceAnimation(bool i)
{
    // set the middle key value to either less than 1 when bouncing or zooming in overview mode,
    // or over 1 when zooming in detail mode
    bounceAnimation->setKeyValueAt(0.5f, 1.0f + (i ? -1.0f : 1.0f) * style()->bounceScale());
}

void SwitcherView::startBounceAnimation()
{
    bounceAnimation->setDuration(style()->bounceDuration());
    bounceAnimation->setEasingCurve(style()->bounceCurve());

    pannedWidget->setTransformOriginPoint(viewport->rect().center() +
                                          QPointF(viewport->pageWidth() * viewport->currentPage(), 0));
    bounceAnimation->start();
}

void SwitcherView::runOverviewBounceAnimation()
{
    if(pinchGestureTargetMode == SwitcherModel::Overview) {
        setInwardBounceAnimation(true);
        startBounceAnimation();
    }
}

M_REGISTER_VIEW_NEW(SwitcherView, Switcher)
