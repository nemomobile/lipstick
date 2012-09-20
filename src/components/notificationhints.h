#ifndef NOTIFICATIONHINTS_H
#define NOTIFICATIONHINTS_H

#include <QHash>
#include <QVariant>

class QDataStream;
class QDBusArgument;

class NotificationHints
{
public:
    NotificationHints();

    friend QDataStream &operator<<(QDataStream &, const NotificationHints &);
    friend QDataStream &operator>>(QDataStream &, NotificationHints &);

    friend QDBusArgument &operator<<(QDBusArgument &, const NotificationHints &);
    friend const QDBusArgument &operator>>(const QDBusArgument &, NotificationHints &);

private:
    QHash<QString, QVariant> hintValues;
};

QDataStream &operator<<(QDataStream &datastream, const NotificationHints &hints);
QDataStream &operator>>(QDataStream &datastream, NotificationHints &hints);

Q_DECLARE_METATYPE(NotificationHints)

#endif // NOTIFICATIONHINTS_H
