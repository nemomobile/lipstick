/*
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
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
#include "xeventlistener.h"

class QFileSystemWatcher;

class SwitcherModel : public QObjectListModel<WindowInfo>, XEventListener
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

    // TODO: would be nice to make these some kind of QObject methods instead of
    // stuck onto the model randomly
    Q_INVOKABLE void closeWindow(qulonglong window);
    Q_INVOKABLE void windowToFront(qulonglong window);

signals:
    void itemCountChanged();

};

#endif // SWITCHERMODEL_H
