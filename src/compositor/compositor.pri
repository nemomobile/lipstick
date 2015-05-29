system(qdbusxml2cpp compositor.xml -a lipstickcompositoradaptor -c LipstickCompositorAdaptor -l LipstickCompositor -i lipstickcompositor.h)

INCLUDEPATH += $$PWD

PUBLICHEADERS += \
    $$PWD/lipstickcompositor.h \
    $$PWD/lipstickcompositorwindow.h \
    $$PWD/lipstickcompositorprocwindow.h \
    $$PWD/lipstickcompositoradaptor.h \
    $$PWD/lipstickkeymap.h \
    $$PWD/windowmodel.h \
    $$PWD/lipsticksurfaceinterface.h \

HEADERS += \
    $$PWD/windowpixmapitem.h \
    $$PWD/windowproperty.h \
    $$PWD/lipstickrecorder.h \
    $$PWD/hwcrenderstage.h \
    $$PWD/hwcimage.h \

SOURCES += \
    $$PWD/lipstickcompositor.cpp \
    $$PWD/lipstickcompositorwindow.cpp \
    $$PWD/lipstickcompositorprocwindow.cpp \
    $$PWD/lipstickcompositoradaptor.cpp \
    $$PWD/lipstickkeymap.cpp \
    $$PWD/windowmodel.cpp \
    $$PWD/windowpixmapitem.cpp \
    $$PWD/windowproperty.cpp \
    $$PWD/lipsticksurfaceinterface.cpp \
    $$PWD/lipstickrecorder.cpp \
    $$PWD/hwcrenderstage.cpp \
    $$PWD/hwcimage.cpp \

DEFINES += QT_COMPOSITOR_QUICK

QT += compositor

# needed for hardware compositor
QT += quick-private gui-private core-private compositor-private

WAYLANDSERVERSOURCES += ../protocol/lipstick-recorder.xml \
