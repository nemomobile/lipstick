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

void SwitcherButtonGLESView::backendSpecificDrawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option, const QRect& target) const
{
    Q_UNUSED(option);

    if (windowTextureID != 0) {

        DuiGLES2Renderer* renderer = DuiGLES2Renderer::instance();
        WARN_IF(renderer == NULL);

        if (renderer) {
            renderer->updateX11Pixmap(xWindowPixmap);

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

            renderer->draw(target);
            renderer->end();
        }
    }
}

void SwitcherButtonGLESView::updateXWindowPixmap()
{
    if (windowTextureID != 0) {
        // Unbind the texture of the X window
        DuiGLES2Renderer* renderer = DuiGLES2Renderer::instance();
        WARN_IF(renderer == NULL);

        if (renderer) {
            renderer->unbindX11Pixmap(xWindowPixmap);
            windowTextureID = 0;
        }
    }

    SwitcherButtonView::updateXWindowPixmap();
}

void SwitcherButtonGLESView::backendSpecificUpdateXWindowPixmap()
{
    if (xWindowPixmap != 0) {
        DuiGLES2Renderer* renderer = DuiGLES2Renderer::instance();
        WARN_IF(renderer == NULL);

        if (renderer) {
            windowTextureID = renderer->bindX11Pixmap(xWindowPixmap);
            if (windowTextureID != 0) {
                renderer->updateX11Pixmap(xWindowPixmap);
            }
        }
    }
}

DUI_REGISTER_VIEW_NEW(SwitcherButtonGLESView, SwitcherButton)
