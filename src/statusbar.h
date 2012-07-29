#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QDeclarativeItem>
#include <QPixmap>

class StatusBar : public QDeclarativeItem
{
    Q_OBJECT
    QPixmap _sharedPixmap;

public:
    explicit StatusBar(QDeclarativeItem *parent = 0);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void updateSharedPixmap();

};

#endif // STATUSBAR_H
