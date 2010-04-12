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
#ifndef SWITCHERBUTTONSWVIEW_H
#define SWITCHERBUTTONSWVIEW_H

#include "switcherbuttonview.h"
#include <QBitmap>

/*!
 * \class SwitcherButtonSWView
 *
 * \brief SwitcherButtonSWView implements a switcher style view in software rendering mode
 */
class SwitcherButtonSWView : public SwitcherButtonView
{
    Q_OBJECT
    M_VIEW(SwitcherButtonModel, SwitcherButtonStyle)

public:
    /*!
     * \brief Constructs a new SwitcherButtonSWView.
     *
     * \param controller The button's controller
     */
    SwitcherButtonSWView(SwitcherButton *controller);

    /*!
     * \brief Destroys the SwitcherButtonSWView.
     */
    virtual ~SwitcherButtonSWView();

private:
    //! \reimp
    virtual void backendSpecificDrawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option, const QRect& target) const;
    virtual void backendSpecificUpdateXWindowPixmap();
    //! \reimp_end

    //! The X window's pixmap as a shared QPixmap
    QPixmap qWindowPixmap;

    Q_DISABLE_COPY(SwitcherButtonSWView);
};

#endif
