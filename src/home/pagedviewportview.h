/*
 * This file is part of mhome
 *
 * Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * Contact: Tomas Junnonen <tomas.junnonen@nokia.com>
 *
 * This software, including documentation, is protected by copyright
 * controlled by Nokia Corporation. All rights are reserved. Copying,
 * including reproducing, storing, adapting or translating, any or all of
 * this material requires the prior written consent of Nokia Corporation.
 * This material also contains confidential information which may not be
 * disclosed to others without the prior written consent of Nokia.
 */
#ifndef PAGEDVIEWPORTVIEW_H
#define PAGEDVIEWPORTVIEW_H

#include <mpannablewidgetview.h>
#include "pagedviewportstyle.h"
#include "pagedviewport.h"

class PagedViewport;

/*!
 * \class PagedViewportView
 * \brief PagedViewportView implements the view for PagedViewport
 */
class  PagedViewportView : public MPannableWidgetView
{
    Q_OBJECT
    M_VIEW(MPannableWidgetModel, PagedViewportStyle)

public:
    PagedViewportView(PagedViewport *controller);
    virtual ~PagedViewportView();

protected:
    virtual void applyStyle();

private:

    PagedViewport* controller;
};

#endif
