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

#ifndef LAYOUTVISUALIZATIONWRAPPER_STUB
#define LAYOUTVISUALIZATIONWRAPPER_STUB

#include "layoutvisualizationwrapper.h"
#include <stubbase.h>

class LayoutVisualizationWrapperStub : public StubBase {
public:
    virtual void LayoutVisualizationWrapperConstructor(QGraphicsLinearLayout &layout);
    virtual void LayoutVisualizationWrapperDestructor();
    virtual void setWrappingMode(LayoutVisualizationWrapper::WrappingMode mode);
    virtual qreal parentLayoutWidth();
};


void LayoutVisualizationWrapperStub::LayoutVisualizationWrapperConstructor(QGraphicsLinearLayout &layout) {
    QList<ParameterBase*> params;
    params.append(new Parameter<QGraphicsLinearLayout&>(layout));
    stubMethodEntered("Constructor(QGraphicsLinearLayout)", params);
}

void LayoutVisualizationWrapperStub::LayoutVisualizationWrapperDestructor() {
    stubMethodEntered("Destructor");
}

void LayoutVisualizationWrapperStub::setWrappingMode(LayoutVisualizationWrapper::WrappingMode mode) {
    QList<ParameterBase*> params;
    params.append(new Parameter<LayoutVisualizationWrapper::WrappingMode>(mode));
    stubMethodEntered("setWrappingMode", params);
}
qreal LayoutVisualizationWrapperStub::parentLayoutWidth() {
    stubMethodEntered("parentLayoutWidth");
    return stubReturnValue<qreal>("0.0");
}


LayoutVisualizationWrapperStub gDefaultLayoutVisualizationWrapperStub;
LayoutVisualizationWrapperStub* gLayoutVisualizationWrapperStub = &gDefaultLayoutVisualizationWrapperStub;


LayoutVisualizationWrapper::LayoutVisualizationWrapper(QGraphicsLinearLayout &layout) :
        layout(layout)
{
    gLayoutVisualizationWrapperStub->LayoutVisualizationWrapperConstructor(layout);
}

LayoutVisualizationWrapper::~LayoutVisualizationWrapper() {
    gLayoutVisualizationWrapperStub->LayoutVisualizationWrapperDestructor();
}

void LayoutVisualizationWrapper::setWrappingMode(WrappingMode mode) {
    gLayoutVisualizationWrapperStub->setWrappingMode(mode);
}

void LayoutVisualizationWrapper::resetTransformation(int) const {
}

qreal LayoutVisualizationWrapper::parentLayoutWidth() const {
    return gLayoutVisualizationWrapperStub->parentLayoutWidth();
}

void LayoutVisualizationWrapper::setTransformation(int, Direction) {
}

#endif
