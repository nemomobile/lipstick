INCLUDEPATH += $$PWD

PUBLICHEADERS += \
    $$PWD/lipstickcompositor.h \
    $$PWD/windowmodel.h \

HEADERS += \
    $$PWD/windowpixmapitem.h \
    $$PWD/windowproperty.h \

SOURCES += \
    $$PWD/lipstickcompositor.cpp \
    $$PWD/windowmodel.cpp \
    $$PWD/windowpixmapitem.cpp \
    $$PWD/windowproperty.cpp \

DEFINES += QT_COMPOSITOR_QUICK

QT += compositor
