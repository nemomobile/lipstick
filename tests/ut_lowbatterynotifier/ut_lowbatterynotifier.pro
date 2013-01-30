include(../common.pri)
TARGET = ut_lowbatterynotifier
INCLUDEPATH += $$NOTIFICATIONSRCDIR /usr/include/qmsystem2 /usr/include/contextsubscriber

SOURCES += \
    ut_lowbatterynotifier.cpp \
    $$NOTIFICATIONSRCDIR/lowbatterynotifier.cpp \
    stubbase.cpp

HEADERS += \
    ut_lowbatterynotifier.h \
    $$NOTIFICATIONSRCDIR/lowbatterynotifier.h \
    /usr/include/qmsystem2/qmdisplaystate.h \
    /usr/include/contextsubscriber/contextproperty.h
