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
#include "switcherview.h"
#include "switcher.h"
#include "switcherbutton.h"
#include "switcherphysicsintegrationstrategy.h"

#include <DuiViewCreator>
#include <DuiLayout>
#include <DuiTheme>
#include <DuiLocale>
#include <DuiApplication>
#include <DuiWindow>
#include <DuiFlowLayoutPolicy>
#include <DuiPannableViewport>
#include <DuiLinearLayoutPolicy>
#include <QTimeLine>
#include <QGraphicsLinearLayout>
#include <math.h>
#include <algorithm>

static qreal calculateCenterCorrection(qreal value, qreal scaleFactor);
static const qreal HALF_PI = M_PI / 2.0;
static const qreal MAX_Z_VALUE = 1.0;

SwitcherView::SwitcherView(Switcher *switcher) :
    DuiWidgetView(switcher),
    controller(switcher),
    mainLayout(new QGraphicsLinearLayout(Qt::Vertical)),
    pannedWidget(new DuiWidget)
{
    mainLayout->setContentsMargins(0, 0, 0, 0);
    switcher->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    switcher->setLayout(mainLayout);
    
    DuiPannableViewport *viewport = new DuiPannableViewport;
    // We have custom values for this view port in the style
    viewport->setObjectName("SwitcherDetailsViewport");
    viewport->setPanDirection(Qt::Horizontal);

    mainLayout->addItem(viewport);

    pannedLayout = new QGraphicsLinearLayout(Qt::Horizontal, pannedWidget);    
    pannedLayout->setSpacing(0);
    connect(viewport, SIGNAL(sizePosChanged(QSizeF, QRectF, QPointF)), 
            this, SLOT(sizePosChanged(QSizeF, QRectF, QPointF)));

    connect(viewport, SIGNAL(panningStopped()), this, SLOT(panningStopped()));

    DuiWidget* verticalWidget = new DuiWidget;    
    QGraphicsLinearLayout* verticalLayoutHelper = new QGraphicsLinearLayout(Qt::Vertical, 
                                                                            verticalWidget);
    verticalLayoutHelper->setContentsMargins(0, 0, 0, 0);
    verticalLayoutHelper->setSpacing(0);
    verticalLayoutHelper->addStretch();
    verticalLayoutHelper->addItem(pannedWidget);
    verticalLayoutHelper->addStretch();
    viewport->setWidget(verticalWidget);  

    // The integrator gets deleted by the physics 
    integrator = new SwitcherPhysicsIntegrationStrategy;

    connect(integrator, SIGNAL(snapIndexChanged(int)),
            this, SLOT(snapIndexChanged(int)));

    viewport->physics()->setIntegrationStrategy(integrator);
    focusedSwitcherButton = 0;

    firstButtonPriority = WindowInfo::Normal;
}

SwitcherView::~SwitcherView()
{
}

void SwitcherView::panningStopped()
{    
    if (model()->buttons().empty()) {
        return;
    }
    model()->buttons().at(focusedSwitcherButton)->setEmphasis();
    for (int i = 0; i < model()->buttons().count(); i++) {
        if (i != focusedSwitcherButton) {
            model()->buttons().at(i)->unsetEmphasis();
        }
    }
    update();
}

void SwitcherView::sizePosChanged(const QSizeF &viewportSize, 
                                  const QRectF &pannedRange, 
                                  const QPointF &pannedPos) 
{
    Q_UNUSED(pannedRange);
    qreal viewportWidthHalf = viewportSize.width() / 2;
    for (int i = 0; i < pannedLayout->count(); i++) {
        SwitcherButton* widget = dynamic_cast<SwitcherButton*>(pannedLayout->itemAt(i));
        widget->unsetEmphasis();
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
            rotation.translate(0, widgetGeometry.height()/2);
            rotation.rotate(angle, Qt::YAxis);
            rotation.translate(0, -widgetGeometry.height()/2);
            widget->setTransform(rotation, true);        
        }
        
    }
}

void SwitcherView::setGeometry(const QRectF &rect)
{
    DuiWidgetView::setGeometry(rect);
    updatePanMarginsAndSnapInterval();
}


void SwitcherView::updateData(const QList<const char*>& modifications)
{
    DuiWidgetView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == SwitcherModel::Buttons) {
	    focusedSwitcherButton = std::min(focusedSwitcherButton, model()->buttons().size() - 1);
	    focusedSwitcherButton = std::max(focusedSwitcherButton, 0);
            // Remove all widgets from the layout (do not destroy them)
            while (pannedLayout->count() > 0) {
                pannedLayout->removeAt(0);
            }
            // Add widgets from the model to the layout
            foreach (QSharedPointer<SwitcherButton> button, model()->buttons()) {
		pannedLayout->addItem(button.data());
            }

            if (model()->buttons().isEmpty()) {
                // Reset the priority if the model is empty
                firstButtonPriority = WindowInfo::Normal;
            } else {
                SwitcherButton *firstButton = model()->buttons().first().data();

                // If the first button's priority has risen pan the view to show it
                if (firstButton->windowPriority() < firstButtonPriority) {
                    // The integrator will be started
                    // by updatePanMarginsAndSnapInterval,
                    // so no need to start it here
                    integrator->panToItem(0);
                }

                firstButtonPriority = firstButton->windowPriority();
            }

            updatePanMarginsAndSnapInterval();	    
        }
    }
}

void SwitcherView::snapIndexChanged(int newPosition)
{
    // Just a sanity check that we don't requst a non-existent element
    if (newPosition >= 0 && newPosition < model()->buttons().count()) {
        focusedSwitcherButton = newPosition;
    }
}

void SwitcherView::updatePanMarginsAndSnapInterval()
{
    if (!model()->buttons().empty()) {
        // For simplicity we assume that 
        // 1. every button is of constant size and
        // 2. preferred == minimum size == maximum size
        SwitcherButton* button = model()->buttons().first().data();
        /*
          Practically we enable margins to the panned layout in order to 
          align the centers of the switcher button and the duipannableviewport.
         */
        qreal width = button->preferredSize().width();
        qreal margin = (geometry().width() - width) / 2;

        pannedLayout->setContentsMargins(margin, 0, margin, 0);
        uint snapInterval = width + pannedLayout->spacing();
        integrator->setSnapInterval(snapInterval);
    } else {
        integrator->setSnapInterval(0);
    }
}

static qreal calculateCenterCorrection(qreal value, qreal scaleFactor)
{
  return (value * (scaleFactor - 1)) / 2.0;
}


DUI_REGISTER_VIEW_NEW(SwitcherView, Switcher)
