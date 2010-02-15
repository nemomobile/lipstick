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

#ifndef LAUNCHERMODEL_H_
#define LAUNCHERMODEL_H_

#include <duiwidgetmodel.h>

class DuiWidget;

typedef QList<DuiWidget *> WidgetList;

class LauncherModel : public DuiWidgetModel
{
    Q_OBJECT
    DUI_MODEL(LauncherModel)

public:
    enum CategoryType {
        RootCategory,
        SubCategory
    };

private:
    //! The list of widgets to show in the launcher
    DUI_MODEL_PROPERTY(WidgetList, widgets, Widgets, true, QList<DuiWidget *>())

    //! Type of the current category
    DUI_MODEL_PROPERTY(LauncherModel::CategoryType, category, Category, true, RootCategory)

    //! Name of the layout object
    DUI_MODEL_PROPERTY(QString, layoutObjectName, LayoutObjectName, true, QString())
};

#endif /* LAUNCHERMODEL_H_ */
