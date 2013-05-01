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

equals(QT_MAJOR_VERSION, 4): QMSYSTEM = qmsystem2
equals(QT_MAJOR_VERSION, 5): QMSYSTEM = qmsystem2-qt5

packagesExist($$QMSYSTEM) {
    DEFINES += HAVE_QMSYSTEM
    INCLUDEPATH += /usr/include/$$QMSYSTEM
    HEADERS += /usr/include/$$QMSYSTEM/qmdisplaystate.h
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
