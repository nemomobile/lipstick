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
#ifndef LAUNCHERBUTTONVIEW_H
#define LAUNCHERBUTTONVIEW_H

#include <DuiButtonIconView>
#include <QBitmap>
#include "launcherbuttonmodel.h"
#include "launcherbuttonstyle.h"

class LauncherButton;

/*!
 * \class LauncherButtonView
 *
 * \brief LauncherButtonView implements a launcher style view
 */
class LauncherButtonView : public DuiButtonIconView
{
    Q_OBJECT
    DUI_VIEW(LauncherButtonModel, LauncherButtonStyle)

public:
    /*!
     * \brief Constructs a new LauncherButtonView.
     *
     * \param controller The button's controller
     */
    LauncherButtonView(LauncherButton *controller);

    /*!
     * \brief Destroys the LauncherButtonView.
     */
    virtual ~LauncherButtonView();

protected:
    //! \reimp
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void applyStyle();
    virtual void setupModel();
    //! \reimp_end

private slots:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    /*!
     * \brief Updates the thumbnail pixmap by scaling it from the thumbnail stored in the model.
     */
    void updateThumbnail();

    /*!
     * Draws the background of category buttons.
     *
     * \param painter the QPainter to paint to
     */
    void drawCategoryBackground(QPainter *painter) const;

    /*!
     * Draws the contents of shortcut buttons.
     *
     * \param painter the QPainter to paint to
     */
    void drawShortcutContents(QPainter *painter) const;

    /*!
     * Draws the text of the button.
     *
     * \param painter the QPainter to paint to
     * \param pos the position to paint to
     * \param size the size to paint into
     */
    void drawText(QPainter *painter, const QPointF &pos, const QSize &size) const;

    /*!
     * Draws the container of shortcut buttons.
     *
     * \param painter the QPainter to paint to
     */
    void drawShortcutContainer(QPainter *painter) const;

    //! LauncherButton controller
    LauncherButton *controller;

    //! Thumbnail
    mutable QPixmap thumbnailPixmap;

    //! Thumbnail mask
    mutable QBitmap thumbnailMask;

    //! Whether the mask has been applied to the pixmap
    mutable bool thumbnailMaskApplied;

    Q_DISABLE_COPY(LauncherButtonView);
};

#endif
