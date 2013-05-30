include(../common.pri)
TARGET = ut_notificationpreviewpresenter
INCLUDEPATH += $$SRCDIR $$NOTIFICATIONSRCDIR $$UTILITYSRCDIR
QT += declarative dbus

equals(QT_MAJOR_VERSION, 4): QMSYSTEM = qmsystem2
equals(QT_MAJOR_VERSION, 5): QMSYSTEM = qmsystem2-qt5

packagesExist($$QMSYSTEM) {
    DEFINES += HAVE_QMSYSTEM
    INCLUDEPATH += /usr/include/$$QMSYSTEM
} else {
    warning("QmSystem2 not found")
}

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
    $$NOTIFICATIONSRCDIR/notificationmanager.h \
    $$NOTIFICATIONSRCDIR/lipsticknotification.h \
    $$UTILITYSRCDIR/closeeventeater.h \
    /usr/include/$$QMSYSTEM/qmlocks.h \
    /usr/include/$$QMSYSTEM/qmdisplaystate.h
