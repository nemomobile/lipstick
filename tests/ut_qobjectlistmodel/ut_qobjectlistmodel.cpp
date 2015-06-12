/***************************************************************************
**
** Copyright (C) 2015 Jolla Ltd.
** Contact: Matt Vogt <matt.vogt@jollamobile.com>
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

#include "qobjectlistmodel.h"

#include <QtTest/QtTest>
#include <QObject>

class Ut_QObjectListModel : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void testPopulation();
    void testInsertion();
    void testRemoval();
    void testBatchRemoval();
    void testMove();
    void testUpdate();
    void testSynchronization();
};

void Ut_QObjectListModel::init()
{
}

void Ut_QObjectListModel::cleanup()
{
}

QObject *makeObject(const QString &name)
{
    QObject *rv(new QObject);
    rv->setProperty("name", name);
    return rv;
}

QString objectName(const QObject *object)
{
    return object->property("name").value<QString>();
}

void Ut_QObjectListModel::testPopulation()
{
    QList<QObject *> *objects = new QList<QObject *>;
    objects->append(makeObject("a"));
    objects->append(makeObject("b"));
    objects->append(makeObject("c"));
    objects->append(makeObject("d"));
    objects->append(makeObject("e"));

    QObjectListModel model(this, objects);

    QCOMPARE(model.itemCount(), 5);
    QCOMPARE(::objectName(model.get(0)), QString("a"));
    QCOMPARE(::objectName(model.get(1)), QString("b"));
    QCOMPARE(::objectName(model.get(2)), QString("c"));
    QCOMPARE(::objectName(model.get(3)), QString("d"));
    QCOMPARE(::objectName(model.get(4)), QString("e"));

    qDeleteAll(*objects);
    delete objects;
}

void Ut_QObjectListModel::testInsertion()
{
    QList<QObject *> *objects = new QList<QObject *>;
    objects->append(makeObject("a"));
    objects->append(makeObject("b"));
    objects->append(makeObject("c"));
    objects->append(makeObject("d"));
    objects->append(makeObject("e"));

    QObjectListModel model(this);

    QCOMPARE(model.itemCount(), 0);

    QSignalSpy addedSpy(&model, SIGNAL(itemAdded(QObject*)));
    QSignalSpy removedSpy(&model, SIGNAL(itemRemoved(QObject*)));
    QSignalSpy countSpy(&model, SIGNAL(itemCountChanged()));

    model.addItem(objects->at(0));
    model.addItems(QList<QObject *>() << objects->at(1) << objects->at(2));

    QCOMPARE(model.itemCount(), 3);
    QCOMPARE(::objectName(model.get(0)), QString("a"));
    QCOMPARE(::objectName(model.get(1)), QString("b"));
    QCOMPARE(::objectName(model.get(2)), QString("c"));

    QCOMPARE(addedSpy.count(), 3);
    QCOMPARE(addedSpy.at(0), QVariantList() << QVariant::fromValue(objects->at(0)));
    QCOMPARE(addedSpy.at(1), QVariantList() << QVariant::fromValue(objects->at(1)));
    QCOMPARE(addedSpy.at(2), QVariantList() << QVariant::fromValue(objects->at(2)));
    QCOMPARE(removedSpy.count(), 0);
    QCOMPARE(countSpy.count(), 2);

    addedSpy.clear();
    countSpy.clear();

    model.insertItem(1, objects->at(3));
    model.insertItem(0, objects->at(4));

    QCOMPARE(addedSpy.count(), 2);
    QCOMPARE(addedSpy.at(0), QVariantList() << QVariant::fromValue(objects->at(3)));
    QCOMPARE(addedSpy.at(1), QVariantList() << QVariant::fromValue(objects->at(4)));
    QCOMPARE(removedSpy.count(), 0);
    QCOMPARE(countSpy.count(), 2);

    QCOMPARE(::objectName(model.get(0)), QString("e"));
    QCOMPARE(::objectName(model.get(1)), QString("a"));
    QCOMPARE(::objectName(model.get(2)), QString("d"));
    QCOMPARE(::objectName(model.get(3)), QString("b"));
    QCOMPARE(::objectName(model.get(4)), QString("c"));

    qDeleteAll(*objects);
    delete objects;
}

void Ut_QObjectListModel::testRemoval()
{
    QList<QObject *> *objects = new QList<QObject *>;
    objects->append(makeObject("a"));
    objects->append(makeObject("b"));
    objects->append(makeObject("c"));
    objects->append(makeObject("d"));
    objects->append(makeObject("e"));

    QObjectListModel model(this);
    model.addItems(*objects);

    QCOMPARE(model.itemCount(), 5);
    QCOMPARE(::objectName(model.get(0)), QString("a"));
    QCOMPARE(::objectName(model.get(1)), QString("b"));
    QCOMPARE(::objectName(model.get(2)), QString("c"));
    QCOMPARE(::objectName(model.get(3)), QString("d"));
    QCOMPARE(::objectName(model.get(4)), QString("e"));

    QSignalSpy addedSpy(&model, SIGNAL(itemAdded(QObject*)));
    QSignalSpy removedSpy(&model, SIGNAL(itemRemoved(QObject*)));
    QSignalSpy countSpy(&model, SIGNAL(itemCountChanged()));
    QSignalSpy rowsRemovedSpy(&model, SIGNAL(rowsRemoved(QModelIndex,int,int)));

    model.removeItem(objects->at(0));
    model.removeItem(objects->at(2));
    model.removeItem(objects->at(4));

    QCOMPARE(model.itemCount(), 2);
    QCOMPARE(::objectName(model.get(0)), QString("b"));
    QCOMPARE(::objectName(model.get(1)), QString("d"));

    QCOMPARE(addedSpy.count(), 0);
    QCOMPARE(removedSpy.count(), 3);
    QCOMPARE(removedSpy.at(0), QVariantList() << QVariant::fromValue(objects->at(0)));
    QCOMPARE(removedSpy.at(1), QVariantList() << QVariant::fromValue(objects->at(2)));
    QCOMPARE(removedSpy.at(2), QVariantList() << QVariant::fromValue(objects->at(4)));
    QCOMPARE(countSpy.count(), 3);

    QCOMPARE(rowsRemovedSpy.count(), 3);
    QCOMPARE(qvariant_cast<QModelIndex>(rowsRemovedSpy.at(0).at(0)), QModelIndex());
    QCOMPARE(qvariant_cast<int>(rowsRemovedSpy.at(0).at(1)), 0);
    QCOMPARE(qvariant_cast<int>(rowsRemovedSpy.at(0).at(2)), 0);
    QCOMPARE(qvariant_cast<QModelIndex>(rowsRemovedSpy.at(1).at(0)), QModelIndex());
    QCOMPARE(qvariant_cast<int>(rowsRemovedSpy.at(1).at(1)), 1);
    QCOMPARE(qvariant_cast<int>(rowsRemovedSpy.at(1).at(2)), 1);
    QCOMPARE(qvariant_cast<QModelIndex>(rowsRemovedSpy.at(2).at(0)), QModelIndex());
    QCOMPARE(qvariant_cast<int>(rowsRemovedSpy.at(2).at(1)), 2);
    QCOMPARE(qvariant_cast<int>(rowsRemovedSpy.at(2).at(2)), 2);

    removedSpy.clear();
    countSpy.clear();
    rowsRemovedSpy.clear();

    model.removeItem(1);
    model.removeItem(0);
    
    QCOMPARE(model.itemCount(), 0);

    QCOMPARE(addedSpy.count(), 0);
    QCOMPARE(removedSpy.count(), 2);
    QCOMPARE(removedSpy.at(0), QVariantList() << QVariant::fromValue(objects->at(3)));
    QCOMPARE(removedSpy.at(1), QVariantList() << QVariant::fromValue(objects->at(1)));
    QCOMPARE(countSpy.count(), 2);

    QCOMPARE(rowsRemovedSpy.count(), 2);
    QCOMPARE(qvariant_cast<QModelIndex>(rowsRemovedSpy.at(0).at(0)), QModelIndex());
    QCOMPARE(qvariant_cast<int>(rowsRemovedSpy.at(0).at(1)), 1);
    QCOMPARE(qvariant_cast<int>(rowsRemovedSpy.at(0).at(2)), 1);
    QCOMPARE(qvariant_cast<QModelIndex>(rowsRemovedSpy.at(1).at(0)), QModelIndex());
    QCOMPARE(qvariant_cast<int>(rowsRemovedSpy.at(1).at(1)), 0);
    QCOMPARE(qvariant_cast<int>(rowsRemovedSpy.at(1).at(2)), 0);

    qDeleteAll(*objects);
    delete objects;
}

void Ut_QObjectListModel::testBatchRemoval()
{
    QList<QObject *> *objects = new QList<QObject *>;
    objects->append(makeObject("a"));
    objects->append(makeObject("b"));
    objects->append(makeObject("c"));
    objects->append(makeObject("d"));
    objects->append(makeObject("e"));
    objects->append(makeObject("f"));
    objects->append(makeObject("g"));
    objects->append(makeObject("h"));
    objects->append(makeObject("i"));

    QObjectListModel model(this);
    model.addItems(*objects);

    QCOMPARE(model.itemCount(), 9);
    QCOMPARE(::objectName(model.get(0)), QString("a"));
    QCOMPARE(::objectName(model.get(1)), QString("b"));
    QCOMPARE(::objectName(model.get(2)), QString("c"));
    QCOMPARE(::objectName(model.get(3)), QString("d"));
    QCOMPARE(::objectName(model.get(4)), QString("e"));
    QCOMPARE(::objectName(model.get(5)), QString("f"));
    QCOMPARE(::objectName(model.get(6)), QString("g"));
    QCOMPARE(::objectName(model.get(7)), QString("h"));
    QCOMPARE(::objectName(model.get(8)), QString("i"));

    QSignalSpy addedSpy(&model, SIGNAL(itemAdded(QObject*)));
    QSignalSpy removedSpy(&model, SIGNAL(itemRemoved(QObject*)));
    QSignalSpy countSpy(&model, SIGNAL(itemCountChanged()));
    QSignalSpy rowsRemovedSpy(&model, SIGNAL(rowsRemoved(QModelIndex,int,int)));

    QList<QObject *> removals;
    removals.append(objects->at(0));
    removals.append(objects->at(8));
    removals.append(objects->at(7));
    removals.append(objects->at(3));
    removals.append(objects->at(2));
    removals.append(objects->at(5));
    model.removeItems(removals);

    QCOMPARE(model.itemCount(), 3);
    QCOMPARE(::objectName(model.get(0)), QString("b"));
    QCOMPARE(::objectName(model.get(1)), QString("e"));
    QCOMPARE(::objectName(model.get(2)), QString("g"));

    QCOMPARE(addedSpy.count(), 0);
    QCOMPARE(removedSpy.count(), 6);
    QCOMPARE(removedSpy.at(0), QVariantList() << QVariant::fromValue(objects->at(0)));
    QCOMPARE(removedSpy.at(1), QVariantList() << QVariant::fromValue(objects->at(2)));
    QCOMPARE(removedSpy.at(2), QVariantList() << QVariant::fromValue(objects->at(3)));
    QCOMPARE(removedSpy.at(3), QVariantList() << QVariant::fromValue(objects->at(5)));
    QCOMPARE(removedSpy.at(4), QVariantList() << QVariant::fromValue(objects->at(7)));
    QCOMPARE(removedSpy.at(5), QVariantList() << QVariant::fromValue(objects->at(8)));
    QCOMPARE(countSpy.count(), 1);

    QCOMPARE(rowsRemovedSpy.count(), 4);
    QCOMPARE(qvariant_cast<QModelIndex>(rowsRemovedSpy.at(0).at(0)), QModelIndex());
    QCOMPARE(qvariant_cast<int>(rowsRemovedSpy.at(0).at(1)), 7);
    QCOMPARE(qvariant_cast<int>(rowsRemovedSpy.at(0).at(2)), 8);
    QCOMPARE(qvariant_cast<QModelIndex>(rowsRemovedSpy.at(1).at(0)), QModelIndex());
    QCOMPARE(qvariant_cast<int>(rowsRemovedSpy.at(1).at(1)), 5);
    QCOMPARE(qvariant_cast<int>(rowsRemovedSpy.at(1).at(2)), 5);
    QCOMPARE(qvariant_cast<QModelIndex>(rowsRemovedSpy.at(2).at(0)), QModelIndex());
    QCOMPARE(qvariant_cast<int>(rowsRemovedSpy.at(2).at(1)), 2);
    QCOMPARE(qvariant_cast<int>(rowsRemovedSpy.at(2).at(2)), 3);
    QCOMPARE(qvariant_cast<QModelIndex>(rowsRemovedSpy.at(3).at(0)), QModelIndex());
    QCOMPARE(qvariant_cast<int>(rowsRemovedSpy.at(3).at(1)), 0);
    QCOMPARE(qvariant_cast<int>(rowsRemovedSpy.at(3).at(2)), 0);

    removedSpy.clear();
    countSpy.clear();
    rowsRemovedSpy.clear();

    removals.clear();
    removals.append(objects->at(1));
    removals.append(objects->at(6));
    removals.append(objects->at(4));
    model.removeItems(removals);

    QCOMPARE(model.itemCount(), 0);

    QCOMPARE(addedSpy.count(), 0);
    QCOMPARE(removedSpy.count(), 3);
    QCOMPARE(removedSpy.at(0), QVariantList() << QVariant::fromValue(objects->at(1)));
    QCOMPARE(removedSpy.at(1), QVariantList() << QVariant::fromValue(objects->at(4)));
    QCOMPARE(removedSpy.at(2), QVariantList() << QVariant::fromValue(objects->at(6)));
    QCOMPARE(countSpy.count(), 1);

    QCOMPARE(rowsRemovedSpy.count(), 1);
    QCOMPARE(qvariant_cast<QModelIndex>(rowsRemovedSpy.at(0).at(0)), QModelIndex());
    QCOMPARE(qvariant_cast<int>(rowsRemovedSpy.at(0).at(1)), 0);
    QCOMPARE(qvariant_cast<int>(rowsRemovedSpy.at(0).at(2)), 2);

    qDeleteAll(*objects);
    delete objects;
}

void Ut_QObjectListModel::testMove()
{
    QList<QObject *> *objects = new QList<QObject *>;
    objects->append(makeObject("a"));
    objects->append(makeObject("b"));
    objects->append(makeObject("c"));
    objects->append(makeObject("d"));
    objects->append(makeObject("e"));

    QObjectListModel model(this);
    model.addItems(*objects);

    QCOMPARE(model.itemCount(), 5);
    QCOMPARE(::objectName(model.get(0)), QString("a"));
    QCOMPARE(::objectName(model.get(1)), QString("b"));
    QCOMPARE(::objectName(model.get(2)), QString("c"));
    QCOMPARE(::objectName(model.get(3)), QString("d"));
    QCOMPARE(::objectName(model.get(4)), QString("e"));

    QSignalSpy addedSpy(&model, SIGNAL(itemAdded(QObject*)));
    QSignalSpy removedSpy(&model, SIGNAL(itemRemoved(QObject*)));
    QSignalSpy countSpy(&model, SIGNAL(itemCountChanged()));
    QSignalSpy movedSpy(&model, SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)));

    model.move(0, 2);
    model.move(4, 3);

    QCOMPARE(model.itemCount(), 5);
    QCOMPARE(::objectName(model.get(0)), QString("b"));
    QCOMPARE(::objectName(model.get(1)), QString("c"));
    QCOMPARE(::objectName(model.get(2)), QString("a"));
    QCOMPARE(::objectName(model.get(3)), QString("e"));
    QCOMPARE(::objectName(model.get(4)), QString("d"));

    QCOMPARE(addedSpy.count(), 0);
    QCOMPARE(removedSpy.count(), 0);
    QCOMPARE(countSpy.count(), 0);
    QCOMPARE(movedSpy.count(), 2);
    QCOMPARE(movedSpy.at(0), QVariantList() << QModelIndex() << 0 << 0 << QModelIndex() << 3);
    QCOMPARE(movedSpy.at(1), QVariantList() << QModelIndex() << 4 << 4 << QModelIndex() << 3);

    qDeleteAll(*objects);
    delete objects;
}

void Ut_QObjectListModel::testUpdate()
{
    QList<QObject *> *objects = new QList<QObject *>;
    objects->append(makeObject("a"));
    objects->append(makeObject("b"));
    objects->append(makeObject("c"));
    objects->append(makeObject("d"));
    objects->append(makeObject("e"));

    QObjectListModel model(this);
    model.addItems(*objects);

    QCOMPARE(model.itemCount(), 5);
    QCOMPARE(::objectName(model.get(0)), QString("a"));
    QCOMPARE(::objectName(model.get(1)), QString("b"));
    QCOMPARE(::objectName(model.get(2)), QString("c"));
    QCOMPARE(::objectName(model.get(3)), QString("d"));
    QCOMPARE(::objectName(model.get(4)), QString("e"));

    QSignalSpy addedSpy(&model, SIGNAL(itemAdded(QObject*)));
    QSignalSpy removedSpy(&model, SIGNAL(itemRemoved(QObject*)));
    QSignalSpy countSpy(&model, SIGNAL(itemCountChanged()));
    QSignalSpy changedSpy(&model, SIGNAL(dataChanged(QModelIndex,QModelIndex)));

    model.update(1);

    QCOMPARE(addedSpy.count(), 0);
    QCOMPARE(removedSpy.count(), 0);
    QCOMPARE(countSpy.count(), 0);
    QCOMPARE(changedSpy.count(), 1);
    QVariantList args = changedSpy.takeFirst();
    QCOMPARE(args.count(), 2);
    QModelIndex topLeft(qvariant_cast<QModelIndex>(args.at(0)));
    QCOMPARE(topLeft.parent(), QModelIndex());
    QCOMPARE(topLeft.row(), 1);
    QCOMPARE(topLeft.column(), 0);
    QModelIndex bottomRight(qvariant_cast<QModelIndex>(args.at(1)));
    QCOMPARE(bottomRight.parent(), QModelIndex());
    QCOMPARE(bottomRight.row(), 1);
    QCOMPARE(bottomRight.column(), 0);

    qDeleteAll(*objects);
    delete objects;
}

void Ut_QObjectListModel::testSynchronization()
{
    QList<QObject *> *objects = new QList<QObject *>;
    objects->append(makeObject("a"));
    objects->append(makeObject("b"));
    objects->append(makeObject("c"));
    objects->append(makeObject("d"));
    objects->append(makeObject("e"));

    QObjectListModel model(this);
    model.addItem(objects->at(0));
    model.addItem(objects->at(2));
    model.addItem(objects->at(4));

    QCOMPARE(model.itemCount(), 3);
    QCOMPARE(::objectName(model.get(0)), QString("a"));
    QCOMPARE(::objectName(model.get(1)), QString("c"));
    QCOMPARE(::objectName(model.get(2)), QString("e"));

    QSignalSpy addedSpy(&model, SIGNAL(itemAdded(QObject*)));
    QSignalSpy removedSpy(&model, SIGNAL(itemRemoved(QObject*)));
    QSignalSpy countSpy(&model, SIGNAL(itemCountChanged()));
    QSignalSpy movedSpy(&model, SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)));
    QSignalSpy rowsInsertedSpy(&model, SIGNAL(rowsInserted(QModelIndex,int,int)));
    QSignalSpy rowsRemovedSpy(&model, SIGNAL(rowsRemoved(QModelIndex,int,int)));

    model.synchronizeList(*objects);

    QCOMPARE(model.itemCount(), 5);
    QCOMPARE(::objectName(model.get(0)), QString("a"));
    QCOMPARE(::objectName(model.get(1)), QString("b"));
    QCOMPARE(::objectName(model.get(2)), QString("c"));
    QCOMPARE(::objectName(model.get(3)), QString("d"));
    QCOMPARE(::objectName(model.get(4)), QString("e"));

    QCOMPARE(addedSpy.count(), 2);
    QCOMPARE(addedSpy.at(0), QVariantList() << QVariant::fromValue(objects->at(1)));
    QCOMPARE(addedSpy.at(1), QVariantList() << QVariant::fromValue(objects->at(3)));
    QCOMPARE(removedSpy.count(), 0);
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(movedSpy.count(), 0);
    QCOMPARE(rowsInsertedSpy.count(), 2);
    QCOMPARE(qvariant_cast<QModelIndex>(rowsInsertedSpy.at(0).at(0)), QModelIndex());
    QCOMPARE(qvariant_cast<int>(rowsInsertedSpy.at(0).at(1)), 1);
    QCOMPARE(qvariant_cast<int>(rowsInsertedSpy.at(0).at(2)), 1);
    QCOMPARE(qvariant_cast<QModelIndex>(rowsInsertedSpy.at(1).at(0)), QModelIndex());
    QCOMPARE(qvariant_cast<int>(rowsInsertedSpy.at(1).at(1)), 3);
    QCOMPARE(qvariant_cast<int>(rowsInsertedSpy.at(1).at(2)), 3);
    QCOMPARE(rowsRemovedSpy.count(), 0);

    addedSpy.clear();
    countSpy.clear();
    rowsInsertedSpy.clear();

    model.synchronizeList(QList<QObject *>() << objects->at(0) << objects->at(2));
    QCOMPARE(model.itemCount(), 2);
    QCOMPARE(::objectName(model.get(0)), QString("a"));
    QCOMPARE(::objectName(model.get(1)), QString("c"));

    QCOMPARE(addedSpy.count(), 0);
    QCOMPARE(removedSpy.count(), 3);
    QCOMPARE(removedSpy.at(0), QVariantList() << QVariant::fromValue(objects->at(1)));
    QCOMPARE(removedSpy.at(1), QVariantList() << QVariant::fromValue(objects->at(3)));
    QCOMPARE(removedSpy.at(2), QVariantList() << QVariant::fromValue(objects->at(4)));
    QCOMPARE(countSpy.count(), 1);
    QCOMPARE(movedSpy.count(), 0);
    QCOMPARE(rowsInsertedSpy.count(), 0);
    QCOMPARE(rowsRemovedSpy.count(), 2);
    QCOMPARE(qvariant_cast<QModelIndex>(rowsRemovedSpy.at(0).at(0)), QModelIndex());
    QCOMPARE(qvariant_cast<int>(rowsRemovedSpy.at(0).at(1)), 1);
    QCOMPARE(qvariant_cast<int>(rowsRemovedSpy.at(0).at(2)), 1);
    QCOMPARE(qvariant_cast<QModelIndex>(rowsRemovedSpy.at(1).at(0)), QModelIndex());
    QCOMPARE(qvariant_cast<int>(rowsRemovedSpy.at(1).at(1)), 2);
    QCOMPARE(qvariant_cast<int>(rowsRemovedSpy.at(1).at(2)), 3);

    removedSpy.clear();
    countSpy.clear();
    rowsRemovedSpy.clear();

    model.synchronizeList(QList<QObject *>() << objects->at(2) << objects->at(0));
    QCOMPARE(model.itemCount(), 2);
    QCOMPARE(::objectName(model.get(0)), QString("c"));
    QCOMPARE(::objectName(model.get(1)), QString("a"));

    QCOMPARE(addedSpy.count(), 0);
    QCOMPARE(removedSpy.count(), 0);
    QCOMPARE(countSpy.count(), 0);
    QCOMPARE(movedSpy.count(), 0);
    QCOMPARE(rowsInsertedSpy.count(), 1);
    QCOMPARE(qvariant_cast<QModelIndex>(rowsInsertedSpy.at(0).at(0)), QModelIndex());
    QCOMPARE(qvariant_cast<int>(rowsInsertedSpy.at(0).at(1)), 0);
    QCOMPARE(qvariant_cast<int>(rowsInsertedSpy.at(0).at(2)), 0);
    QCOMPARE(rowsRemovedSpy.count(), 1);
    QCOMPARE(qvariant_cast<QModelIndex>(rowsRemovedSpy.at(0).at(0)), QModelIndex());
    QCOMPARE(qvariant_cast<int>(rowsRemovedSpy.at(0).at(1)), 2);
    QCOMPARE(qvariant_cast<int>(rowsRemovedSpy.at(0).at(2)), 2);

    qDeleteAll(*objects);
    delete objects;
}

QTEST_MAIN(Ut_QObjectListModel)

#include "ut_qobjectlistmodel.moc"

