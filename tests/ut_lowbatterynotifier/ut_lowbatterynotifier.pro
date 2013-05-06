include(../common.pri)
TARGET = ut_lowbatterynotifier
INCLUDEPATH += $$NOTIFICATIONSRCDIR /usr/include/qmsystem2-qt5

SOURCES += \
    ut_lowbatterynotifier.cpp \
    $$NOTIFICATIONSRCDIR/lowbatterynotifier.cpp \
    $$STUBSDIR/stubbase.cpp

HEADERS += \
    ut_lowbatterynotifier.h \
    $$NOTIFICATIONSRCDIR/lowbatterynotifier.h \
    /usr/include/qmsystem2-qt5/qmdisplaystate.h

packagesExist(contextsubscriber-1.0) {
    DEFINES += HAVE_CONTEXTSUBSCRIBER
    INCLUDEPATH += /usr/include/contextsubscriber
    HEADERS += /usr/include/contextsubscriber/contextproperty.h
} else {
    warning("Contextsubscriber not found")
}
