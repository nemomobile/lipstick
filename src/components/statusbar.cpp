
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
#include <QGraphicsView>
#include <QTimer>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xdamage.h>

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
    setAcceptTouchEvents(true);
    setImplicitHeight(36);
    QTimer::singleShot(3000, this, SLOT(initializeStatusBar()));
}

void StatusBar::initializeStatusBar()
{
    _sharedPixmap = fetchSharedPixmap();
    setImplicitHeight(_sharedPixmap.height() / 2);
    updateXThings();
}

void StatusBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if (_sharedPixmap.isNull())
    {
        qDebug() << "StatusBar: the shared pixmap is null, can't draw it!";
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
    unsigned long data[4] = { (int)p.x(), (int)p.y(), width(), height() };
    qDebug() << "statusbar geo:" << (int)p.x() << (int)p.y() << width() << height();

    // Orientation angle
    int angle = isPortrait() ? 270 : 0;
    qDebug() << "orientation angle:" << angle;

    // Stuff for X
    QWidget *activeWindow = this->scene()->views().at(0);
    Display *dpy = QX11Info::display();

    // Setting the status bar geometry atom (probably not necessary here)
    Atom statusBarGeometryAtom = XInternAtom(dpy, "_MEEGOTOUCH_MSTATUSBAR_GEOMETRY", False);
    XChangeProperty(dpy, activeWindow->effectiveWinId(), statusBarGeometryAtom, XA_CARDINAL, 32, PropModeReplace, (unsigned char*)data, 4);

    // Setting the orientation angle atom (sysuid uses this to determine what orientation it should draw itself)
    Atom orientationAngleAtom = XInternAtom(dpy, "_MEEGOTOUCH_ORIENTATION_ANGLE", False);
    XChangeProperty(dpy, activeWindow->effectiveWinId(), orientationAngleAtom, XA_CARDINAL, 32, PropModeReplace, (unsigned char*)&angle, 1);

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

bool StatusBar::sceneEvent(QEvent *event)
{
    if (event->type() == QEvent::TouchBegin)
    {
        return true;
    }
    if (event->type() == QEvent::TouchEnd)
    {
        qDebug() << "opening status menu";

        QDBusInterface interface("com.meego.core.MStatusIndicatorMenu",
                                 "/statusindicatormenu",
                                 "com.meego.core.MStatusIndicatorMenu",
                                 QDBusConnection::sessionBus());

        interface.call(QDBus::NoBlock, "open");

        return true;
    }

    return false;
}
