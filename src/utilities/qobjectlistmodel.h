
// Copyright (C) 2011-2012, Timur Kristóf <venemo@fedoraproject.org>

#ifndef QOBJECTLISTMODEL_H
#define QOBJECTLISTMODEL_H

#include <QAbstractListModel>
#include <QMetaProperty>

class QObjectListModelMagic : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int itemCount READ itemCount NOTIFY itemCountChanged)

public:
    QObjectListModelMagic(QObject *parent) : QAbstractListModel(parent) { }
    Q_INVOKABLE virtual QObject *getItem(int index) = 0;
    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const = 0;
    Q_INVOKABLE virtual int indexOf(QObject *obj) const = 0;
    Q_INVOKABLE void reset() { QAbstractListModel::reset(); emit itemCountChanged(); }
    virtual int itemCount() const = 0;

protected slots:
    virtual void removeDestroyedItem() = 0;

signals:
    void itemAdded(QObject *item);
    void itemCountChanged();

};

template<typename X>
class QObjectListModel : public QObjectListModelMagic
{
    QHash<int, QByteArray> _roles;
    QList<X*> *_list;

public:
    explicit QObjectListModel(QObject *parent = 0, QList<X*> *list = new QList<X*>());
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int itemCount() const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    void addItem(X *item);
    void removeItem(X *item);
    void removeItem(int index);
    QObject* getItem(int index);
    int indexOf(QObject *obj) const;
    void removeDestroyedItem();

    QList<X*> &getList();
    void setList(QList<X*> *list);
};

template<typename T>
QObjectListModel<T>::QObjectListModel(QObject *parent, QList<T*> *list)
    : QObjectListModelMagic(parent),
      _list(list)
{
    QMetaObject meta = T::staticMetaObject;
    for (int i = 0; i < meta.propertyCount(); i++)
    {
        QMetaProperty prop = meta.property(i);
        _roles[Qt::UserRole + i + 1] = QByteArray(prop.name());
    }
    setRoleNames(_roles);
}

template<typename T>
QList<T*> &QObjectListModel<T>::getList()
{
    return *_list;
}

template<typename T>
void QObjectListModel<T>::setList(QList<T*> *list)
{
    beginResetModel();
    _list = list;
    endResetModel();
    emit itemCountChanged();
}

template<typename T>
int QObjectListModel<T>::indexOf(QObject *obj) const
{
    return _list->indexOf((T*) obj);
}

template<typename T>
int QObjectListModel<T>::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _list->count();
}

template<typename T>
int QObjectListModel<T>::itemCount() const
{
    return _list->count();
}

template<typename T>
int QObjectListModel<T>::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _roles.count();
}

template<typename T>
QVariant QObjectListModel<T>::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= _list->count())
        return QVariant();

    const QObject *obj = _list->at(index.row());
    return obj->property(_roles[role].data());
}

template<typename T>
bool QObjectListModel<T>::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() < 0 || index.row() >= _list->count())
        return false;

    QObject *obj = _list->at(index.row());
    return obj->setProperty(_roles[role].data(), value);
}

template<typename T>
void QObjectListModel<T>::addItem(T *item)
{
    int z = _list->count();
    beginInsertRows(QModelIndex(), z, z);
    _list->append(item);
    connect(item, SIGNAL(destroyed()), this, SLOT(removeDestroyedItem()));
    endInsertRows();

    emit itemAdded(item);
    emit itemCountChanged();
}

template<typename T>
void QObjectListModel<T>::removeDestroyedItem()
{
    T *obj = (T*) QObject::sender();
    removeItem(obj);
}

template<typename T>
void QObjectListModel<T>::removeItem(T *item)
{
    int z = _list->indexOf(item);
    beginRemoveRows(QModelIndex(), z, z);
    _list->removeAt(z);
    disconnect(item, SIGNAL(destroyed()), this, SLOT(removeDestroyedItem()));
    endRemoveRows();
    emit itemCountChanged();
}

template<typename T>
void QObjectListModel<T>::removeItem(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    disconnect(_list[index], SIGNAL(destroyed()), this, SLOT(removeDestroyedItem()));
    _list->removeAt(index);
    endRemoveRows();
    emit itemCountChanged();
}

template<typename T>
QObject* QObjectListModel<T>::getItem(int index)
{
    if (index >= _list->count() || index < 0)
        return 0;

    return _list->at(index);
}

#endif // QOBJECTLISTMODEL_H
