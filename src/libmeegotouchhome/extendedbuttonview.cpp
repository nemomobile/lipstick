/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of meegotouchhome.
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

#include "extendedbuttonview.h"
#include <MButton>

ExtendedButtonView::ExtendedButtonView(MButton *controller) :
    MButtonIconView(controller)
{
}

ExtendedButtonView::~ExtendedButtonView()
{
}

QRectF ExtendedButtonView::boundingRect() const
{
    return MButtonIconView::boundingRect().adjusted(-style()->extendLeft(), -style()->extendTop(), style()->extendRight(), style()->extendBottom());
}

M_REGISTER_VIEW_NEW(ExtendedButtonView, MButton)
