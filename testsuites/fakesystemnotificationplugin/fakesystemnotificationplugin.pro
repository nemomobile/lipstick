TEMPLATE = lib
CONFIG += dll plugin console
DEFINES += WATCHED_PATH=\'$$quote(\"/tmp/systemnotification\")\'
TARGET = fakesystemnotificationplugin

# Plugin developers should use the includes from the system directories
#INCLUDEPATH += /usr/include/duihome
#LIBS += -L/usr/lib -lduihome

# However, in order to compile this along with the homescreen package the local includes must be used
INCLUDEPATH += ../../libduihome
LIBS += -L../../lib -lduihome

HEADERS += fakesystemnotificationplugin.h
SOURCES += fakesystemnotificationplugin.cpp
target.path = /usr/lib/dui/notifications/systemnotificationplugins/ 
INSTALLS += target

