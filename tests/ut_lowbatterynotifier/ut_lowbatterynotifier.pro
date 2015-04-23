include(../common.pri)
TARGET = ut_lowbatterynotifier
INCLUDEPATH += $$NOTIFICATIONSRCDIR

SOURCES += \
    ut_lowbatterynotifier.cpp \
    $$NOTIFICATIONSRCDIR/lowbatterynotifier.cpp \
    $$STUBSDIR/stubbase.cpp

HEADERS += \
    ut_lowbatterynotifier.h \
    $$NOTIFICATIONSRCDIR/lowbatterynotifier.h \

packagesExist(contextkit-statefs) {
    DEFINES += HAVE_CONTEXTSUBSCRIBER
    HEADERS += /usr/include/contextproperty.h
} else {
    warning("Contextsubscriber not found")
}
