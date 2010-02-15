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
#ifndef SWITCHERBUTTONGLESVIEW_H
#define SWITCHERBUTTONGLESVIEW_H

#include <DuiGLRenderer>
#include "switcherbuttonview.h"

/*!
 * \class SwitcherButtonGLESView
 *
 * \brief SwitcherButtonGLESView implements a switcher style view with OpenGL ES rendering
 */
class SwitcherButtonGLESView : public SwitcherButtonView
{
    Q_OBJECT
    DUI_VIEW(SwitcherButtonModel, SwitcherButtonStyle)

public:
    /*!
     * \brief Constructs a new SwitcherButtonGLESView.
     *
     * \param controller The button's controller
     */
    SwitcherButtonGLESView(SwitcherButton *controller);

    /*!
     * \brief Destroys the SwitcherButtonGLESView.
     */
    virtual ~SwitcherButtonGLESView();

private slots:
    /*!
     * \brief A slot for receiving information about pixmap changes
     */
    void damageEvent(Qt::HANDLE &damage, short &x, short &y, unsigned short &width, unsigned short &height);

    //! \reimp
    virtual void windowVisibilityChanged(Window window);
    //! \reimp_end

private:
    //! \reimp
    virtual void updateThumbnail();
    virtual void backendSpecificDrawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    //! \reimp_end

    //! Window texture ID
    quint32 windowTextureID;

    //! Mask texture ID
    mutable quint32 maskTextureID;

    Q_DISABLE_COPY(SwitcherButtonGLESView);
};

/*!
 * Provides the uniforms required by SwitcherButtonGLESView.
 *
 * Currently this provides the following uniforms:
 * - texture1 (set to the integer value 1 to point to the texture unit 1)
 * - opacity (set to a float value between 0 and 1)
 */
class SwitcherButtonViewUniformProvider : public IDuiGLUniformProvider
{
public:
    /*!
     * \brief Constructs a new SwitcherButtonViewUniformProvider.
     */
    SwitcherButtonViewUniformProvider(float opacity);

    /*!
     * \brief Destroys the SwitcherButtonViewUniformProvider.
     */
    virtual ~SwitcherButtonViewUniformProvider();

    //! \reimp
    virtual bool setUniformValue(const QString &name, const DuiGLShaderUniform &uniform);
    //! \reimp_end

private:
    //! The current opacity
    float opacity;
};

#endif
