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
#include <cmath>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLinearLayout>
#include "mainwindow.h"
#include <QX11Info>
#include <QPainter>
#include <QApplication>
#include <MScalableImage>
#include <MCancelEvent>
#include <MSceneManager>
#include "switcherbuttonview.h"
#include "switcherbuttonviewxeventlistener.h"
#include "switcherbutton.h"
#include "x11wrapper.h"

#ifdef Q_WS_X11
unsigned char SwitcherButtonView::xErrorCode = Success;
#endif

// Time between icon geometry updates in milliseconds
static const int ICON_GEOMETRY_UPDATE_INTERVAL = 200;
Atom SwitcherButtonView::iconGeometryAtom = 0;

SwitcherButtonView::SwitcherButtonView(SwitcherButton *button) :
    MButtonView(button),
    controller(button),
    xWindowPixmap(0),
    xWindowPixmapIsValid(false),
    xWindowPixmapDamage(0),
    onDisplay(false),
    xEventListener(new SwitcherButtonViewXEventListener(*this))
{
    QGraphicsLinearLayout *titleBarLayout = new QGraphicsLinearLayout(Qt::Horizontal, controller);
    titleBarLayout->setContentsMargins(0,0,0,0);
    titleBarLayout->addStretch();
    controller->setLayout(titleBarLayout);

    // Enable or disable reception of damage events based on whether the switcher button is on the screen or not
    connect(button, SIGNAL(displayEntered()), this, SLOT(setOnDisplay()));
    connect(button, SIGNAL(displayExited()), this, SLOT(unsetOnDisplay()));

    if (iconGeometryAtom == 0) {
        // Get the icon geometry X11 Atom if it doesn't exist yet
        iconGeometryAtom = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_WM_ICON_GEOMETRY", False);
    }

    // Set up the timer for updating the icon geometry
    updateXWindowIconGeometryTimer.setSingleShot(true);
    updateXWindowIconGeometryTimer.setInterval(ICON_GEOMETRY_UPDATE_INTERVAL);
    connect(&updateXWindowIconGeometryTimer, SIGNAL(timeout()), this, SLOT(updateXWindowIconGeometry()));
}

SwitcherButtonView::~SwitcherButtonView()
{
#ifdef Q_WS_X11
    // Unregister the pixmap from XDamage events
    destroyDamage();

    if (xWindowPixmap != 0) {
        // Dereference the old pixmap ID
        X11Wrapper::XFreePixmap(QX11Info::display(), xWindowPixmap);
    }
#endif
}

void SwitcherButtonView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (!xWindowPixmapIsValid) {
        // Try to update the X window pixmap if it's not valid
        updateXWindowPixmap();
    }

    MButtonView::paint(painter, option, widget);
}

void SwitcherButtonView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *) const
{
    // Store the painter state
    painter->save();
    painter->setRenderHint(QPainter::SmoothPixmapTransform);

    QSize size(style()->iconSize());

    // Rotate the thumbnails and adjust their size if the screen
    // has been rotated
    MSceneManager *manager = MainWindow::instance()->sceneManager();
    painter->rotate(-manager->orientationAngle());

    if (manager->orientation() == M::Portrait) {
        size.transpose();
    }

    int croppingTop = style()->croppingTop();
    QPoint pos;
    QPoint iconPos(thumbnailPosition());
    QRect source(0, 0, qWindowPixmap.width(), qWindowPixmap.height());
    switch (manager->orientationAngle()) {
        case M::Angle90:
            pos -= QPoint(iconPos.y() + size.width(), -iconPos.x());
            if (qWindowPixmap.width() > croppingTop) {
                source.setWidth(qWindowPixmap.width() - croppingTop);
            }
            break;
        case M::Angle180:
            pos -= QPoint(iconPos.x() + size.width(), iconPos.y() + size.height());
            if (qWindowPixmap.height() > croppingTop) {
                source.setHeight(qWindowPixmap.height() - croppingTop);
            }
            break;
        case M::Angle270:
            pos -= QPoint(-iconPos.y(), iconPos.x() + size.height());
            if (qWindowPixmap.width() > croppingTop) {
                source.setLeft(croppingTop);
                source.setWidth(qWindowPixmap.width() - croppingTop);
            }
            break;
        default:
            pos += iconPos;
            if (qWindowPixmap.height() > croppingTop) {
                source.setTop(croppingTop);
                source.setHeight(qWindowPixmap.height() - croppingTop);
            }
            break;
    }

    // Do the actual drawing
    QT_TRY {
        painter->drawPixmap(QRect(pos, size), qWindowPixmap, source);
    } QT_CATCH (std::bad_alloc e) {
        // XGetImage() failed, the window has been already unmapped.
    }

    // Restore the painter state
    painter->restore();

    // Update the X window _NET_WM_ICON_GEOMETRY property if necessary
    updateXWindowIconGeometryIfNecessary();
}

void SwitcherButtonView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *) const
{
    // Store the painter state
    painter->save();

    // Draw the container
    const MScalableImage *container = style()->containerImage();
    if (container != NULL) {
        container->draw(QRect(QPoint(0, 0), size().toSize()), painter);
    }

    // Restore the painter state
    painter->restore();
}

QPoint SwitcherButtonView::thumbnailPosition() const
{
    return QPoint();
}

void SwitcherButtonView::setupModel()
{
    // The MButtonView implementation is skipped on purpose so that MButtonView's MLabel is not set up
    MWidgetView::setupModel();

    if (model()->xWindow() != 0) {
        // The X window has changed so the pixmap needs to be updated before it can be drawn
        xWindowPixmapIsValid = false;
        update();

        // Each window should always have at least some kind of a value for _NET_WM_ICON_GEOMETRY
        updateXWindowIconGeometry();
    }
    updateViewMode();
}

void SwitcherButtonView::updateData(const QList<const char *>& modifications)
{
    // The MButtonView implementation is skipped on purpose so that MButtonView's MLabel is not set up
    MWidgetView::updateData(modifications);

    const char *member;
    foreach(member, modifications) {
        if (member == SwitcherButtonModel::XWindow && model()->xWindow() != 0) {
            // The X window has changed so the pixmap needs to be updated before it can be drawn
            xWindowPixmapIsValid = false;
            update();

            // Each window should always have at least some kind of a value for _NET_WM_ICON_GEOMETRY
            updateXWindowIconGeometry();
        } else if (member == SwitcherButtonModel::ViewMode) {
            updateViewMode();
        }
    }
}

void SwitcherButtonView::applyStyle()
{
    // MButtonView's implementation must be called starting from libmeegotouch 0.20.86
    MButtonView::applyStyle();
}

void SwitcherButtonView::updateViewMode()
{
    switch (model()->viewMode()) {
    case SwitcherButtonModel::Small:
        style().setModeSmall();
        break;
    case SwitcherButtonModel::Medium:
        style().setModeMedium();
        break;
    case SwitcherButtonModel::Large:
        style().setModeLarge();
        break;
    }

    // When the style mode changes, the style is not automatically applied -> call it explicitly (skipping the MButtonView's implementation so that the mode setting does not get reset)
    MWidgetView::applyStyle();
}

void SwitcherButtonView::updateXWindowPixmap()
{
#ifdef Q_WS_X11
    // It is possible that the window is not redirected so check for errors.
    // XSync() needs to be called so that previous errors go to the original
    // handler.
    X11Wrapper::XSync(QX11Info::display(), FALSE);
    XErrorHandler errh = X11Wrapper::XSetErrorHandler(handleXError);
    xErrorCode = Success;

    // Get the pixmap ID of the X window
    Pixmap newWindowPixmap = X11Wrapper::XCompositeNameWindowPixmap(QX11Info::display(), model()->xWindow());

    // XCompositeNameWindowPixmap doesn't wait for the server to reply, we'll
    // need to do it ourselves to catch the possible BadMatch
    X11Wrapper::XSync(QX11Info::display(), FALSE);

    xWindowPixmapIsValid = xErrorCode == Success;
    if (xWindowPixmapIsValid) {
        // Unregister the old pixmap from XDamage events
        destroyDamage();

        if (xWindowPixmap != 0) {
            // Dereference the old pixmap ID
            X11Wrapper::XFreePixmap(QX11Info::display(), xWindowPixmap);
        }

        xWindowPixmap = newWindowPixmap;

        // Register the pixmap for XDamage events
        createDamage();

        qWindowPixmap = QPixmap::fromX11Pixmap(xWindowPixmap, QPixmap::ExplicitlyShared);
    } else {
        // If a BadMatch error occurred the window wasn't redirected yet; deference the invalid pixmap
        if (newWindowPixmap != 0) {
            X11Wrapper::XFreePixmap(QX11Info::display(), newWindowPixmap);
        }
    }

    // Reset the error handler
    X11Wrapper::XSetErrorHandler(errh);
#endif
}

#ifdef Q_WS_X11
int SwitcherButtonView::handleXError(Display *, XErrorEvent *event)
{
    xErrorCode = event->error_code;

    return 0;
}
#endif

bool SwitcherButtonView::windowFullyObscured(Window window)
{
    bool ownWindow = window == model()->xWindow();

    if (ownWindow) {
        // The X window has been fully obscured so the pixmap needs to be updated before it can be drawn
        xWindowPixmapIsValid = false;
        update();
    }

    return ownWindow;
}

void SwitcherButtonView::damageEvent(Qt::HANDLE &damage, short &x, short &y, unsigned short &width, unsigned short &height)
{
    Q_UNUSED(x);
    Q_UNUSED(y);
    Q_UNUSED(width);
    Q_UNUSED(height);
#ifdef Q_WS_X11
    if (xWindowPixmapDamage == damage) {
        X11Wrapper::XDamageSubtract(QX11Info::display(), xWindowPixmapDamage, None, None);
        update();
    }
#else
    Q_UNUSED(damage);
#endif
}

void SwitcherButtonView::setOnDisplay()
{
    onDisplay = true;
    createDamage();

    // Start listening to X pixmap change signals
    connect(qApp, SIGNAL(damageEvent(Qt::HANDLE &, short &, short &, unsigned short &, unsigned short &)), this, SLOT(damageEvent(Qt::HANDLE &, short &, short &, unsigned short &, unsigned short &)));
    update();
}

void SwitcherButtonView::unsetOnDisplay()
{
    onDisplay = false;
    destroyDamage();

    // Disconnect the damage signal so that off-screen buttons aren't signaled by damage notifications of other buttons
    disconnect(qApp, SIGNAL(damageEvent(Qt::HANDLE &, short &, short &, unsigned short &, unsigned short &)), this, SLOT(damageEvent(Qt::HANDLE &, short &, short &, unsigned short &, unsigned short &)));
}

void SwitcherButtonView::createDamage()
{
#ifdef Q_WS_X11
    if (onDisplay && model()->xWindow() != 0) {
        // Register the pixmap for XDamage events
        xWindowPixmapDamage = X11Wrapper::XDamageCreate(QX11Info::display(), model()->xWindow(), XDamageReportNonEmpty);
    }
#endif
}

void SwitcherButtonView::destroyDamage()
{
#ifdef Q_WS_X11
    if (xWindowPixmapDamage != 0) {
        X11Wrapper::XDamageDestroy(QX11Info::display(), xWindowPixmapDamage);
        xWindowPixmapDamage = 0;
    }
#endif
}

void SwitcherButtonView::updateXWindowIconGeometryIfNecessary() const
{
    // Update only if position has changed
    if (updatedXWindowIconGeometry.topLeft() != controller->mapToScene(thumbnailPosition())) {
        // Update the icon geometry in a moment. If timer was already active, restart it so
        // we don't send constantly updates while the button is moving.
        updateXWindowIconGeometryTimer.start();
    }
}

void SwitcherButtonView::updateXWindowIconGeometry()
{
    // Get the geometry of the Switcher Button in scene coordinates
    QPointF topLeft(controller->mapToScene(thumbnailPosition()));
    QPointF bottomRight(controller->mapToScene(thumbnailPosition().x() + style()->iconSize().width(), thumbnailPosition().y() + style()->iconSize().height()));
    QRectF iconSceneGeometry;
    iconSceneGeometry.setCoords(topLeft.x(), topLeft.y(), bottomRight.x(), bottomRight.y());
    iconSceneGeometry = iconSceneGeometry.normalized();

    // Update only if geometry has changed
    if (updatedXWindowIconGeometry != iconSceneGeometry) {
        // Replace the old X icon geometry property for the window with iconGeometry, which consists of 4 unsigned ints (32 bits)
        unsigned int iconGeometry[4];
        iconGeometry[0] = iconSceneGeometry.x();
        iconGeometry[1] = iconSceneGeometry.y();
        iconGeometry[2] = iconSceneGeometry.width();
        iconGeometry[3] = iconSceneGeometry.height();
        X11Wrapper::XChangeProperty(QX11Info::display(), model()->xWindow(), iconGeometryAtom, XA_CARDINAL, sizeof(unsigned int) * 8, PropModeReplace, (unsigned char *)&iconGeometry, 4);

        // Store which position has already been updated
        updatedXWindowIconGeometry = iconSceneGeometry;
    }
}

M_REGISTER_VIEW_NEW(SwitcherButtonView, SwitcherButton)
