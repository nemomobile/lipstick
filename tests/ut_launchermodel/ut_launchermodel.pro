include(../common.pri)
TARGET = ut_launchermodel

INCLUDEPATH += $$COMPONENTSSRCDIR
INCLUDEPATH += $$UTILITYSRCDIR

QMAKE_CXXFLAGS += `pkg-config --cflags-only-I mlite5`

QT += dbus

SOURCES += \
    ut_launchermodel.cpp \
    $$COMPONENTSSRCDIR/launchermodel.cpp \
    $$COMPONENTSSRCDIR/launchermonitor.cpp \
    $$COMPONENTSSRCDIR/launcheritem.cpp \
    $$COMPONENTSSRCDIR/launcherdbus.cpp \
    $$STUBSDIR/stubbase.cpp \
    $$UTILITYSRCDIR/qobjectlistmodel.cpp \

HEADERS += \
    ut_launchermodel.h \
    $$COMPONENTSSRCDIR/launchermodel.h \
    $$COMPONENTSSRCDIR/launchermonitor.h \
    $$COMPONENTSSRCDIR/launcheritem.h \
    $$COMPONENTSSRCDIR/launcherdbus.h \
    $$UTILITYSRCDIR/qobjectlistmodel.h \
    /usr/include/mlite5/mdesktopentry.h \

