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

#include "pagepositionindicator.h"

#include <MWidgetCreator>
M_REGISTER_WIDGET(PagePositionIndicator)

PagePositionIndicator::PagePositionIndicator(QGraphicsItem *parent)
        : MPositionIndicator(parent)
{
    setModel(new PagePositionIndicatorModel);
}

void PagePositionIndicator::setPageCount(int pageCount)
{
    model()->setPageCount(pageCount);
}

void PagePositionIndicator::setFocusedPage(int focusedPage)
{
    model()->setFocusedPage(focusedPage);
}
