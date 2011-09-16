#ifndef WINDOWPIXMAPPROVIDER_H
#define WINDOWPIXMAPPROVIDER_H

#include "x11wrapper.h"
#include "xeventlistener.h"

#include <QDeclarativeImageProvider>
#include <QPixmap>

class WindowPixmapProvider : public QDeclarativeImageProvider, XEventListener
{
public:
    WindowPixmapProvider();
    ~WindowPixmapProvider();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);
    bool handleXEvent(const XEvent &event);

    void clearCacheFor(const QString &windowId);
    void refreshPixmapFor(const QString &windowId);

private:
    // TODO: move these to a struct?
    QHash<QString, QPixmap> mPixmaps;
    QHash<QString, Damage> mDamages;
};

#endif // WINDOWPIXMAPPROVIDER_H
