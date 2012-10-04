include(../common.pri)
TARGET = ut_categorydefinitionstore
INCLUDEPATH += $$NOTIFICATIONSRCDIR

# unit test and unit
SOURCES += \
    ut_categorydefinitionstore.cpp \
    $$NOTIFICATIONSRCDIR/categorydefinitionstore.cpp \
    $$STUBSDIR/stubbase.cpp \

# unit test and unit
HEADERS += \
    ut_categorydefinitionstore.h \
    $$NOTIFICATIONSRCDIR/categorydefinitionstore.h
