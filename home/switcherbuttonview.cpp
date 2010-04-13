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
#include <QTimer>
#include <QGraphicsSceneMouseEvent>
#include "mainwindow.h"
#include <QApplication>
#include <QX11Info>
#include <QPainter>
#include <MScalableImage>
#include <MCancelEvent>
#include <MSceneManager>
#include "switcherbuttonview.h"
#include "switcherbutton.h"

#ifdef Q_WS_X11
bool SwitcherButtonView::badMatchOccurred = false;
#endif

SwitcherButtonView::SwitcherButtonView(SwitcherButton *button) :
    MButtonView(button),
    controller(button),
    xWindowPixmap(0),
    xWindowPixmapDamage(0),
    onDisplay(false)
{
    // Connect to the windowVisibilityChanged signal of the HomeApplication to get information about window visiblity changes
    connect(qApp, SIGNAL(windowVisibilityChanged(Window)), this, SLOT(windowVisibilityChanged(Window)));

    // Show interest in X pixmap change signals
    connect(qApp, SIGNAL(damageEvent(Qt::HANDLE &, short &, short &, unsigned short &, unsigned short &)), this, SLOT(damageEvent(Qt::HANDLE &, short &, short &, unsigned short &, unsigned short &)));

    // Create a close button for the window
    closeButton = new MButton(controller);
    closeButton->setViewType(MButton::iconType);
    connect(closeButton, SIGNAL(clicked()), controller, SLOT(close()));

    // Enable or disable reception of damage events based on whether the switcher button is on the screen or not
    connect(button, SIGNAL(displayEntered()), this, SLOT(setOnDisplay()));
    connect(button, SIGNAL(displayExited()), this, SLOT(unsetOnDisplay()));
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

void SwitcherButtonView::setGeometry(const QRectF &rect)
{
    MButtonView::setGeometry(rect);

    closeButton->setGeometry(closeRect());
}

void SwitcherButtonView::drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    // Store the painter state
    painter->save();

    // Rotate the thumbnails and adjust their size if the screen
    // has been rotated
    MSceneManager *manager = MainWindow::instance()->sceneManager();
    QPoint pos = style()->iconPosition().toPoint();
    QSize size = style()->iconSize();

    if (manager->orientation() == M::Portrait) {
        size.transpose();
    }

    switch (manager->orientationAngle()) {
        case M::Angle90:
            pos -= QPoint(size.width(), 0);
            break;
        case M::Angle180:
            pos -= QPoint(size.width(), size.height());
            break;
        case M::Angle270:
            pos -= QPoint(0, size.height());
            break;
        default:
            break;
    }

    painter->rotate(-manager->orientationAngle());

    QRect target(pos, size);

    // Do the actual drawing
    backendSpecificDrawBackground(painter, option, target);

    // Restore the painter state
    painter->restore();
}

void SwitcherButtonView::backendSpecificDrawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option, const QRect& target) const
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(target);
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

    // Draw text
    const QString text(controller->text());
    if (!text.isEmpty() && controller->isTextVisible()) {
        QFont font = style()->font();
        painter->setFont(font);
        painter->setPen(style()->textColor());
        painter->setOpacity(style()->textOpacity());
        painter->drawText(titleRect(), Qt::AlignLeft | Qt::ElideRight, text);
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
    QRectF rect = buttonRect();
    QRectF close = closeRect();
    QFontMetrics fm(style()->font());

    rect.setTopLeft(rect.topLeft() - QPointF(0, fm.height()));
    rect.setBottomRight(rect.topRight() + QPointF(-(close.width() + 2 * style()->textMarginLeft()), fm.height()));
    rect.translate(style()->textMarginLeft(), -style()->textMarginBottom());
    return rect;
}

QRectF SwitcherButtonView::closeRect() const
{
    QRectF switchButtonRect = buttonRect();
    QRectF rect;

    rect.setX(switchButtonRect.right() - closeButton->preferredWidth());
    rect.setY(switchButtonRect.top() + style()->closeButtonVerticalPosition());
    rect.setSize(closeButton->preferredSize());

    return rect;
}

QRectF SwitcherButtonView::boundingRect() const
{
    return buttonRect().united(closeRect());
}

void SwitcherButtonView::applyStyle()
{
    updateThumbnail();

    // Apply style to close button
    if (controller->objectName() == "DetailviewButton") {
        closeButton->setObjectName("CloseButtonDetailview");
        if (model()->viewMode() == SwitcherButtonModel::Large) {
            closeButton->show();
        } else {
            closeButton->hide();
        }
    } else {
        closeButton->setObjectName("CloseButtonOverview");
        closeButton->show();
    }
    closeButton->setIconID(style()->closeIcon());

    MWidgetView::applyStyle();
}

void SwitcherButtonView::setupModel()
{
    MWidgetView::setupModel();

    if (model()->xWindow() != 0) {
        updateXWindowPixmap();
        updateThumbnail();
    }
    updateViewMode();
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

    // When the style mode changes, the style is not automatically applied -> call it explicitly
    applyStyle();
}

void SwitcherButtonView::updateData(const QList<const char *>& modifications)
{
    MWidgetView::updateData(modifications);
    const char *member;
    foreach(member, modifications) {
        if (member == SwitcherButtonModel::XWindow && model()->xWindow() != 0) {
            updateXWindowPixmap();
            updateThumbnail();
        }

        if (member == SwitcherButtonModel::ViewMode) {
            updateViewMode();
        }
    }
}

void SwitcherButtonView::updateXWindowPixmap()
{
#ifdef Q_WS_X11
    // Unregister the old pixmap from XDamage events
    destroyDamage();

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

    // Register the pixmap for XDamage events
    createDamage();
#endif
}

void SwitcherButtonView::backendSpecificUpdateXWindowPixmap()
{
}

void SwitcherButtonView::updateThumbnail()
{
    // Redraw
    update();
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

void SwitcherButtonView::windowVisibilityChanged(Window window)
{
    if (window == model()->xWindow()) {
        updateXWindowPixmap();
        updateThumbnail();
    }
}

void SwitcherButtonView::damageEvent(Qt::HANDLE &damage, short &x, short &y, unsigned short &width, unsigned short &height)
{
    Q_UNUSED(x);
    Q_UNUSED(y);
    Q_UNUSED(width);
    Q_UNUSED(height);
    if (xWindowPixmapDamage == damage) {
        update();
    }
}

void SwitcherButtonView::setOnDisplay()
{
    onDisplay = true;
    createDamage();
    update();
}

void SwitcherButtonView::unsetOnDisplay()
{
    onDisplay = false;
    destroyDamage();
}

void SwitcherButtonView::createDamage()
{
#ifdef Q_WS_X11
    if (onDisplay && xWindowPixmap != 0) {
        // Register the pixmap for XDamage events
        xWindowPixmapDamage = X11Wrapper::XDamageCreate(QX11Info::display(), model()->xWindow(), XDamageReportNonEmpty);
        backendSpecificUpdateXWindowPixmap();
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

M_REGISTER_VIEW_NEW(SwitcherButtonView, SwitcherButton)
