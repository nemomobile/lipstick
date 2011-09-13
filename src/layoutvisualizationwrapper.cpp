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
