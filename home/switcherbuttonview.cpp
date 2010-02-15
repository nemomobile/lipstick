/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of duihome.
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
#include <QTimer>
#include <QGraphicsSceneMouseEvent>
#include "mainwindow.h"
#include <QApplication>
#include <QX11Info>
#include <QPainter>
#include <DuiScalableImage>
#include <DuiCancelEvent>
#include <DuiSceneManager>
#include "switcherbuttonview.h"
#include "switcherbutton.h"

#ifdef Q_WS_X11
bool SwitcherButtonView::badMatchOccurred = false;
#endif

SwitcherButtonView::SwitcherButtonView(SwitcherButton *button) :
    DuiWidgetView(button),
    controller(button),
    xWindowPixmap(0),
    xWindowPixmapDamage(0)
{
    // Configure timers
    windowCloseTimer.setSingleShot(true);
    connect(&windowCloseTimer, SIGNAL(timeout()), this, SLOT(resetState()));

    // Connect to the windowVisibilityChanged signal of the HomeApplication to get information about window visiblity changes
    connect(qApp, SIGNAL(windowVisibilityChanged(Window)), this, SLOT(windowVisibilityChanged(Window)));
}

SwitcherButtonView::~SwitcherButtonView()
{
#ifdef Q_WS_X11
    if (xWindowPixmapDamage != 0) {
        // Unregister the pixmap from XDamage events
        X11Wrapper::XDamageDestroy(QX11Info::display(), xWindowPixmapDamage);
    }

    if (xWindowPixmap != 0) {
        // Dereference the old pixmap ID
        X11Wrapper::XFreePixmap(QX11Info::display(), xWindowPixmap);
    }
#endif
}

void SwitcherButtonView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    // Store the painter state
    painter->save();

    // Do the actual drawing
    backendSpecificDrawBackground(painter, option);

    // Restore the painter state
    painter->restore();
}

void SwitcherButtonView::backendSpecificDrawBackground(QPainter *, const QStyleOptionGraphicsItem *) const
{
}

void SwitcherButtonView::drawContents(QPainter *painter, const QStyleOptionGraphicsItem *) const
{
    // Store the painter state
    painter->save();

    // Draw the container
    const DuiScalableImage *container = style()->containerImage();
    if (container != NULL) {
        container->draw(QRect(QPoint(0, 0), size().toSize()), painter);
    }

    // Draw text
    const QString text(controller->text());
    if (!text.isEmpty() && controller->isTextVisible()) {
        QFont font = style()->font();
        painter->setFont(font);
        painter->setPen(style()->textColor());
        painter->setOpacity(style()->textOpacity());
        painter->drawText(titleRect(), Qt::AlignCenter, text);
    }

    // Draw a close image at top right corner
    const QPixmap *closeImage = style()->closeImage();
    if (closeImage != NULL) {
        painter->setOpacity(1.0f);
        QPointF p = buttonRect().topRight() - QPointF(closeImage->width() * 0.5f,
                    closeImage->height() * 0.5f);
        painter->drawPixmap(p, *closeImage);
    }

    // Restore the painter state
    painter->restore();
}

QRectF SwitcherButtonView::buttonRect() const
{
    QRectF rect(QPointF(), size());

    return rect;
}

QRectF SwitcherButtonView::titleRect() const
{
    QRectF rect(style()->textPosition(), style()->textSize());

    rect.translate(buttonRect().topLeft());

    return rect;
}

QRectF SwitcherButtonView::closeRect() const
{
    QRectF rect = buttonRect();

    const QPixmap *closeImage = style()->closeImage();
    if (closeImage != NULL) {
        // calculate the rect for the close button alone
        rect.setTopRight(rect.topRight() + QPointF(closeImage->width() * 0.5f, -closeImage->height() * 0.5f));
        rect.setBottomLeft(rect.topRight() - QPointF(closeImage->width(), -closeImage->height()));
    } else {
        // HACK: specify a fake 1x1 rectagle at the top-right corner of the button
        rect.setBottomLeft(rect.topRight() - QPointF(1, -1));
    }

    return rect;
}

QRectF SwitcherButtonView::boundingRect() const
{
    return buttonRect().united(closeRect());
}

void SwitcherButtonView::applyStyle()
{
    DuiWidgetView::applyStyle();

    updateThumbnail();
}

void SwitcherButtonView::setupModel()
{
    DuiWidgetView::setupModel();

    if (model()->xWindow() != 0) {
        updateXWindowPixmap();
        updateThumbnail();
    }
}

void SwitcherButtonView::updateData(const QList<const char *>& modifications)
{
    DuiWidgetView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == SwitcherButtonModel::XWindow && model()->xWindow() != 0) {
            updateXWindowPixmap();
            updateThumbnail();
        }
    }
}

void SwitcherButtonView::resetState()
{
    controller->setVisible(true);
    controller->prepareGeometryChange();
}

void SwitcherButtonView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (!model()->down()) {
        model()->setDown(true);

        if (closeRect().contains(event->pos())) {
            model()->setPressed(SwitcherButtonModel::ClosePressed);
        } else if (buttonRect().contains(event->pos())) {
            model()->setPressed(SwitcherButtonModel::ButtonPressed);
        } else {
            model()->setPressed(SwitcherButtonModel::NonePressed);
        }

        event->accept();
    }
}

void SwitcherButtonView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (model()->down()) {
        model()->setDown(false);

        if (closeRect().contains(event->pos())) {
            if (model()->pressed() == SwitcherButtonModel::ClosePressed) {
                // Close icon clicked, send the close signal and hide
                // the button
                controller->close();
                controller->setVisible(false);
                windowCloseTimer.start(5000);
            }
        } else if (buttonRect().contains(event->pos())) {
            if (model()->pressed() == SwitcherButtonModel::ButtonPressed) {
                // Switcher button clicked, let the controller know that
                // the window should be brought to front
                model()->click();
                controller->switchToWindow();
                update();
            }
        }

        model()->setPressed(SwitcherButtonModel::NonePressed);
        event->accept();
    }
}

void SwitcherButtonView::cancelEvent(DuiCancelEvent *event)
{
    if (model()->down()) {
        model()->setDown(false);
        model()->setPressed(SwitcherButtonModel::NonePressed);

        event->accept();
    }
}

void SwitcherButtonView::updateXWindowPixmap()
{
#ifdef Q_WS_X11
    if (xWindowPixmapDamage != 0) {
        // Unregister the old pixmap from XDamage events
        X11Wrapper::XDamageDestroy(QX11Info::display(), xWindowPixmapDamage);
        xWindowPixmapDamage = 0;
    }

    if (xWindowPixmap != 0) {
        // Dereference the old pixmap ID
        X11Wrapper::XFreePixmap(QX11Info::display(), xWindowPixmap);
        xWindowPixmap = 0;
    }

    // It is possible that the window is not redirected so check for errors
    XErrorHandler errh = X11Wrapper::XSetErrorHandler(handleXError);

    // Get the pixmap ID of the X window
    xWindowPixmap = X11Wrapper::XCompositeNameWindowPixmap(QX11Info::display(), model()->xWindow());
    X11Wrapper::XSync(QX11Info::display(), FALSE);

    // If a BadMatch error occurred set the window pixmap ID to 0
    if (badMatchOccurred) {
        xWindowPixmap = 0;
        badMatchOccurred = false;
    }

    // Reset the error handler
    X11Wrapper::XSetErrorHandler(errh);

    if (xWindowPixmap != 0) {
        // Register the pixmap for XDamage events
        xWindowPixmapDamage = X11Wrapper::XDamageCreate(QX11Info::display(), xWindowPixmap, XDamageReportNonEmpty);

        backendSpecificUpdateXWindowPixmap();
    }
#endif
}

void SwitcherButtonView::backendSpecificUpdateXWindowPixmap()
{
}

void SwitcherButtonView::updateThumbnail()
{
}

#ifdef Q_WS_X11
int SwitcherButtonView::handleXError(Display *, XErrorEvent *event)
{
    if (event->error_code == BadMatch) {
        badMatchOccurred = true;
    }
    return 0;
}
#endif

void SwitcherButtonView::windowVisibilityChanged(Window)
{
}

DUI_REGISTER_VIEW_NEW(SwitcherButtonView, SwitcherButton)
