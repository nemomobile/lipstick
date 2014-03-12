/***************************************************************************
**
** Copyright (C) 2014 Jolla Ltd.
** Contact: Thomas Perl <thomas.perl@jolla.com>
**
** This file is part of lipstick.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#ifndef UT_LAUNCHERMODEL_H
#define UT_LAUNCHERMODEL_H

#include <QObject>

class LauncherModel;

class Ut_LauncherModel : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void testUpdating();
    void testUpdatingFileAppears();

private:
    LauncherModel *launcherModel;
};

#endif /* UT_LAUNCHERMODEL_H */
