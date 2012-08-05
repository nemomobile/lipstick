
/*
 * Copyright (c) 2011-2012, Robin Burchell
 * Copyright (c) 2012, Timur Kristóf <venemo@fedoraproject.org>
 * Copyright (c) 2011, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef SWITCHERMODEL_H
#define SWITCHERMODEL_H

#include <QObject>
#include <QtDeclarative>
#include <QAbstractItemModel>
#include <QHash>

#include "utilities/qobjectlistmodel.h"
#include "windowinfo.h"
#include "launcheritem.h"
#include "xtools/xeventlistener.h"
#include "lipstickglobal.h"

class QFileSystemWatcher;

class LIPSTICK_EXPORT SwitcherModel : public QObjectListModel, XEventListener
{
    Q_OBJECT
    Q_DISABLE_COPY(SwitcherModel)

    Q_PROPERTY(int itemCount READ itemCount NOTIFY itemCountChanged)

    QList<Window> windowsBeingClosed;
    QList<Window> windowsStillBeingClosed;

public:
    explicit SwitcherModel(QObject *parent = 0);
    virtual ~SwitcherModel();

    virtual bool handleXEvent(const XEvent &event);
    void updateWindowList();
    void updateApps(const QList<WindowInfo> &windowList);

signals:
    void itemCountChanged();

};

#endif // SWITCHERMODEL_H
