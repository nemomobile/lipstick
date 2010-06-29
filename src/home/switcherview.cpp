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
#include <MPositionIndicator>
#include "pagepositionindicatorview.h"
#include <math.h>
#include <algorithm>
#include "pagedviewport.h"

static const qreal HALF_PI = M_PI / 2.0;
static const qreal MAX_Z_VALUE = 1.0;

SwitcherView::SwitcherView(Switcher *switcher) :
    MWidgetView(switcher), controller(switcher), mainLayout(new QGraphicsLinearLayout(Qt::Vertical)), pannedWidget(new MWidget), viewport(new PagedViewport)
{
    mainLayout->setContentsMargins(0, 0, 0, 0);
    switcher->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    switcher->setLayout(mainLayout);

    connect(viewport, SIGNAL(pageChanged(int)), this, SLOT(updateFocusedButton(int)));
    connect(MainWindow::instance()->sceneManager(), SIGNAL(orientationChanged(M::Orientation)), this, SLOT(updateButtons()));

    // We have custom values for this view port in the style
    viewport->setObjectName("SwitcherViewport");

    mainLayout->addItem(viewport);
    pannedLayout = new MLayout(pannedWidget);
    pannedLayout->setContentsMargins(0, 0, 0, 0);

    overviewPolicy = new MGridLayoutPolicy(pannedLayout);
    overviewPolicy->setObjectName("OverviewPolicy");

    detailPolicy = new MLinearLayoutPolicy(pannedLayout, Qt::Horizontal);
    detailPolicy->setObjectName("DetailviewPolicy");

    viewport->setAutoRange(false);
    viewport->setWidget(pannedWidget);

    viewport->positionIndicator()->setObjectName("SwitcherOverviewPageIndicator");

    focusedSwitcherButton = 0;
}

SwitcherView::~SwitcherView()
{
    removeButtonsFromLayout();
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
        connect(viewport, SIGNAL(panningStopped()), this, SLOT(panningStopped()));

        pannedLayout->setPolicy(detailPolicy);
        controller->setObjectName("DetailviewSwitcher");
    } else {
        disconnect(viewport, 0, this, 0);

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
        }
    }
}

void SwitcherView::updateButtons()
{
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
            foreach (QSharedPointer<SwitcherButton> button, model()->buttons()) {
                button.data()->setObjectName("DetailviewButton");
                button.data()->model()->setViewMode(SwitcherButtonModel::Medium);
            }

            qreal buttonWidth = model()->buttons().first()->preferredSize().width();
            pages = buttonCount;
            range = (buttonCount - 1) * (buttonWidth + (geometry().width() - buttonWidth) / 4) + geometry().width();
        } else {
            SwitcherButtonModel::ViewModeType mode = buttonCount < 3 ? SwitcherButtonModel::Large : SwitcherButtonModel::Medium;
            foreach (QSharedPointer<SwitcherButton> button, model()->buttons()) {
                button.data()->setObjectName("OverviewButton");
                button.data()->model()->setViewMode(mode);
            }

            pages = ceilf((qreal)buttonCount / (style()->columnsPerPage() * style()->rowsPerPage()));
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
        int pos = currentPage * (style()->columnsPerPage() * style()->rowsPerPage());
        // Just a sanity check that we don't requst a non-existent element
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
    if (buttonCount > (style()->columnsPerPage() * style()->rowsPerPage())) {
        int lastPageButtonCount = buttonCount % (style()->columnsPerPage() * style()->rowsPerPage());
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
    int colsPerPage = style()->columnsPerPage();
    int rows = style()->rowsPerPage();
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

M_REGISTER_VIEW_NEW(SwitcherView, Switcher)
