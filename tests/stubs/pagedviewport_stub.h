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

#ifndef PAGEDVIEWPORT_STUB
#define PAGEDVIEWPORT_STUB

#include "pagedviewport.h"
#include <stubbase.h>

class PagedViewportStub : public StubBase {
public:
    virtual void PagedViewportConstructor(QGraphicsItem *parent);
    virtual void PagedViewportDestructor();
    virtual void setPanDirection(const Qt::Orientations &panDirection);
    virtual void updatePageCount(int pages);
    virtual int currentPage();
    virtual int targetPage();
    virtual float pageWidth();
    virtual void setPageWrapMode(bool enable);
    virtual void panToPage(uint page);
    virtual void setPage(uint page);
    virtual void focusFirstPage();
    virtual void newPageSet(int newPage);
    virtual void updateVisualizationWrapper();
};

// 2. IMPLEMENT STUB
void PagedViewportStub::PagedViewportConstructor(QGraphicsItem *parent) {
    QList<ParameterBase*> params;
    params.append(new Parameter<QGraphicsItem*>(parent));
    stubMethodEntered("Constructor(QGraphicsItem*)", params);
}

void PagedViewportStub::PagedViewportDestructor() {
    stubMethodEntered("Destructor");
}

void PagedViewportStub::setPanDirection(const Qt::Orientations &panDirection) {
    QList<ParameterBase*> params;
    params.append(new Parameter<const Qt::Orientations&>(panDirection));
    stubMethodEntered("setPanDirection", params);
}

void PagedViewportStub::updatePageCount(int pages) {
    QList<ParameterBase*> params;
    params.append(new Parameter<int>(pages));
    stubMethodEntered("updatePageCount", params);
}

int PagedViewportStub::currentPage() {
    stubMethodEntered("currentPage");
    return stubReturnValue<int>("currentPage");
}

int PagedViewportStub::targetPage() {
    stubMethodEntered("targetPage");
    return stubReturnValue<int>("targetPage");
}

float PagedViewportStub::pageWidth() {
    stubMethodEntered("pageWidth");
    return stubReturnValue<float>("pageWidth");
}

void PagedViewportStub::setPageWrapMode(bool enable) {
    QList<ParameterBase*> params;
    params.append(new Parameter<bool>(enable));
    stubMethodEntered("setPageWrapMode", params);
}

void PagedViewportStub::panToPage(uint page) {
    QList<ParameterBase*> params;
    params.append(new Parameter<uint>(page));
    stubMethodEntered("panToPage", params);
}

void PagedViewportStub::setPage(uint page) {
    QList<ParameterBase*> params;
    params.append(new Parameter<uint>(page));
    stubMethodEntered("setPage", params);
}

void PagedViewportStub::focusFirstPage() {
    stubMethodEntered("focusFirstPage");
}

void PagedViewportStub::newPageSet(int newPage) {
    QList<ParameterBase*> params;
    params.append(new Parameter<int>(newPage));
    stubMethodEntered("newPageSet", params);
}

void PagedViewportStub::updateVisualizationWrapper() {
    stubMethodEntered("updateVisualizationWrapper");
}


PagedViewportStub gDefaultPagedViewportStub;
PagedViewportStub* gPagedViewportStub = &gDefaultPagedViewportStub;


PagedViewport::PagedViewport(QGraphicsItem *parent) :
        MPannableViewport(parent)
{
    gPagedViewportStub->PagedViewportConstructor(parent);
}

PagedViewport::~PagedViewport() {
    gPagedViewportStub->PagedViewportDestructor();
}

void PagedViewport::setPanDirection(const Qt::Orientations &panDirection) {
    gPagedViewportStub->setPanDirection(panDirection);
}

void PagedViewport::updatePageCount(int pages) {
    gPagedViewportStub->updatePageCount(pages);
}

int PagedViewport::currentPage() const {
    return gPagedViewportStub->currentPage();
}

int PagedViewport::targetPage() const {
    return gPagedViewportStub->targetPage();
}

float PagedViewport::pageWidth() const {
    return gPagedViewportStub->pageWidth();
}

void PagedViewport::setPageWrapMode(bool enable) {
    gPagedViewportStub->setPageWrapMode(enable);
}

void PagedViewport::panToPage(uint page) {
    gPagedViewportStub->panToPage(page);
}

void PagedViewport::setPage(uint page) {
    gPagedViewportStub->setPage(page);
}

void PagedViewport::focusFirstPage() {
    gPagedViewportStub->focusFirstPage();
}

void PagedViewport::newPageSet(int newPage) {
    gPagedViewportStub->newPageSet(newPage);
}

void PagedViewport::updateVisualizationWrapper() {
    gPagedViewportStub->updateVisualizationWrapper();
}

#endif
