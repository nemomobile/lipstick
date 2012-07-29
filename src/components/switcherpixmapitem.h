#ifndef SWITCHERPIXMAPITEM_H
#define SWITCHERPIXMAPITEM_H

#include <QDeclarativeItem>

class SwitcherPixmapItem : public QDeclarativeItem
{
    Q_OBJECT
public:
    SwitcherPixmapItem();
    ~SwitcherPixmapItem();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    int windowId() const;
    void setWindowId(int windowId);
    Q_PROPERTY(int windowId READ windowId WRITE setWindowId);

private slots:
    void updateXWindowIconGeometry();
    void damageEvent(Qt::HANDLE &damage, short &x, short &y, unsigned short &width, unsigned short &height);
private:
    void createDamage();
    void destroyDamage();
    void updateXWindowPixmap();
    void updateXWindowIconGeometryIfNecessary();

    struct Private;
    Private * const d;
};

#endif // SWITCHERPIXMAPITEM_H
