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
#include <math.h>
#include <algorithm>
#include "pagedviewport.h"

static qreal calculateCenterCorrection(qreal value, qreal scaleFactor);
static const qreal HALF_PI = M_PI / 2.0;
static const qreal MAX_Z_VALUE = 1.0;

SwitcherView::SwitcherView(Switcher *switcher) :
    MWidgetView(switcher), controller(switcher), mainLayout(new QGraphicsLinearLayout(Qt::Vertical)), pannedWidget(new MWidget), viewport(new PagedViewport)
{
    mainLayout->setContentsMargins(0, 0, 0, 0);
    switcher->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    switcher->setLayout(mainLayout);

    connect(viewport, SIGNAL(pageChanged(int)), this, SLOT(updateFocusedButton(int)));

    // We have custom values for this view port in the style
    viewport->setObjectName("SwitcherDetailsViewport");

    mainLayout->addItem(viewport);

    pannedLayout = new MLayout(pannedWidget);
    pannedLayout->setContentsMargins(0, 0, 0, 0);

    overviewPolicy = new MGridLayoutPolicy(pannedLayout);
    overviewPolicy->setSpacing(0);
    overviewPolicy->setObjectName("OverviewPolicy");

    detailPolicy = new MLinearLayoutPolicy(pannedLayout, Qt::Horizontal);
    detailPolicy->setSpacing(0);
    detailPolicy->setObjectName("DetailviewPolicy");

    viewport->setWidget(pannedWidget);

    focusedSwitcherButton = 0;
    firstButtonPriority = WindowInfo::Normal;
}

SwitcherView::~SwitcherView()
{
}

void SwitcherView::panningStopped()
{
    if (model()->switcherMode() == SwitcherModel::Detailview) {
        if (model()->buttons().empty()) {
            return;
        }
        model()->buttons().at(focusedSwitcherButton)->model()->setViewMode(SwitcherButtonModel::Large);
        for (int i = 0; i < model()->buttons().count(); i++) {
            if (i != focusedSwitcherButton) {
                model()->buttons().at(i)->model()->setViewMode(SwitcherButtonModel::Medium);
            }
        }
    }
}

void SwitcherView::animateDetailView(const QPointF &pannedPos)
{
    qreal viewportWidthHalf = geometry().width() / 2;
    for (int i = 0; i < pannedLayout->count(); i++) {
        SwitcherButton* widget = dynamic_cast<SwitcherButton*> (pannedLayout->itemAt(i));
        widget->model()->setViewMode(SwitcherButtonModel::Medium);
        QRectF widgetGeometry = widget->geometry();

        // Pre calculate some variables for speed and readability
        qreal center = pannedPos.x() + viewportWidthHalf;
        qreal widgetCenter = widgetGeometry.left() + (widgetGeometry.width() / 2);
        qreal distanceFromCenter = widgetCenter - center;
        qreal normalizedDistFromCenter = distanceFromCenter / viewportWidthHalf;
        // sined + the curve "wave length" fitted to the width
        qreal transformationPath = sin(normalizedDistFromCenter * M_PI);

        /*
         The scale factor is 1 when the widget's center is at the center of the screen
         and style()->scaleFactor() when widgets center is at the edge of the screen.
         The sin curve will be phase shifted (== times HALF_PI) and 'amplitude capped'
         so that we scale factor will behave as describe above.
         */
        qreal amplitudeLimitation = 1 - style()->scaleFactor();
        qreal scaleFactor = 1 - amplitudeLimitation * sin((abs(distanceFromCenter) / (viewportWidthHalf)) * HALF_PI);

        /*
         Calculate center correction factors as the scaling will expand the button
         to right and south.
         */
        qreal xCentered = calculateCenterCorrection(widgetGeometry.width(), scaleFactor);
        qreal yCentered = calculateCenterCorrection(widgetGeometry.height(), scaleFactor);
        /*
         As the unemphasized items are scaled downwards they need to be horizontally
         shifted closer to the emphasized item, otherwise a big gap will appear in
         between the items
         */
        qreal overLapCurve = sin(normalizedDistFromCenter * HALF_PI) * style()->itemOverLap();
        qreal scaleInducedShift = widgetGeometry.width() * (1 - scaleFactor);
        qreal shiftFactor = overLapCurve + scaleInducedShift;
        if (distanceFromCenter < 0) {
            xCentered -= shiftFactor;
        } else {
            xCentered += shiftFactor;
        }

        // The horizontal movement of the focused item is "fast-forwarded"
        if (focusedSwitcherButton == i) {
            xCentered -= style()->fastForward() * transformationPath;
        }

        QTransform positionAndScale;
        positionAndScale.translate(-xCentered, -yCentered);
        positionAndScale.scale(scaleFactor, scaleFactor);
        widget->setTransform(positionAndScale);

        /*
         As the distance of the widget from the center of the viewport is
         normalized to 0...1 (0 meaning that the widget is in the center)
         we make sure that the the widget in the center has the highest Z order
         */
        widget->setZValue(MAX_Z_VALUE - abs(distanceFromCenter));

        // Rotate only if the item rotation is non-zero 
        if (style()->itemRotation() != 0) {
            int angle = 0.0;
            // The angle is only valid when we are with-in the viewport.
            if (abs(distanceFromCenter) / viewportWidthHalf < 1.0) {
                angle = style()->itemRotation() * transformationPath;
            }
            QTransform rotation;
            rotation.translate(0, widgetGeometry.height() / 2);
            rotation.rotate(angle, Qt::YAxis);
            rotation.translate(0, -widgetGeometry.height() / 2);
            widget->setTransform(rotation, true);
        }

    }
}

void SwitcherView::setGeometry(const QRectF &rect)
{
    MWidgetView::setGeometry(rect);
    updateSnapInterval();
}

void SwitcherView::setupModel()
{
    MWidgetView::setupModel();
    applySwitcherMode();
}

void SwitcherView::applySwitcherMode()
{
    if (model()->switcherMode() == SwitcherModel::Detailview) {
        disconnect(MainWindow::instance()->sceneManager(), 0, this, 0);
        pannedLayout->setPolicy(detailPolicy);
        connect(viewport, SIGNAL(positionChanged(QPointF)), this, SLOT(animateDetailView(QPointF)));
        connect(viewport, SIGNAL(panningStopped()), this, SLOT(panningStopped()));
        controller->setObjectName("DetailviewSwitcher");
    } else {
        disconnect(viewport, 0, this, 0);
        connect(MainWindow::instance()->sceneManager(), SIGNAL(orientationChangeFinished(M::Orientation)), this, SLOT(updateButtons()));

        pannedLayout->setPolicy(overviewPolicy);
        controller->setObjectName("OverviewSwitcher");
    }

    updateButtonModes();
}

void SwitcherView::updateData(const QList<const char*>& modifications)
{
    MWidgetView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == SwitcherModel::Buttons) {
            if (model()->buttons().isEmpty()) {
                // Reset the priority if the model is empty
                firstButtonPriority = WindowInfo::Normal;
            } else {
                SwitcherButton *firstButton = model()->buttons().first().data();

                // If the first button's priority has risen pan the view to show it
                if (firstButton->windowPriority() < firstButtonPriority) {
                    viewport->panToPage(0);
                }

                firstButtonPriority = firstButton->windowPriority();
            }

            updateButtons();
        }

        if (member == SwitcherModel::SwitcherMode) {
            applySwitcherMode();
        }
    }
}

void SwitcherView::updateButtons()
{
    focusedSwitcherButton = std::min(focusedSwitcherButton, model()->buttons().size() - 1);
    focusedSwitcherButton = std::max(focusedSwitcherButton, 0);
    // Remove all widgets from the layout (do not destroy them)
    while (pannedLayout->count() > 0) {
        pannedLayout->removeAt(0);
    }
    // Add widgets from the model to the layout
    foreach (QSharedPointer<SwitcherButton> button, model()->buttons()) {
        detailPolicy->addItem(button.data());
        addButtonInOverviewPolicy(button);
    }

    updateButtonModes();
}

void SwitcherView::updateButtonModes()
{
    int buttonCount = model()->buttons().count();
    if (model()->switcherMode() == SwitcherModel::Detailview) {
        foreach (QSharedPointer<SwitcherButton> button, model()->buttons()) {
            button.data()->setObjectName("DetailviewButton");
            button.data()->model()->setViewMode(SwitcherButtonModel::Medium);
        }
    } else {
        foreach (QSharedPointer<SwitcherButton> button, model()->buttons()) {
            button.data()->setObjectName("OverviewButton");
            if (MainWindow::instance()->sceneManager()->orientation() == M::Portrait) {
                if (buttonCount < 3) {
                    button.data()->model()->setViewMode(SwitcherButtonModel::Large);
                } else if (buttonCount < 5) {
                    button.data()->model()->setViewMode(SwitcherButtonModel::Medium);
                } else {
                    button.data()->model()->setViewMode(SwitcherButtonModel::Small);
                }
            } else {
                if (buttonCount < 3) {
                    button.data()->model()->setViewMode(SwitcherButtonModel::Large);
                } else {
                    button.data()->model()->setViewMode(SwitcherButtonModel::Medium);
                }
            }
        }
    }

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
        int pos = currentPage * (style()->columnsPerPage() * style()->rowsPerPage());
        // Just a sanity check that we don't requst a non-existent element
        if (pos >= 0 && pos < model()->buttons().count()) {
            focusedSwitcherButton = pos;
        }
    }
}

void SwitcherView::updateSnapInterval()
{
    if (!model()->buttons().empty()) {
        // For simplicity we assume that 
        // 1. every button is of constant size and
        // 2. preferred == minimum size == maximum size
        SwitcherButton* button = model()->buttons().first().data();
        if (model()->switcherMode() == SwitcherModel::Detailview) {
            /*
             Practically we enable margins to the panned layout in order to
             align the centers of the switcher button and the mpannableviewport.
             */
	    viewport->updatePageWidth(button->preferredSize().width());
        } else {
	    viewport->updatePageWidth(geometry().width());
        }
    } else {
	viewport->updatePageWidth(0);
    }
}

void SwitcherView::updateContentsMarginsAndSpacings()
{
    int buttonCount = model()->buttons().count();
    int colsPerPage = style()->columnsPerPage();
    int rows = style()->rowsPerPage();

    if (buttonCount == 0 || colsPerPage == 0 || rows == 0) {
        detailPolicy->setContentsMargins(0, 0, 0, 0);
        overviewPolicy->setContentsMargins(0, 0, 0, 0);
        return;
    }

    SwitcherButton* button = model()->buttons().first().data();
    qreal buttonWidth = button->preferredSize().width();
    qreal buttonHeight = button->preferredSize().height();

    qreal detailMargin = (geometry().width() - buttonWidth) / 2;
    qreal verticalContentMargin = (geometry().height() - buttonHeight) / 2;
    detailPolicy->setContentsMargins(detailMargin, verticalContentMargin,
                                     detailMargin, verticalContentMargin);


    int columns;
    if (overviewPolicy->columnCount() < style()->columnsPerPage()) {
        columns = overviewPolicy->columnCount();
    } else {
        columns = style()->columnsPerPage();
    }
    qreal effectiveButtonsWidth = (columns * buttonWidth) + ((columns - 1) * style()->buttonHorizontalSpacing());
    qreal effectiveButtonsHeight = overviewPolicy->rowCount() * buttonHeight + qMax(0, overviewPolicy->rowCount() - 1) * style()->buttonVerticalSpacing();
    qreal leftContentMargin = (geometry().width() - effectiveButtonsWidth) / 2;
    qreal rightContentMargin = leftContentMargin;
    verticalContentMargin = (geometry().height() - effectiveButtonsHeight) / 2;

    /*
       If the last page is missing buttons i.e. it is not full, we add extra margins to it
       so that the pageing works
     */
    if (buttonCount > (style()->columnsPerPage() * style()->rowsPerPage())) {
        int lastPageButtonCount = buttonCount % (style()->columnsPerPage() * style()->rowsPerPage());
        if (lastPageButtonCount > 0 && lastPageButtonCount < style()->columnsPerPage()) {
            // Compensate the missing buttons
            int missingButtonCount = style()->columnsPerPage() - lastPageButtonCount;
            rightContentMargin = leftContentMargin + buttonWidth * missingButtonCount;
            // Also add the spacings
            rightContentMargin += missingButtonCount * style()->buttonHorizontalSpacing();
        }
    }

    overviewPolicy->setContentsMargins(leftContentMargin, verticalContentMargin,
                                       rightContentMargin, verticalContentMargin);

    for(int column = 0; column < overviewPolicy->columnCount(); column++) {
        if ((column % colsPerPage) == colsPerPage - 1) {
            overviewPolicy->setColumnSpacing(column, leftContentMargin * 2);
        } else {
            overviewPolicy->setColumnSpacing(column, style()->buttonHorizontalSpacing());
        }
    }

    for(int row = 0; row < rows; row++) {
        // add vertical spacing for all but last row
        if(row < rows - 1){
            overviewPolicy->setRowSpacing(row, style()->buttonVerticalSpacing());
        }
    }
}

void SwitcherView::addButtonInOverviewPolicy(QSharedPointer<SwitcherButton> button)
{
    int colsPerPage = style()->columnsPerPage();
    int rows = style()->rowsPerPage();
    // just a precautionary measure so that we do not divide with zero later on
    if (rows == 0 || colsPerPage == 0) {
        return;
    }
    int location = model()->buttons().indexOf(button);
    int page = location / (colsPerPage * rows);
    int row = (location / colsPerPage) % rows;
    int column = (location % colsPerPage) + (page * colsPerPage);

    overviewPolicy->addItem(button.data(), row, column);
}

static qreal calculateCenterCorrection(qreal value, qreal scaleFactor)
{
    return (value * (scaleFactor - 1)) / 2.0;
}


M_REGISTER_VIEW_NEW(SwitcherView, Switcher)
