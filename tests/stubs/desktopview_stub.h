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
#ifndef DESKTOPVIEW_STUB
#define DESKTOPVIEW_STUB

#include "desktopview.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DesktopViewStub : public StubBase
{
public:
    virtual void DesktopViewConstructor(Desktop *controller);
    virtual void DesktopViewDestructor();
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void setGeometry(const QRectF &rect);
    virtual QRectF boundingRect() const;
    virtual void updatePosition(const QPointF &pos);
    virtual void widgetInserted(uint index);
    virtual void widgetAboutToBeRemoved(uint index);
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const;
};

// 2. IMPLEMENT STUB
void DesktopViewStub::DesktopViewConstructor(Desktop *controller)
{
    Q_UNUSED(controller);

}
void DesktopViewStub::DesktopViewDestructor()
{

}
void DesktopViewStub::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QPainter * >(painter));
    params.append(new Parameter<const QStyleOptionGraphicsItem * >(option));
    params.append(new Parameter<QWidget * >(widget));
    stubMethodEntered("paint", params);
}

void DesktopViewStub::setGeometry(const QRectF &rect)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QRectF & >(rect));
    stubMethodEntered("setGeometry", params);
}

QRectF DesktopViewStub::boundingRect() const
{
    stubMethodEntered("boundingRect");
    return stubReturnValue<QRectF>("boundingRect");
}

void DesktopViewStub::updatePosition(const QPointF &pos)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QPointF & >(pos));
    stubMethodEntered("updatePosition", params);
}

void DesktopViewStub::widgetInserted(uint index)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(index));
    stubMethodEntered("widgetInserted", params);
}

void DesktopViewStub::widgetAboutToBeRemoved(uint index)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<uint >(index));
    stubMethodEntered("widgetAboutToBeRemoved", params);
}

QSizeF DesktopViewStub::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Qt::SizeHint>(which));
    params.append(new Parameter<QSizeF>(constraint));
    stubMethodEntered("sizeHint", params);
    return stubReturnValue<QSizeF>("sizeHint");
}

// 3. CREATE A STUB INSTANCE
DesktopViewStub gDefaultDesktopViewStub;
DesktopViewStub *gDesktopViewStub = &gDefaultDesktopViewStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DesktopView::DesktopView(Desktop *controller)
{
    gDesktopViewStub->DesktopViewConstructor(controller);
}

DesktopView::~DesktopView()
{
    gDesktopViewStub->DesktopViewDestructor();
}

void DesktopView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    gDesktopViewStub->paint(painter, option, widget);
}

void DesktopView::setGeometry(const QRectF &rect)
{
    gDesktopViewStub->setGeometry(rect);
}

QRectF DesktopView::boundingRect() const
{
    return gDesktopViewStub->boundingRect();
}

void DesktopView::updatePosition(const QPointF &pos)
{
    gDesktopViewStub->updatePosition(pos);
}

void DesktopView::widgetInserted(uint index)
{
    gDesktopViewStub->widgetInserted(index);
}

void DesktopView::widgetAboutToBeRemoved(uint index)
{
    gDesktopViewStub->widgetAboutToBeRemoved(index);
}

QSizeF DesktopView::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    return gDesktopViewStub->sizeHint(which, constraint);
}

#endif
