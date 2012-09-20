#ifndef NOTIFICATIONMANAGER_H
#define NOTIFICATIONMANAGER_H

#include "notificationhints.h"
#include <QObject>

class NotificationManager : public QObject
{
    Q_OBJECT
public:
    explicit NotificationManager(QObject *parent = 0);
    
    QStringList GetCapabilities();
    uint Notify(const QString &app_name, uint replaces_id, const QString &app_icon, const QString &summary, const QString &body, const QStringList &actions, NotificationHints hints, int expire_timeout);
    void CloseNotification(uint id);
    QString GetServerInformation(QString &name, QString &vendor, QString &version);

signals:
    void NotificationClosed(uint id, uint reason);
    void ActionInvoked(uint id, const QString &action_key);
};

#endif // NOTIFICATIONMANAGER_H
