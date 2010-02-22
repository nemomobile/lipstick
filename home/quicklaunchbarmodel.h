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

#ifndef QUICKLAUNCHBARMODEL_H_
#define QUICKLAUNCHBARMODEL_H_

#include <DuiWidgetModel>

typedef QList<DuiWidget *> WidgetList;

/*!
 * A model for the QuickLaunchBar widget.
 */
class QuickLaunchBarModel : public DuiWidgetModel
{
    Q_OBJECT
    DUI_MODEL(QuickLaunchBarModel)

    //! The list of widgets to show in the quick launch bar
    DUI_MODEL_PROPERTY(WidgetList, widgets, Widgets, true, QList<DuiWidget *>())
};

#endif /* QUICKLAUNCHBARMODEL_H_ */
