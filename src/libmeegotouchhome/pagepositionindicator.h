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
#ifndef PAGEPOSITIONINDICATOR_H
#define PAGEPOSITIONINDICATOR_H

#include "mpositionindicator.h"
#include "pagepositionindicatormodel.h"

/*!
 * \class PagePositionIndicator
 * \brief Page position indicator draws a position indicator bar for paged viewport.
 */
class PagePositionIndicator : public MPositionIndicator
{
    Q_OBJECT

    M_CONTROLLER(PagePositionIndicator)

public:
    PagePositionIndicator(QGraphicsItem *parent = 0);

public slots:
    void setFocusedPage(int focusedPage);
    void setPageCount(int pageCount);
signals:
    //! Emitted when a page starts to pan and when page has stopped panning
    void pageIsPanning(bool);
};

#endif
