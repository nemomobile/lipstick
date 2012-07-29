
#include "statusbar.h"
#include <QPainter>
#include <QX11Info>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/Xdamage.h>

// Fetches the shared status bar pixmap
static QPixmap fetchSharedPixmap()
{
    static Atom propertyWindowAtom = 0;
    static Atom pixmapHandleAtom = 0;

    // This contains the statusbar window
    if (propertyWindowAtom == 0)
        propertyWindowAtom = XInternAtom(QX11Info::display(), "_MEEGOTOUCH_STATUSBAR_PROPERTY_WINDOW", False);

    // This contains the shared pixmap
    if (pixmapHandleAtom == 0)
        pixmapHandleAtom = XInternAtom(QX11Info::display(), "_MEEGOTOUCH_STATUSBAR_PIXMAP", False);

    // Some common variables
    Atom actualType = 0;
    int actualFormat = 0;
    unsigned long nitems = 0;
    unsigned long bytesAfter = 0;
    unsigned char *data = 0;
    int status = 0;

    // Getting the status bar window
    status = XGetWindowProperty(QX11Info::display(),
                                QX11Info::appRootWindow(),
                                propertyWindowAtom,
                                0,
                                1,
                                False,
                                XA_WINDOW,
                                &actualType,
                                &actualFormat,
                                &nitems,
                                &bytesAfter,
                                &data);

    Window window(0);
    if (status == Success && data != None)
    {
        window = *(Window *)data;
        XFree(data);
    }

    // Getting the shared pixmap from the status bar window
    status = XGetWindowProperty(QX11Info::display(),
                                window,
                                pixmapHandleAtom,
                                0,
                                1,
                                False,
                                XA_PIXMAP,
                                &actualType,
                                &actualFormat,
                                &nitems,
                                &bytesAfter,
                                &data);

    QPixmap result;

    if (status == Success)
    {
        quint32 handle = *((unsigned long*)data);

        if (actualType == XA_PIXMAP && actualFormat == 32 && handle != 0)
        {
            result = QPixmap::fromX11Pixmap(handle, QPixmap::ExplicitlyShared);
            // TODO: we should register for damage events for this pixmap and repaint when they arrive
            //       (perhaps create an XEventListener fromt his class too?)
        }

        XFree(data);
    }

    return result;
}

StatusBar::StatusBar(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{
    _sharedPixmap = fetchSharedPixmap();
    setFlag(QGraphicsItem::ItemHasNoContents, false);
    setImplicitWidth(_sharedPixmap.width());
    setImplicitHeight(_sharedPixmap.height() / 2);
}

void StatusBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if (_sharedPixmap.isNull())
    {
        qDebug() << "StatusBar: the shared pixmap is null, can't draw it!";
        return;
    }

    // Info: The shared pixmap contains both the portrait and the landscape mode status bars below each other.
    //       Landscape is on top, portrait on bottom. They have the same height.
    //       All we need is check the orientation here and set the source rect accordingly.

    painter->drawPixmap(QRectF(0, 0, width(), height()), _sharedPixmap, QRectF(0, _isPortrait ? height() : 0, width(), height()));
}

bool StatusBar::isPortrait() const
{
    return _isPortrait;
}

void StatusBar::setIsPortrait(bool value)
{
    _isPortrait = value;
    emit isPortraitChanged();
}
