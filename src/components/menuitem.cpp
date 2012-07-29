/*
 * Copyright 2011 Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 */

#include <QStringList>

#include "menuitem.h"
#include "menumodel.h"

MenuItem::MenuItem(Desktop *data)
{
    itemData = data;
}

MenuItem::~MenuItem()
{
}

void MenuItem::setCategoryName(QString name)
{
    if(!itemData)
        categoryName = name;
}

QString MenuItem::getCategoryName()
{
    return categoryName;
}

void MenuItem::setCategoryIcon(QString icon)
{
    if(!itemData)
        categoryIcon = icon;
}

QString MenuItem::getCategoryIcon()
{
    return categoryIcon;
}

void MenuItem::setCategoryGroups(QList<QString> groups)
{
    if(!itemData)
    {
        while(!categoryGroups.isEmpty())
            categoryGroups.removeFirst();
        foreach (QString item, groups)
            categoryGroups << item;
    }
}

QList<QString> MenuItem::getCategoryGroups()
{
    return categoryGroups;
}

bool MenuItem::containsGroup(QString group)
{
    return categoryGroups.contains(group);
}

