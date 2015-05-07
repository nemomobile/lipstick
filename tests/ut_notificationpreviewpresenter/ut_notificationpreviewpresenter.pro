include(../common.pri)
TARGET = ut_notificationpreviewpresenter
INCLUDEPATH += $$SRCDIR $$NOTIFICATIONSRCDIR $$UTILITYSRCDIR $$SRCDIR/compositor ../../src/qmsystem2
QT += qml quick dbus compositor

# unit test and unit
SOURCES += \
    ut_notificationpreviewpresenter.cpp \
    $$NOTIFICATIONSRCDIR/notificationpreviewpresenter.cpp \
    $$NOTIFICATIONSRCDIR/lipsticknotification.cpp \
    $$STUBSDIR/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_notificationpreviewpresenter.h \
    $$NOTIFICATIONSRCDIR/notificationpreviewpresenter.h \
    $$NOTIFICATIONSRCDIR/notificationfeedbackplayer.h \
    $$NOTIFICATIONSRCDIR/notificationmanager.h \
    $$NOTIFICATIONSRCDIR/lipsticknotification.h \
    $$UTILITYSRCDIR/closeeventeater.h \
    $$COMPOSITORSRCDIR/lipstickcompositor.h \
    ../../src/qmsystem2/qmlocks.h \
    ../../src/qmsystem2/qmdisplaystate.h \
    $$SRCDIR/homewindow.h \
