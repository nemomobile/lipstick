/*
 * This file is part of mhome
 *
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * Contact: Tomas Junnonen <tomas.junnonen@nokia.com>
 *
 * This software, including documentation, is protected by copyright
 * controlled by Nokia Corporation. All rights are reserved. Copying,
 * including reproducing, storing, adapting or translating, any or all of
 * this material requires the prior written consent of Nokia Corporation.
 * This material also contains confidential information which may not be
 * disclosed to others without the prior written consent of Nokia.
 */

#include "layoutvisualizationwrapper.h"
#include <QGraphicsLinearLayout>
#include <QGraphicsWidget>

LayoutVisualizationWrapper::LayoutVisualizationWrapper(QGraphicsLinearLayout &layout) :
        layout(layout),
        wrappingMode(NoWrap)
{
}

LayoutVisualizationWrapper::~LayoutVisualizationWrapper()
{
}

void LayoutVisualizationWrapper::setWrappingMode(LayoutVisualizationWrapper::WrappingMode mode)
{
    if (mode == wrappingMode) {
        return;
    }

    wrappingMode = mode;

    if (layout.orientation() != Qt::Horizontal) {
        return;
    }

    switch (mode) {
    case NoWrap:
        resetTransformation(0);
        resetTransformation(layout.count() - 1);
        break;

    case WrapRightEdgeToLeft:
        resetTransformation(0);
        setTransformation(layout.count() - 1, Left);
        break;

    case WrapLeftEdgeToRight:
        resetTransformation(layout.count() - 1);
        setTransformation(0, Right);
        break;
    }
}

void LayoutVisualizationWrapper::resetTransformation(int layoutIndex) const
{
    QGraphicsItem* layoutItem = layout.itemAt(layoutIndex)->graphicsItem();
    if (layoutItem != NULL) {
        layoutItem->setTransform(QTransform::fromTranslate(0.0, 0.0));
    }
}

void LayoutVisualizationWrapper::setTransformation(int layoutIndex, Direction direction) const
{
    QGraphicsItem* layoutItem = layout.itemAt(layoutIndex)->graphicsItem();
    if (layoutItem != NULL) {
        QGraphicsWidget *parentWidget = dynamic_cast<QGraphicsWidget*>(layout.parentLayoutItem());
        if (parentWidget != NULL) {
            qreal xTransform = parentWidget->size().width();
            if (direction == Left) {
                xTransform = -xTransform;
            }

            layoutItem->setTransform(QTransform::fromTranslate(xTransform, 0.0));
        }
    }
}
