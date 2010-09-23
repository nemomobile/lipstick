/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libmeegotouch.
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

#ifndef MPOSITIONINDICATORVIEW_H
#define MPOSITIONINDICATORVIEW_H

#include "mwidgetview.h"
#include "pagepositionindicatormodel.h"
#include "pagepositionindicatorstyle.h"

class PagePositionIndicator;

/*!
  \class PagePositionIndicatorView
  \brief PagePositionIndicatorView implements a view for position indicator in paged viewport
*/
class PagePositionIndicatorView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(PagePositionIndicatorModel, PagePositionIndicatorStyle)

public:
    /*!
      \brief Constructor
    */
    PagePositionIndicatorView(PagePositionIndicator *controller);

    /*!
      \brief Destructor
    */
    virtual ~PagePositionIndicatorView();

protected:
    //! \reimp
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void updateData(const QList<const char *>& modifications);
    virtual void applyStyle();
    //! \reimp_end

private slots:
    /*!
     * Sets if unfocused icons should only be drawn when panning a page
     * \param force \c true if only unfocused icons should be drawn, \c false if both focused and unfocused icon needs to be drawn
     */
    void setForceUnfocusedIcon(bool force);

private:
    //! Page position indicator
    PagePositionIndicator *controller;
    //! Force drawing only an unfocused icon
    bool forceUnfocusedIcon;

#ifdef UNIT_TEST
    friend class Ut_PagePositionIndicatorView;
#endif
};

#endif
