#include <QDebug>
#include <QApplication>
#include <QX11Info>

#include "windowpixmapprovider.h"
#include "windowinfo.h"

// TODO: handle damage events on mDamage. need to get it from MApplication...

WindowPixmapProvider::WindowPixmapProvider()
    : QDeclarativeImageProvider(Pixmap)
{
    // TODO: as in mhome, we should disconnect this when appropriate
    connect(qApp, SIGNAL(damageEvent(Qt::HANDLE &, short &, short &, unsigned short &, unsigned short &)), this, SLOT(damageEvent(Qt::HANDLE &, short &, short &, unsigned short &, unsigned short &)));

}

WindowPixmapProvider::~WindowPixmapProvider()
{

}

QPixmap WindowPixmapProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    QString windowId = id.section('/', 0, 0);
    qDebug() << Q_FUNC_INFO << "Requested pixmap for window " << windowId;

    QHash<QString, QPixmap>::ConstIterator it = mPixmaps.find(windowId);
    if (it == mPixmaps.constEnd()) {
        qWarning() << Q_FUNC_INFO << "PANIC! No pixmap for " << windowId;
        refreshPixmapFor(windowId);
        return QPixmap();
    }


    // grab a reference
    //
    // note that we explicitly copy the original pixmap so we aren't affected by
    // it becoming invalid; we do this with the expectation that QPixmapCache
    // will cover our inefficiencies.
    QPixmap qp = (*it).copy();

    if (requestedSize.isValid()) {
        qp = qp.scaled(requestedSize);
    }

    if (size)
        *size = qp.size();

    return qp;
}

bool WindowPixmapProvider::handleXEvent(const XEvent &event)
{
    if (event.type == VisibilityNotify &&
        event.xvisibility.state == VisibilityFullyObscured &&
        event.xvisibility.send_event == True) {
        // window obscured; remove from cache if it exists
        clearCacheFor(QString::number(event.xvisibility.window));
        return true;
    }

    return false;
}

void WindowPixmapProvider::damageEvent(Qt::HANDLE &damage, short &x, short &y, unsigned short &width, unsigned short &height)
{
    Q_UNUSED(x);
    Q_UNUSED(y);
    Q_UNUSED(width);
    Q_UNUSED(height);

    // XXX: TODO: this is inefficient
    foreach (const QString &wid, mDamages.keys()) {
        if (mDamages.value(wid) == damage) {
            qDebug() << Q_FUNC_INFO << "Got damage event for " << wid;
            X11Wrapper::XDamageSubtract(QX11Info::display(), mDamages.value(wid), None, None);

            WindowInfo *winInfo = WindowInfo::windowFor(wid.toULongLong());
            winInfo->setPixmapSerial(winInfo->pixmapSerial() + 1);
            return;
        }
    }
}

void WindowPixmapProvider::clearCacheFor(const QString &windowId)
{
    QHash<QString, Damage>::iterator it = mDamages.find(windowId);
    if (it != mDamages.end()) {
        X11Wrapper::XDamageDestroy(QX11Info::display(), *it);
        mDamages.erase(it);
    }

    QHash<QString, QPixmap>::iterator pit = mPixmaps.find(windowId);
    if (pit != mPixmaps.end()) {
        // Dereference the old pixmap ID
        X11Wrapper::XFreePixmap(QX11Info::display(), (*pit).handle());
        mPixmaps.erase(pit);
    }
}

void WindowPixmapProvider::refreshPixmapFor(const QString &windowId)
{
    Window wid = windowId.toLongLong();
    qDebug() << Q_FUNC_INFO << "Refreshing pixmap for " << windowId << "(" << wid << ")";

    // TODO: set some kind of a pixmapCounter property so we can reliably
    // refresh windows, similar to contacts demo solution to lack of a reliable
    // reload mechanism for pixmap providers. for now, we'll just reload
    // constantly.

    // Get the pixmap ID of the X window
    QPixmap newWindowPixmap = X11Wrapper::XCompositeNameWindowPixmap(QX11Info::display(), wid);

    if (!newWindowPixmap.isNull()) {
        // remove previous pixmap
        clearCacheFor(windowId);

        // add new one
        mPixmaps.insert(windowId, newWindowPixmap);

        // Register the pixmap for XDamage events
        // TODO: mhome-mtf only registered if onDisplay
        mDamages.insert(windowId, X11Wrapper::XDamageCreate(QX11Info::display(), wid, XDamageReportNonEmpty));

        // force reload from QML
        WindowInfo *winInfo = WindowInfo::windowFor(wid);
        winInfo->setPixmapSerial(winInfo->pixmapSerial() + 1);
    }
}

