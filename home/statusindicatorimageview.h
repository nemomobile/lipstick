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

#ifndef STATUSINDICATORIMAGEVIEW_H
#define STATUSINDICATORIMAGEVIEW_H

#include "statusindicator.h"
#include "statusindicatormodel.h"
#include "statusindicatorimagestyle.h"
#include <DuiWidgetView>

class StatusIndicator;
class QPainter;
class QTimeLine;

/*!
 * \class StatusIndicatorImageView
 * \brief a view class for the status indicator widgets.
*/
class StatusIndicatorImageView : public DuiWidgetView
{
    Q_OBJECT
    DUI_VIEW(StatusIndicatorModel, StatusIndicatorImageStyle)

public:
    /*!
     * \brief Constructs a new StatusIndicatorImageView.
     *
     * \param controller The view's controller
     */
    StatusIndicatorImageView(StatusIndicator *controller);

    /*!
     * \brief Destroys the StatusIndicatorImageView.
     */
    virtual ~StatusIndicatorImageView();

protected:
    //! \reimp
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void applyStyle();
    //! \reimp_end

public slots:
    /*!
     * \brief A slot for starting the animation
     */
    virtual void startAnimation();

    /*!
     * \brief A slot for stopping the animation
     */
    virtual void stopAnimation();

    /*!
     * \brief A slot for setting the animation frame
     *
     * \param frame the animation frame to show
     */
    virtual void setAnimationFrame(int frame);

protected slots:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

protected:
    //! \reimp
    virtual void setupModel();
    //! \reimp_end

private:
    /*!
     * Clears the images list
     */
    void clearImageList();

    //! The controller for this view
    StatusIndicator *controller;

    //! The list of names of the images to be shown
    QStringList imageList;

    //! A vector of images to be shown
    mutable QVector<const QPixmap *> images;

    //! The current animation frame
    int animationFrame;

    //! Timeline for the animation
    QTimeLine *animationTimeline;
};

#endif
