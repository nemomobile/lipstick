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

#include <QApplication>
#include <QPainter>
#include <QTimer>
#include <QX11Info>

#include "switcherpixmapitem.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xdamage.h>

// TODO: disable damage event processing when not on the screen
// TODO: handle visibility/obscuring invalidating pixmaps

const int ICON_GEOMETRY_UPDATE_INTERVAL = 200;
Atom iconGeometryAtom = 0;
#ifdef Q_WS_X11
unsigned char xErrorCode = Success;
#endif

#ifdef Q_WS_X11
static int handleXError(Display *, XErrorEvent *event)
{
    xErrorCode = event->error_code;

    return 0;
}
#endif

struct SwitcherPixmapItem::Private
{
    Private()
        : xWindowPixmapIsValid(false)
        , xWindowPixmap(0)
        , xWindowPixmapDamage(0)
        , windowId(0)
    {}

    bool xWindowPixmapIsValid;
    Pixmap xWindowPixmap;
    Damage xWindowPixmapDamage;
    QPixmap qWindowPixmap;
    int windowId;
    QTimer updateXWindowIconGeometryTimer;
};

SwitcherPixmapItem::SwitcherPixmapItem()
    : QDeclarativeItem()
    , d(new Private)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);

    if (iconGeometryAtom == 0)
        iconGeometryAtom = XInternAtom(QX11Info::display(), "_NET_WM_ICON_GEOMETRY", false);

    d->updateXWindowIconGeometryTimer.setSingleShot(true);
    d->updateXWindowIconGeometryTimer.setInterval(ICON_GEOMETRY_UPDATE_INTERVAL);
    connect(&d->updateXWindowIconGeometryTimer, SIGNAL(timeout()), SLOT(updateXWindowIconGeomery()));

    connect(qApp, SIGNAL(damageEvent(Qt::HANDLE &, short &, short &, unsigned short &, unsigned short &)), this, SLOT(damageEvent(Qt::HANDLE &, short &, short &, unsigned short &, unsigned short &)));
}

SwitcherPixmapItem::~SwitcherPixmapItem()
{
    destroyDamage();
    if (d->xWindowPixmap)
        XFreePixmap(QX11Info::display(), d->xWindowPixmap);
    delete d;
}

void SwitcherPixmapItem::damageEvent(Qt::HANDLE &damage, short &x, short &y, unsigned short &width, unsigned short &height)
{
    Q_UNUSED(x);
    Q_UNUSED(y);
    Q_UNUSED(width);
    Q_UNUSED(height);
#ifdef Q_WS_X11
    if (d->xWindowPixmapDamage == damage) {
        XDamageSubtract(QX11Info::display(), d->xWindowPixmapDamage, None, None);
        update();
    }
#else
    Q_UNUSED(damage);
#endif
}

void SwitcherPixmapItem::updateXWindowIconGeometry()
{
    qDebug() << Q_FUNC_INFO << "Implement me!";
}

void SwitcherPixmapItem::updateXWindowIconGeometryIfNecessary()
{
    qDebug() << Q_FUNC_INFO << "Implement me!";
}

void SwitcherPixmapItem::destroyDamage()
{
    if (d->xWindowPixmapDamage != 0) {
        XDamageDestroy(QX11Info::display(), d->xWindowPixmapDamage);
        d->xWindowPixmapDamage = 0;
    }
}

void SwitcherPixmapItem::createDamage()
{
    // TODO: check on display status, don't create damage if off
    if (d->windowId == 0)
        return;

    // Register the pixmap for XDamage events
    d->xWindowPixmapDamage = XDamageCreate(QX11Info::display(), d->windowId, XDamageReportNonEmpty);
}

void SwitcherPixmapItem::updateXWindowPixmap()
{
#ifdef Q_WS_X11
    // It is possible that the window is not redirected so check for errors.
    // XSync() needs to be called so that previous errors go to the original
    // handler.
    XSync(QX11Info::display(), FALSE);
    XErrorHandler errh = XSetErrorHandler(handleXError);
    xErrorCode = Success;

    // Get the pixmap ID of the X window
    Pixmap newWindowPixmap = XCompositeNameWindowPixmap(QX11Info::display(), d->windowId);

    // XCompositeNameWindowPixmap doesn't wait for the server to reply, we'll
    // need to do it ourselves to catch the possible BadMatch
    XSync(QX11Info::display(), FALSE);

    d->xWindowPixmapIsValid = xErrorCode == Success;
    if (d->xWindowPixmapIsValid) {
        // Unregister the old pixmap from XDamage events
        destroyDamage();

        if (d->xWindowPixmap != 0) {
            // Dereference the old pixmap ID
            XFreePixmap(QX11Info::display(), d->xWindowPixmap);
        }

        d->xWindowPixmap = newWindowPixmap;

        // Register the pixmap for XDamage events
        createDamage();

        d->qWindowPixmap = QPixmap::fromX11Pixmap(d->xWindowPixmap, QPixmap::ExplicitlyShared);
    } else {
        // If a BadMatch error occurred the window wasn't redirected yet; deference the invalid pixmap
        if (newWindowPixmap != 0) {
            XFreePixmap(QX11Info::display(), newWindowPixmap);
        }
    }

    // Reset the error handler
    XSetErrorHandler(errh);
#else
#error "not implemented"
#endif
}

void SwitcherPixmapItem::setWindowId(int window)
{
    d->windowId = window;
    d->xWindowPixmapIsValid = false;

    // TODO: should we XFreePixmap here?

    update();

    // Each window should always have at least some kind of a value for _NET_WM_ICON_GEOMETRY
    updateXWindowIconGeometry();
}

int SwitcherPixmapItem::windowId() const
{
    return d->windowId;
}

void SwitcherPixmapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                               QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (!d->xWindowPixmapIsValid) {
        updateXWindowPixmap();
    }

    QT_TRY {
        painter->drawPixmap(QRect(0, 0, boundingRect().width(), boundingRect().height()), d->qWindowPixmap);
    } QT_CATCH (std::bad_alloc e) {
        // XGetImage failed, the window has been already unmapped
    }

    updateXWindowIconGeometryIfNecessary();
}
