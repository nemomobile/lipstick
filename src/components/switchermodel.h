/*
 * Copyright 2011 Intel Corporation.
 *
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
#include "windowinfo.h"
#include "launcheritem.h"
#include "xeventlistener.h"

class QFileSystemWatcher;

class SwitcherModel : public QAbstractItemModel, XEventListener
{
    Q_OBJECT
    Q_PROPERTY(int itemCount READ itemCount NOTIFY itemCountChanged)

public:
    explicit SwitcherModel(QObject *parent = 0);
    ~SwitcherModel();
    int itemCount();

    enum Role
    {
        id = Qt::UserRole + 1,
        name,
        exec,
        comment,
        icon,
        filename,
        nodisplay,
        object,
        windowId
    };

    virtual bool handleXEvent(const XEvent &event);
    void updateWindowList();
    void updateApps(const QList<WindowInfo> &windowList);

    ///overrides from QAbstractModel:
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role) const;
    bool hasChildren ( const QModelIndex & parent = QModelIndex() ) const;

    // TODO: would be nice to make these some kind of QObject methods instead of
    // stuck onto the model randomly
    Q_INVOKABLE void closeWindow(qulonglong window);
    Q_INVOKABLE void windowToFront(qulonglong window);

signals:
    void itemCountChanged();

private:
    QList<Window> windowsBeingClosed;
    QList<WindowInfo *> m_windows;
    QList<Window> windowsStillBeingClosed;

    Q_DISABLE_COPY(SwitcherModel)
};

#endif // SWITCHERMODEL_H
