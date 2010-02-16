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

#include "switcherbuttonglesview.h"
#include "mainwindow.h"
#include <DuiApplication>
#include <DuiScalableImage>
#include <DuiSceneManager>

static const QString MASK_FRAGMENT_SHADER(SHADERS_DIR "/mask.frag");

#define WARN_IF(x) \
    { if(x) qWarning( "%s:%d: WARNING: %s", __FILE__, __LINE__, #x ); }

SwitcherButtonGLESView::SwitcherButtonGLESView(SwitcherButton *button) :
    SwitcherButtonView(button),
    windowTextureID(0)
{
    DuiGLES2Renderer* renderer = DuiGLES2Renderer::instance();
    WARN_IF(renderer == NULL);

    // Show interest in X pixmap change signals
    connect(qApp, SIGNAL(damageEvent(Qt::HANDLE &, short &, short &, unsigned short &, unsigned short &)), this, SLOT(damageEvent(Qt::HANDLE &, short &, short &, unsigned short &, unsigned short &)));
}

SwitcherButtonGLESView::~SwitcherButtonGLESView()
{
    if (windowTextureID != 0 && xWindowPixmap != 0) {
        DuiGLES2Renderer* renderer = DuiGLES2Renderer::instance();
        WARN_IF(renderer == NULL);

        if (renderer) {
            // Unbind the texture of the X window
            renderer->unbindX11Pixmap(xWindowPixmap);
        }
    }
}

void SwitcherButtonGLESView::backendSpecificDrawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);

    if (windowTextureID != 0) {

        DuiGLES2Renderer* renderer = DuiGLES2Renderer::instance();
        WARN_IF(renderer == NULL);

        if (renderer) {
            const QPixmap* maskPixmap = NULL;

            const DuiScalableImage *mask = style()->maskImage();
            if (mask != NULL) {
                maskPixmap = mask->pixmap();
            }

            if (maskPixmap != NULL) {
                QGLShaderProgram* program = renderer->getShaderProgram(MASK_FRAGMENT_SHADER);
                renderer->begin(painter, program);
                renderer->bindTexture(*maskPixmap, 1);
            } else {
                QGLShaderProgram* program = renderer->getShaderProgram();
                renderer->begin(painter, program);
            }

            renderer->bindTexture(windowTextureID);
            renderer->setInvertTexture(true);

            // Rotate the thumbnails and adjust their size if the screen
            // has been rotated

            DuiSceneManager *manager = MainWindow::instance()->sceneManager();
            QPoint pos = style()->iconPosition().toPoint();
            QSize size = style()->iconSize();

            if (manager->orientation() == Dui::Portrait) {
                size.transpose();
            }

            switch (manager->orientationAngle()) {
                case Dui::Angle90:
                    pos -= QPoint(size.width(), 0);
                    break;
                case Dui::Angle180:
                    pos -= QPoint(size.width(), size.height());
                    break;
                case Dui::Angle270:
                    pos -= QPoint(0, size.height());
                    break;
                default:
                    break;
            }

            painter->rotate(-manager->orientationAngle());

            QRect target(pos, size);
            renderer->draw(target);
            renderer->end();
        }
    }
}

void SwitcherButtonGLESView::updateThumbnail()
{
    if (xWindowPixmap != 0) {
        DuiGLES2Renderer* renderer = DuiGLES2Renderer::instance();
        WARN_IF(renderer == NULL);

        if (renderer) {
            windowTextureID = renderer->bindX11Pixmap(xWindowPixmap);
            if (windowTextureID != 0) {
                renderer->updateX11Pixmap(xWindowPixmap);
                update();
            }
        }
    }
}

void SwitcherButtonGLESView::damageEvent(Qt::HANDLE &damage, short &x, short &y, unsigned short &width, unsigned short &height)
{
    Q_UNUSED(x);
    Q_UNUSED(y);
    Q_UNUSED(width);
    Q_UNUSED(height);
    if (xWindowPixmapDamage == damage) {
        update();
    }
}

void SwitcherButtonGLESView::windowVisibilityChanged(Window window)
{
    if (window == model()->xWindow()) {
        // The compositing is enabled when minimizing which changes the window pixmap ID
        if (windowTextureID != 0) {
            // Unbind the texture of the X window
            DuiGLES2Renderer* renderer = DuiGLES2Renderer::instance();
            WARN_IF(renderer == NULL);

            if (renderer) {
                renderer->unbindX11Pixmap(xWindowPixmap);
                windowTextureID = 0;
            }
        }

        if (xWindowPixmap != 0) {
            // Dereference the old pixmap ID
            X11Wrapper::XFreePixmap(QX11Info::display(), xWindowPixmap);
        }

        // Get the current window pixmap ID
        xWindowPixmap = X11Wrapper::XCompositeNameWindowPixmap(QX11Info::display(), model()->xWindow());

        // Update the thumbnail
        updateThumbnail();
    }
}

DUI_REGISTER_VIEW_NEW(SwitcherButtonGLESView, SwitcherButton)
