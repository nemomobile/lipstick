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
#include <DuiGLShaderUniform>
#include <DuiSceneManager>

SwitcherButtonGLESView::SwitcherButtonGLESView(SwitcherButton *button) :
    SwitcherButtonView(button),
    windowTextureID(0),
    maskTextureID(0)
{
    if (DuiGLRenderer::instance()->getProgram("SwitcherButtonViewMaskedShader") == NULL) {
        // Create a fragment shader which multiplies two textures with each other for switcher button masking
        static const char *fragmentShader = "\n\
                                            varying lowp vec2 fragTexCoord;\n\
                                            uniform sampler2D texture0;\n\
                                            uniform sampler2D texture1;\n\
                                            uniform lowp float opacity;\n\
                                            void main(void)\n\
                                            {\n\
                                             gl_FragColor = texture2D(texture0, fragTexCoord) * texture2D(texture1, fragTexCoord) * opacity;\n\
                                            }\n";


        DuiGLRenderer::instance()->createShader("SwitcherButtonViewMaskedFragmentShader", fragmentShader, DuiGLRenderer::DuiShaderFragment);
        DuiGLRenderer::instance()->createProgram("SwitcherButtonViewMaskedShader", "DefaultVert", "SwitcherButtonViewMaskedFragmentShader");
    }

    if (DuiGLRenderer::instance()->getProgram("SwitcherButtonViewShader") == NULL) {
        // Create a fragment shader which uses only one texture
        static const char *fragmentShader = "\n\
                                            varying lowp vec2 fragTexCoord;\n\
                                            uniform sampler2D texture0;\n\
                                            uniform lowp float opacity;\n\
                                            void main(void)\n\
                                            {\n\
                                             gl_FragColor = texture2D(texture0, fragTexCoord) * opacity;\n\
                                            }\n";


        DuiGLRenderer::instance()->createShader("SwitcherButtonViewFragmentShader", fragmentShader, DuiGLRenderer::DuiShaderFragment);
        DuiGLRenderer::instance()->createProgram("SwitcherButtonViewShader", "DefaultVert", "SwitcherButtonViewFragmentShader");
    }

    // Show interest in X pixmap change signals
    connect(qApp, SIGNAL(damageEvent(Qt::HANDLE &, short &, short &, unsigned short &, unsigned short &)), this, SLOT(damageEvent(Qt::HANDLE &, short &, short &, unsigned short &, unsigned short &)));
}

SwitcherButtonGLESView::~SwitcherButtonGLESView()
{
    if (windowTextureID != 0 && xWindowPixmap != 0) {
        // Unbind the texture of the X window
        DuiGLRenderer::instance()->unbindX11Pixmap(xWindowPixmap);
    }

    if (maskTextureID != 0) {
        if (!DuiApplication::softwareRendering()) {
            // Get the OpenGL context
            QGLContext *context = MainWindow::glContext();

            if (context != NULL) {
                // Delete old mask texture if there is one
                context->deleteTexture(maskTextureID);
            }
        }
    }
}

void SwitcherButtonGLESView::backendSpecificDrawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_UNUSED(option);

    if (windowTextureID != 0) {
        if (!DuiApplication::softwareRendering()) {
            // Get the OpenGL context
            QGLContext *context = MainWindow::glContext();

            if (context != NULL) {
                quint32 oldMaskTextureID = maskTextureID;

                // Get the mask from the style
                const DuiScalableImage *mask = style()->maskImage();
                if (mask != NULL && mask->pixmap() != NULL) {
                    // Bind it into a texture
                    maskTextureID = context->bindTexture(*mask->pixmap());
                }

                if (oldMaskTextureID != maskTextureID && oldMaskTextureID != 0) {
                    // Delete old mask texture if there is one
                    context->deleteTexture(oldMaskTextureID);
                }
            }
        }

        painter->beginNativePainting();

        // Set thumbnail location, size and opacity. Zoom when dragging.
        qreal opacity = 1.0f;
        QTransform transform = painter->combinedTransform().translate(style()->iconPosition().x(), style()->iconPosition().y());
        QSizeF size = style()->iconSize();

        // Rotate the thumbnail
        transform = transform.rotate(-MainWindow::instance()->sceneManager()->orientationAngle());

        // If the screen has been rotated adjust the thumbnail size accordingly
        if (MainWindow::instance()->sceneManager()->orientation() == Dui::Portrait) {
            size.transpose();
        }

        switch (MainWindow::instance()->sceneManager()->orientationAngle()) {
        case Dui::Angle90:
            transform = transform.translate(-size.width(), 0);
            break;
        case Dui::Angle180:
            transform = transform.translate(-size.width(), -size.height());
            break;
        case Dui::Angle270:
            transform = transform.translate(0, -size.height());
            break;
        default:
            break;
        }

        // Set the uniforms provider
        SwitcherButtonViewUniformProvider uniforms(opacity);

        if (maskTextureID != 0) {
            // There's a mask for the thumbnail: Setup texturing for it in texturing unit 1
#ifdef DUI_USE_OPENGL
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, maskTextureID);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#endif

            // Draw window (texture 0) and mask (texture 1)
            DuiGLRenderer::instance()->drawTexture("SwitcherButtonViewMaskedShader", transform, windowTextureID, size, &uniforms, true);
        } else {
            // No mask: draw window only (texture 0)
            DuiGLRenderer::instance()->drawTexture("SwitcherButtonViewShader", transform, windowTextureID, size, &uniforms, true);
        }

        painter->endNativePainting();
    }
}

void SwitcherButtonGLESView::updateThumbnail()
{
    if (xWindowPixmap != 0) {
        windowTextureID = DuiGLRenderer::instance()->bindX11Pixmap(xWindowPixmap);
        if (windowTextureID != 0) {
            DuiGLRenderer::instance()->updateX11Pixmap(xWindowPixmap);
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
            DuiGLRenderer::instance()->unbindX11Pixmap(xWindowPixmap);
            windowTextureID = 0;
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

SwitcherButtonViewUniformProvider::SwitcherButtonViewUniformProvider(float opacity) :
    opacity(opacity)
{
}

SwitcherButtonViewUniformProvider::~SwitcherButtonViewUniformProvider()
{
}

bool SwitcherButtonViewUniformProvider::setUniformValue(const QString &name, const DuiGLShaderUniform &uniform)
{
    if (name == "texture1") {
        // Set the texture1 uniform to 1 (refers to texture unit 1)
        uniform = (quint32)1;
        return true;
    } else if (name == "opacity") {
        // Set the opacity uniform to the given opacity
        uniform = opacity;
        return true;
    }

    return false;
}

DUI_REGISTER_VIEW_NEW(SwitcherButtonGLESView, SwitcherButton)
