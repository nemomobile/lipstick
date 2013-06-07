/***************************************************************************
**
** Copyright (C) 2013 Jolla Ltd.
** Contact: Aaron Kennedy <aaron.kennedy@jollamobile.com>
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

#include "windowmodel.h"

#include "lipstickcompositor.h"

WindowModel::WindowModel()
: m_complete(false)
{
    LipstickCompositor *c = LipstickCompositor::instance();
    if (!c) {
        qWarning("WindowModel: Compositor must be created before WindowModel");
    } else {
        c->m_windowModels.append(this);
    }
}

WindowModel::~WindowModel()
{
    LipstickCompositor *c = LipstickCompositor::instance();
    if (c) c->m_windowModels.removeAll(this);
}

int WindowModel::itemCount() const
{
    return m_items.count();
}

int WindowModel::windowId(int index) const
{
    if (index < 0 || index >= m_items.count())
        return 0;
    return m_items.at(index);
}

int WindowModel::rowCount(const QModelIndex &) const
{
    return m_items.count();
}

QVariant WindowModel::data(const QModelIndex &index, int role) const
{
    int idx = index.row();
    if (idx < 0 || idx >= m_items.count())
        return QVariant();

    LipstickCompositor *c = LipstickCompositor::instance();
    if (role == Qt::UserRole + 1) {
        return m_items.at(idx);
    } else if (role == Qt::UserRole + 2) {
        QWaylandSurface *s = c->surfaceForId(m_items.at(idx));
        return s?s->processId():0;
    } else if (role == Qt::UserRole + 3) {
        LipstickCompositorWindow *w = static_cast<LipstickCompositorWindow *>(c->windowForId(m_items.at(idx)));
        return w->title();
    } else {
        return QVariant();
    }
}

QHash<int, QByteArray> WindowModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Qt::UserRole + 1] = "window";
    roles[Qt::UserRole + 2] = "processId";
    roles[Qt::UserRole + 3] = "title";
    return roles;
}

void WindowModel::classBegin()
{
}

void WindowModel::componentComplete()
{
    m_complete = true;
    refresh();
}

/*!
    Reimplement this method to provide custom filtering.
*/
bool WindowModel::approveWindow(LipstickCompositorWindow *window)
{
    return window->isInProcess() == false;
}

void WindowModel::addItem(int id)
{
    if (!m_complete)
        return;

    LipstickCompositor *c = LipstickCompositor::instance();
    LipstickCompositorWindow *window = static_cast<LipstickCompositorWindow *>(c->windowForId(id));
    if (!approveWindow(window))
        return;

    beginInsertRows(QModelIndex(), m_items.count(), m_items.count());
    m_items.append(id);
    endInsertRows();
    emit itemAdded(m_items.count() - 1);
    emit itemCountChanged();
}

void WindowModel::remItem(int id)
{
    if (!m_complete)
        return;

    int idx = m_items.indexOf(id);
    if (idx == -1)
        return;

    beginRemoveRows(QModelIndex(), idx, idx);
    m_items.removeAt(idx);
    endRemoveRows();
    emit itemCountChanged();
}

void WindowModel::titleChanged(int id)
{
    if (!m_complete)
        return;

    int idx = m_items.indexOf(id);
    if (idx == -1)
        return;

    emit dataChanged(index(idx, 0), index(idx, 0));
}

void WindowModel::refresh()
{
    LipstickCompositor *c = LipstickCompositor::instance();
    if (!m_complete || !c)
        return;

    beginResetModel();

    m_items.clear();

    for (QHash<int, LipstickCompositorWindow *>::ConstIterator iter = c->m_mappedSurfaces.begin();
         iter != c->m_mappedSurfaces.end(); ++iter) {

        if (approveWindow(iter.value()))
            m_items.append(iter.key());
    }

    endResetModel();
}
