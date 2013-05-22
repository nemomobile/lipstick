include(../common.pri)
TARGET = ut_shutdownscreen
CONFIG += link_pkgconfig
INCLUDEPATH += $$SRCDIR $$NOTIFICATIONSRCDIR $$UTILITYSRCDIR $$XTOOLSRCDIR
QT += qml quick
PKGCONFIG += qmsystem2-qt5

# unit test and unit
SOURCES += \
    $$SRCDIR/shutdownscreen.cpp \
    $$STUBSDIR/stubbase.cpp \
    $$STUBSDIR/homewindow.cpp \
    $$STUBSDIR/homeapplication.cpp \
    ut_shutdownscreen.cpp

HEADERS += \
    $$SRCDIR/shutdownscreen.h \
    $$NOTIFICATIONSRCDIR/notificationmanager.h \
    $$UTILITYSRCDIR/closeeventeater.h \
    $$SRCDIR/homeapplication.h \
    $$SRCDIR/homewindow.h \
    ut_shutdownscreen.h
