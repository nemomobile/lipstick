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

packagesExist(contextkit-statefs) {
    DEFINES += HAVE_CONTEXTSUBSCRIBER
    HEADERS += /usr/include/contextproperty.h
} else {
    warning("Contextsubscriber not found")
}
