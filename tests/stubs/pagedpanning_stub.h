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

#ifndef PAGEDPANNING_STUB
#define PAGEDPANNING_STUB

#include "pagedpanning.h"
#include <stubbase.h>


class PagedPanningStub : public StubBase {
public:
    virtual void PagedPanningConstructor(QObject *parent);
    virtual void PagedPanningDestructor();
    virtual void setPageCount(int pageCount);
    virtual int pageCount();
    virtual void panToPage(int page);
    virtual void setPage(uint page);
    virtual void setVelocityThreshold(qreal value);
    virtual void setDragThreshold(qreal value);
    virtual void setSlideLimit(int value);
    virtual void setPageSnapSpringK(qreal value);
    virtual void setPageSnapFriction(qreal value);
    virtual void setPanThreshold(qreal value);
    virtual int activePage();
    virtual int targetPage();
    virtual float pageWidth();
    virtual void setPageWrapMode(bool enable);
    virtual bool pageWrapMode();
    virtual void integrateAxis(Qt::Orientation orientation, qreal &position, qreal &velocity, qreal &acceleration, qreal &pointerDifference, bool pointerPressed);
    virtual void pointerPress(const QPointF &pos);
    virtual void pointerMove(const QPointF &pos);
    virtual void pointerRelease();
    virtual void panToCurrentPage();
};

void PagedPanningStub::PagedPanningConstructor(QObject *parent) {
    QList<ParameterBase*> params;
    params.append(new Parameter<QObject*>(parent));
    stubMethodEntered("Constructor(QObject*)", params);
}

void PagedPanningStub::PagedPanningDestructor() {
    stubMethodEntered("Destructor");
}

void PagedPanningStub::setPageCount(int pageCount) {
    QList<ParameterBase*> params;
    params.append(new Parameter<int>(pageCount));
    stubMethodEntered("setPageCount",params);
}

int PagedPanningStub::pageCount() {
    stubMethodEntered("pageCount");
    return stubReturnValue<int>("pageCount");
}

void PagedPanningStub::panToPage(int page) {
    QList<ParameterBase*> params;
    params.append(new Parameter<int>(page));
    stubMethodEntered("panToPage",params);
}

void PagedPanningStub::setPage(uint page) {
    QList<ParameterBase*> params;
    params.append(new Parameter<uint>(page));
    stubMethodEntered("setPage",params);
}

void PagedPanningStub::setVelocityThreshold(qreal value) {
    QList<ParameterBase*> params;
    params.append(new Parameter<qreal>(value));
    stubMethodEntered("setVelocityThreshold",params);
}

void PagedPanningStub::setDragThreshold(qreal value) {
    QList<ParameterBase*> params;
    params.append(new Parameter<qreal>(value));
    stubMethodEntered("setDragThreshold",params);
}

void PagedPanningStub::setSlideLimit(int value) {
    QList<ParameterBase*> params;
    params.append(new Parameter<int>(value));
    stubMethodEntered("setSlideLimit",params);
}

void PagedPanningStub::setPageSnapSpringK(qreal value) {
    QList<ParameterBase*> params;
    params.append(new Parameter<qreal>(value));
    stubMethodEntered("setPageSnapSpringK",params);
}

void PagedPanningStub::setPageSnapFriction(qreal value) {
    QList<ParameterBase*> params;
    params.append(new Parameter<qreal>(value));
    stubMethodEntered("setPageSnapFriction",params);
}

void PagedPanningStub::setPanThreshold(qreal value) {
    QList<ParameterBase*> params;
    params.append(new Parameter<qreal>(value));
    stubMethodEntered("setPanThreshold",params);
}

int PagedPanningStub::activePage() {
    stubMethodEntered("activePage");
    return stubReturnValue<int>("activePage");
}

int PagedPanningStub::targetPage() {
    stubMethodEntered("targetPage");
    return stubReturnValue<int>("targetPage");
}

float PagedPanningStub::pageWidth() {
    stubMethodEntered("pageWidth");
    return stubReturnValue<float>("pageWidth");
}

void PagedPanningStub::setPageWrapMode(bool enable) {
    QList<ParameterBase*> params;
    params.append(new Parameter<bool>(enable));
    stubMethodEntered("setPageWrapMode",params);
}

bool PagedPanningStub::pageWrapMode() {
    stubMethodEntered("pageWrapMode");
    return stubReturnValue<bool>("pageWrapMode");
}

void PagedPanningStub::integrateAxis(Qt::Orientation orientation, qreal &position, qreal &velocity, qreal &acceleration, qreal &pointerDifference, bool pointerPressed) {
    QList<ParameterBase*> params;
    params.append(new Parameter<Qt::Orientation>(orientation));
    params.append(new Parameter<qreal &>(position));
    params.append(new Parameter<qreal &>(velocity));
    params.append(new Parameter<qreal &>(acceleration));
    params.append(new Parameter<qreal &>(pointerDifference));
    params.append(new Parameter<bool>(pointerPressed));
    stubMethodEntered("integrateAxis",params);
}

void PagedPanningStub::pointerPress(const QPointF &pos) {
    QList<ParameterBase*> params;
    params.append(new Parameter<const QPointF &>(pos));
    stubMethodEntered("pointerPress",params);
}

void PagedPanningStub::pointerMove(const QPointF &pos) {
    QList<ParameterBase*> params;
    params.append(new Parameter<const QPointF &>(pos));
    stubMethodEntered("pointerMove",params);
}

void PagedPanningStub::pointerRelease() {
    stubMethodEntered("pointerRelease");
}

void PagedPanningStub::panToCurrentPage() {
    stubMethodEntered("panToCurrentPage");
}


PagedPanningStub gDefaultPagedPanningStub;
PagedPanningStub* gPagedPanningStub = &gDefaultPagedPanningStub;


PagedPanning::PagedPanning(QObject *parent) :
        MPhysics2DPanning(parent)
{
    gPagedPanningStub->PagedPanningConstructor(parent);
}

PagedPanning::~PagedPanning() {
    gPagedPanningStub->PagedPanningDestructor();
}

void PagedPanning::setPageCount(int pageCount) {
    gPagedPanningStub->setPageCount(pageCount);
}

int PagedPanning::pageCount() const {
    return gPagedPanningStub->pageCount();
}

void PagedPanning::panToPage(int page) {
    gPagedPanningStub->panToPage(page);
}

void PagedPanning::setPage(uint page) {
    gPagedPanningStub->setPage(page);
}

void PagedPanning::setVelocityThreshold(qreal value) {
    gPagedPanningStub->setVelocityThreshold(value);
}

void PagedPanning::setDragThreshold(qreal value) {
    gPagedPanningStub->setDragThreshold(value);
}

void PagedPanning::setSlideLimit(int value) {
    gPagedPanningStub->setSlideLimit(value);
}

void PagedPanning::setPageSnapSpringK(qreal value) {
    gPagedPanningStub->setPageSnapSpringK(value);
}

void PagedPanning::setPageSnapFriction(qreal value) {
    gPagedPanningStub->setPageSnapFriction(value);
}

void PagedPanning::setPanThreshold(qreal value) {
    gPagedPanningStub->setPanThreshold(value);
}

int PagedPanning::activePage() const {
    return gPagedPanningStub->activePage();
}

int PagedPanning::targetPage() const {
    return gPagedPanningStub->targetPage();
}

float PagedPanning::pageWidth() const {
    return gPagedPanningStub->pageWidth();
}

void PagedPanning::setPageWrapMode(bool enable) {
    gPagedPanningStub->setPageWrapMode(enable);
}

bool PagedPanning::pageWrapMode() const {
    return gPagedPanningStub->pageWrapMode();
}

void PagedPanning::integrateAxis(Qt::Orientation orientation, qreal &position, qreal &velocity, qreal &acceleration, qreal &pointerDifference, bool pointerPressed) {
    gPagedPanningStub->integrateAxis(orientation, position, velocity, acceleration, pointerDifference, pointerPressed);
}

void PagedPanning::pointerPress(const QPointF &pos) {
    gPagedPanningStub->pointerPress(pos);
}

void PagedPanning::pointerMove(const QPointF &pos) {
    gPagedPanningStub->pointerMove(pos);
}

void PagedPanning::pointerRelease() {
    gPagedPanningStub->pointerRelease();
}

void PagedPanning::panToCurrentPage() {
    gPagedPanningStub->panToCurrentPage();
}

#endif
