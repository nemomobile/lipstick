# Input
include(eventtypes/eventtypes.pri)
HOME_NOTIFICATIONS_SRC_DIR = $$HOME_SRC_DIR/notifications
INCLUDEPATH += $$HOME_SRC_DIR/notifications $$HOME_SRC_DIR/../libduihome
HEADERS += $$HOME_NOTIFICATIONS_SRC_DIR/notificationmanagerinterface.h \
    $$HOME_NOTIFICATIONS_SRC_DIR/notificationmanager.h \
    $$HOME_NOTIFICATIONS_SRC_DIR/notificationparameter.h \
    $$HOME_NOTIFICATIONS_SRC_DIR/notificationparameters.h \
    $$HOME_NOTIFICATIONS_SRC_DIR/notification.h \
    $$HOME_NOTIFICATIONS_SRC_DIR/notificationgroup.h \
    $$HOME_NOTIFICATIONS_SRC_DIR/notificationsource.h \
    $$HOME_NOTIFICATIONS_SRC_DIR/dbusinterfacenotificationsource.h \
    $$HOME_NOTIFICATIONS_SRC_DIR/dbusinterfacenotificationsourceadaptor.h \
    $$HOME_NOTIFICATIONS_SRC_DIR/notificationsink.h \
    $$HOME_NOTIFICATIONS_SRC_DIR/notificationareasink.h \
    $$HOME_NOTIFICATIONS_SRC_DIR/widgetnotificationsink.h \
    $$HOME_NOTIFICATIONS_SRC_DIR/homenotificationsink.h \
    $$HOME_NOTIFICATIONS_SRC_DIR/duicompositornotificationsink.h \
    $$HOME_NOTIFICATIONS_SRC_DIR/duifeedbacknotificationsink.h \
    $$HOME_NOTIFICATIONS_SRC_DIR/notifiernotificationsink.h \
    $$HOME_NOTIFICATIONS_SRC_DIR/eventtypestore.h \
    $$HOME_NOTIFICATIONS_SRC_DIR/systemnotificationsource.h 
SOURCES += $$HOME_NOTIFICATIONS_SRC_DIR/notificationmanager.cpp \
    $$HOME_NOTIFICATIONS_SRC_DIR/notificationparameter.cpp \
    $$HOME_NOTIFICATIONS_SRC_DIR/notificationparameters.cpp \
    $$HOME_NOTIFICATIONS_SRC_DIR/notification.cpp \
    $$HOME_NOTIFICATIONS_SRC_DIR/notificationgroup.cpp \
    $$HOME_NOTIFICATIONS_SRC_DIR/notificationsource.cpp \
    $$HOME_NOTIFICATIONS_SRC_DIR/dbusinterfacenotificationsource.cpp \
    $$HOME_NOTIFICATIONS_SRC_DIR/dbusinterfacenotificationsourceadaptor.cpp \
    $$HOME_NOTIFICATIONS_SRC_DIR/notificationsink.cpp \
    $$HOME_NOTIFICATIONS_SRC_DIR/notificationareasink.cpp \
    $$HOME_NOTIFICATIONS_SRC_DIR/widgetnotificationsink.cpp \
    $$HOME_NOTIFICATIONS_SRC_DIR/homenotificationsink.cpp \
    $$HOME_NOTIFICATIONS_SRC_DIR/duicompositornotificationsink.cpp \
    $$HOME_NOTIFICATIONS_SRC_DIR/duifeedbacknotificationsink.cpp \
    $$HOME_NOTIFICATIONS_SRC_DIR/notifiernotificationsink.cpp \
    $$HOME_NOTIFICATIONS_SRC_DIR/eventtypestore.cpp \
    $$HOME_NOTIFICATIONS_SRC_DIR/systemnotificationsource.cpp
