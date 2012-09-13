
// This file is part of lipstick, a QML desktop library
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License version 2.1 as published by the Free Software Foundation
// and appearing in the file LICENSE.LGPL included in the packaging
// of this file.
//
// This code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// Copyright (c) 2012, Timur Kristóf <venemo@fedoraproject.org>

#include "statusbar.h"

#include <QPainter>
#include <QX11Info>
#include <QTouchEvent>
#include <QDBusInterface>
#include <QDBusConnection>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsView>
#include <QTimer>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xdamage.h>

// Define this if you'd like to see debug messages from the status bar
#ifdef DEBUG_STATUSBAR
#define STATUSBAR_DEBUG(things) qDebug() << Q_FUNC_INFO << things
#else
#define STATUSBAR_DEBUG(things)
#endif

// Fetches the shared status bar pixmap
static QPixmap fetchSharedPixmap()
{
    static Atom propertyWindowAtom = 0;
    static Atom pixmapHandleAtom = 0;

    // This contains the statusbar window
    if (propertyWindowAtom == 0)
        propertyWindowAtom = XInternAtom(QX11Info::display(), "_MEEGOTOUCH_STATUSBAR_PROPERTY_WINDOW", False);

    // This contains the shared pixmap
    if (pixmapHandleAtom == 0)
        pixmapHandleAtom = XInternAtom(QX11Info::display(), "_MEEGOTOUCH_STATUSBAR_PIXMAP", False);

    // Some common variables
    Atom actualType = 0;
    int actualFormat = 0;
    unsigned long nitems = 0;
    unsigned long bytesAfter = 0;
    unsigned char *data = 0;
    int status = 0;

    // Getting the status bar window
    status = XGetWindowProperty(QX11Info::display(),
                                QX11Info::appRootWindow(),
                                propertyWindowAtom,
                                0,
                                1,
                                False,
                                XA_WINDOW,
                                &actualType,
                                &actualFormat,
                                &nitems,
                                &bytesAfter,
                                &data);

    Window window(0);
    if (status == Success && data != None)
    {
        window = *(Window *)data;
        XFree(data);
    }

    // Getting the shared pixmap from the status bar window
    status = XGetWindowProperty(QX11Info::display(),
                                window,
                                pixmapHandleAtom,
                                0,
                                1,
                                False,
                                XA_PIXMAP,
                                &actualType,
                                &actualFormat,
                                &nitems,
                                &bytesAfter,
                                &data);

    QPixmap result;

    if (status == Success)
    {
        quint32 handle = *((unsigned long*)data);

        if (actualType == XA_PIXMAP && actualFormat == 32 && handle != 0)
        {
            result = QPixmap::fromX11Pixmap(handle, QPixmap::ExplicitlyShared);
            // TODO: we should register for damage events for this pixmap and repaint when they arrive
            //       (perhaps create an XEventListener fromt his class too?)
        }

        XFree(data);
    }

    return result;
}

StatusBar::StatusBar(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
    setAcceptedMouseButtons(Qt::LeftButton);
    setImplicitHeight(36);
    QTimer::singleShot(0, this, SLOT(initializeStatusBar()));
}

void StatusBar::initializeStatusBar()
{
    _sharedPixmap = fetchSharedPixmap();

    if (_sharedPixmap.isNull())
    {
        QTimer::singleShot(1000, this, SLOT(initializeStatusBar()));
        return;
    }

    setImplicitHeight(_sharedPixmap.height() / 2);
    updateXThings();
}

void StatusBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if (_sharedPixmap.isNull())
    {
        STATUSBAR_DEBUG("the shared pixmap is null, can't draw it!");
        painter->setPen(QColor(Qt::black));
        painter->drawRect(0, 0, width(), height());
        return;
    }

    // Info: The shared pixmap contains both the portrait and the landscape mode status bars below each other.
    //       Landscape is on top, portrait on bottom. They have the same height.
    //       All we need is check the orientation here and set the source rect accordingly.

    painter->drawPixmap(QRectF(0, 0, width(), height()), _sharedPixmap, QRectF(0, _isPortrait ? height() : 0, width(), height()));
}

bool StatusBar::isPortrait() const
{
    return _isPortrait;
}

void StatusBar::updateXThings()
{
    // Statusbar rect
    QPointF p = mapToScene(0, 0);
    unsigned long data[4] = { (unsigned long)p.x(), (unsigned long)p.y(), (unsigned long)width(), (unsigned long)height() };
    STATUSBAR_DEBUG("statusbar geo:" << (int)p.x() << (int)p.y() << (int)width() << (int)height());

    // Orientation angle
    int angle = isPortrait() ? 270 : 0;
    STATUSBAR_DEBUG("orientation angle:" << angle);

    if (!this->scene() || this->scene()->views().length() == 0)
    {
        STATUSBAR_DEBUG("Something's not ok, trying to get back here later");
        QTimer::singleShot(0, this, SLOT(updateXThings()));
        return;
    }

    // Stuff for X
    QWidget *activeWindow = this->scene()->views().at(0);
    Display *dpy = QX11Info::display();

    STATUSBAR_DEBUG("starting to set X properties");

    // Setting the status bar geometry atom (probably not necessary here)
    Atom statusBarGeometryAtom = XInternAtom(dpy, "_MEEGOTOUCH_MSTATUSBAR_GEOMETRY", False);
    XChangeProperty(dpy, activeWindow->winId(), statusBarGeometryAtom, XA_CARDINAL, 32, PropModeReplace, (unsigned char*)data, 4);
    STATUSBAR_DEBUG("called XChangeProperty for _MEEGOTOUCH_MSTATUSBAR_GEOMETRY");

    // Setting the orientation angle atom (sysuid uses this to determine what orientation it should draw itself)
    Atom orientationAngleAtom = XInternAtom(dpy, "_MEEGOTOUCH_ORIENTATION_ANGLE", False);
    XChangeProperty(dpy, activeWindow->winId(), orientationAngleAtom, XA_CARDINAL, 32, PropModeReplace, (unsigned char*)&angle, 1);
    STATUSBAR_DEBUG("called XChangeProperty for _MEEGOTOUCH_ORIENTATION_ANGLE");

    update();
}

void StatusBar::setIsPortrait(bool value)
{
    // If there is no change, don't bother
    if (_isPortrait == value)
        return;

    _isPortrait = value;
    updateXThings();

    emit isPortraitChanged();
}

void StatusBar::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
}

void StatusBar::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();

    QDBusInterface interface("com.meego.core.MStatusIndicatorMenu",
                             "/statusindicatormenu",
                             "com.meego.core.MStatusIndicatorMenu",
                             QDBusConnection::sessionBus());

    interface.call(QDBus::NoBlock, "open");
}

void StatusBar::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
}

void StatusBar::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
}

void StatusBar::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    event->accept();
}

void StatusBar::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    event->accept();
}

void StatusBar::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    event->accept();
}

void StatusBar::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    event->accept();
}

