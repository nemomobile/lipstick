
#include "qobjectlistmodel.h"
#include <QDebug>



QObjectListModel::QObjectListModel(QObject *parent, QList<QObject*> *list)
    : QAbstractListModel(parent),
      _list(list)
{
    QHash<int, QByteArray> roles;
    roles[Qt::UserRole + 1] = "object";
    setRoleNames(roles);
}

int QObjectListModel::indexOf(QObject *obj) const
{
    return _list->indexOf((QObject*) obj);
}


int QObjectListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _list->count();
}


int QObjectListModel::itemCount() const
{
    return _list->count();
}


int QObjectListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}


QVariant QObjectListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= _list->count())
        return QVariant();

    if (role == Qt::UserRole + 1)
    {
        QObject *obj = _list->at(index.row());
        return QVariant::fromValue(obj);
    }

    return QVariant(0);
}


bool QObjectListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() < 0 || index.row() >= _list->count())
        return false;

    if (role == Qt::UserRole + 1)
    {
        _list->replace(index.row(), reinterpret_cast<QObject*>(value.toInt()));
        return true;
    }

    return false;
}


void QObjectListModel::addItem(QObject *item)
{
    int z = _list->count();
    beginInsertRows(QModelIndex(), z, z);
    _list->append(item);
    connect(item, SIGNAL(destroyed()), this, SLOT(removeDestroyedItem()));
    endInsertRows();

    emit itemAdded(item);
    emit itemCountChanged();
}


void QObjectListModel::removeDestroyedItem()
{
    QObject *obj = QObject::sender();
    removeItem(obj);
}


void QObjectListModel::removeItem(QObject *item)
{
    int z = _list->indexOf(item);
    beginRemoveRows(QModelIndex(), z, z);
    _list->removeAt(z);
    disconnect(item, SIGNAL(destroyed()), this, SLOT(removeDestroyedItem()));
    endRemoveRows();
    emit itemCountChanged();
}


void QObjectListModel::removeItem(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    disconnect(((QObject*)_list->at(index)), SIGNAL(destroyed()), this, SLOT(removeDestroyedItem()));
    _list->removeAt(index);
    endRemoveRows();
    emit itemCountChanged();
}


QObject* QObjectListModel::getItem(int index)
{
    if (index >= _list->count() || index < 0)
        return 0;

    return _list->at(index);
}

QList<QObject*> *QObjectListModel::getList()
{
    return _list;
}

void QObjectListModel::setList(QList<QObject *> *list)
{
    QList<QObject *> *oldList = _list;
    beginResetModel();
    _list = list;
    endResetModel();
    emit itemCountChanged();
    delete oldList;
}

void QObjectListModel::reset()
{
    QAbstractListModel::reset();
    emit itemCountChanged();
}

