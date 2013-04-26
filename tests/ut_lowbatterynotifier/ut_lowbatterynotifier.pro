include(../common.pri)
TARGET = ut_lowbatterynotifier
INCLUDEPATH += $$NOTIFICATIONSRCDIR

SOURCES += \
    ut_lowbatterynotifier.cpp \
    $$NOTIFICATIONSRCDIR/lowbatterynotifier.cpp \
    $$STUBSDIR/stubbase.cpp

HEADERS += \
    ut_lowbatterynotifier.h \
    $$NOTIFICATIONSRCDIR/lowbatterynotifier.h

packagesExist(qmsystem2) {
    DEFINES += HAVE_QMSYSTEM
    INCLUDEPATH += /usr/include/qmsystem2
    HEADERS += /usr/include/qmsystem2/qmdisplaystate.h
} else {
    warning("QmSystem2 not found")
}

packagesExist(contextsubscriber-1.0) {
    DEFINES += HAVE_CONTEXTSUBSCRIBER
    INCLUDEPATH += /usr/include/contextsubscriber
    HEADERS += /usr/include/contextsubscriber/contextproperty.h
} else {
    warning("Contextsubscriber not found")
}
