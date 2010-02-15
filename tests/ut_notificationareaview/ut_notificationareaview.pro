include(../common_top.pri)
TARGET = ut_notificationareaview
INCLUDEPATH += $$SRCDIR/notifications

STYLE_HEADERS += $$SRCDIR/notificationareastyle.h
MODEL_HEADERS += $$SRCDIR/notificationareamodel.h

# unit test and unit
SOURCES += \
    ut_notificationareaview.cpp \
    $$SRCDIR/notificationareaview.cpp

# base classes
SOURCES += \


# unit test and unit
HEADERS += \
    ut_notificationareaview.h \
    $$SRCDIR/notificationareaview.h \
    $$SRCDIR/notificationareastyle.h \
    $$SRCDIR/notificationarea.h \
    $$SRCDIR/notificationareamodel.h

# base classes
HEADERS += \

# service classes
SOURCES += ../stubs/stubbase.cpp

# service classes
HEADERS += \


include(../common_bot.pri)
