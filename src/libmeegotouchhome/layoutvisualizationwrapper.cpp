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
        wrappingMode(NoWrap),
        previousParentLayoutWidth(0.0)
{
}

LayoutVisualizationWrapper::~LayoutVisualizationWrapper()
{
}

void LayoutVisualizationWrapper::setWrappingMode(LayoutVisualizationWrapper::WrappingMode mode)
{
    if (mode != wrappingMode || previousParentLayoutWidth != parentLayoutWidth()) {
        wrappingMode = mode;

        if (layout.orientation() == Qt::Horizontal) {
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
    }
}

void LayoutVisualizationWrapper::resetTransformation(int layoutIndex) const
{
    QGraphicsItem* layoutItem = layout.itemAt(layoutIndex)->graphicsItem();
    if (layoutItem != NULL) {
        layoutItem->setTransform(QTransform::fromTranslate(0.0, 0.0));
    }
}

qreal LayoutVisualizationWrapper::parentLayoutWidth() const
{
    qreal width = 0.0;
    QGraphicsWidget *parentWidget = dynamic_cast<QGraphicsWidget*>(layout.parentLayoutItem());
    if (parentWidget != NULL) {
        width = parentWidget->size().width();
    }
    return width;
}

void LayoutVisualizationWrapper::setTransformation(int layoutIndex, Direction direction)
{
    qreal xTransform = parentLayoutWidth();
    if (xTransform > 0.0)
    {
        previousParentLayoutWidth = xTransform;
        if (direction == Left) {
            xTransform = -xTransform;
        }
        QGraphicsItem* layoutItem = layout.itemAt(layoutIndex)->graphicsItem();
        if (layoutItem != NULL) {
            layoutItem->setTransform(QTransform::fromTranslate(xTransform, 0.0));
        }
    }
}
