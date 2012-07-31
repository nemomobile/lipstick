#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QDeclarativeItem>
#include <QPixmap>

class StatusBar : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(bool isPortrait READ isPortrait WRITE setIsPortrait NOTIFY isPortraitChanged)

    QPixmap _sharedPixmap;
    bool _isPortrait;

public:
    explicit StatusBar(QDeclarativeItem *parent = 0);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void updateSharedPixmap();
    bool isPortrait() const;
    void setIsPortrait(bool value);

signals:
    void isPortraitChanged();

};

#endif // STATUSBAR_H
