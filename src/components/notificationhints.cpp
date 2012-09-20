#include <QDBusArgument>
#include "notificationhints.h"

NotificationHints::NotificationHints()
{
}

QDataStream &operator<<(QDataStream &datastream, const NotificationHints &hints)
{
    return datastream << hints.hintValues;
}

QDataStream &operator>>(QDataStream &datastream, NotificationHints &hints)
{
    return datastream >> hints.hintValues;
}

QDBusArgument &operator<<(QDBusArgument &argument, const NotificationHints &hints)
{
    QHashIterator<QString, QVariant> i(hints.hintValues);

    argument.beginMap(QMetaType::QString, qMetaTypeId<QDBusVariant>());
    while (i.hasNext()) {
        i.next();

        argument.beginMapEntry();
        argument << i.key();
        argument << QDBusVariant(i.value());
        argument.endMapEntry();
    }
    argument.endMap();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, NotificationHints &hints)
{
    argument.beginMap();
    while (!argument.atEnd()) {
        QString key;
        QDBusVariant value;

        argument.beginMapEntry();
        argument >> key;
        argument >> value;
        argument.endMapEntry();

        hints.hintValues[key] = value.variant();
    }
    argument.endMap();
    return argument;
}
