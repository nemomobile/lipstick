
# Input
HEADERS += homeapplication.h \
    windowinfo.h \
    home.h \
    statusarea.h \
    statusareaview.h \
    statusareamodel.h \
    statusareastyle.h \
    statusareawindow.h \
    appletspace.h \
    appletspaceview.h \
    appletspacemodel.h \
    appletspacestyle.h \
    clock.h \
    clockmodel.h \
    clockview.h \
    clockstyle.h \
    desktop.h \
    desktopmodel.h \
    desktopview.h \
    desktopstyle.h \
    mainwindow.h \
    launcher.h \
    launcherview.h \
    launchermodel.h \
    launcherstyle.h \
    launcherbutton.h \
    launcherbuttonmodel.h \
    launcherbuttonview.h \
    launcherbuttonstyle.h \
    notifier.h \
    switcher.h \
    switchermodel.h \
    switcherview.h \
    switcherstyle.h \
    switcherbutton.h \
    switcherbuttonmodel.h \
    switcherbuttonview.h \
    switcherbuttonswview.h \
    switcherbuttonglesview.h \
    switcherbuttonstyle.h \
    switcherphysicsintegrationstrategy.h \
    notificationarea.h \
    notificationareaview.h \
    notificationareamodel.h \
    notificationareastyle.h \
    applicationcontext.h \
    contextframeworkcontext.h \
    statusindicator.h \
    statusindicatormodel.h \
    statusindicatorimageview.h \
    statusindicatorlabelview.h \
    statusindicatorimagestyle.h \
    statusindicatorlabelstyle.h \
    x11helper.h \
    x11wrapper.h
SOURCES += homeapplication.cpp \
    windowinfo.cpp \
    home.cpp \
    statusarea.cpp \
    statusareaview.cpp \
    statusareawindow.cpp \
    appletspace.cpp \
    appletspaceview.cpp \
    clock.cpp \
    clockview.cpp \
    desktop.cpp \
    desktopview.cpp \
    mainwindow.cpp \
    launcher.cpp \
    launcherview.cpp \
    launcherbutton.cpp \
    launcherbuttonview.cpp \
    notifier.cpp \
    switcher.cpp \
    switcherview.cpp \
    switcherbutton.cpp \
    switcherbuttonview.cpp \
    switcherbuttonswview.cpp \
    switcherbuttonglesview.cpp \
    switcherphysicsintegrationstrategy.cpp \
    notificationarea.cpp \
    notificationareaview.cpp \
    contextframeworkcontext.cpp \
    statusindicator.cpp \
    statusindicatorimageview.cpp \
    statusindicatorlabelview.cpp \
    x11helper.cpp \
    x11wrapper.cpp
MODEL_HEADERS += \
    appletspacemodel.h \
    clockmodel.h \
    desktopmodel.h \
    launcherbuttonmodel.h \
    launchermodel.h \
    notificationareamodel.h \
    statusareamodel.h \
    switcherbuttonmodel.h \
    switchermodel.h \
    statusindicatormodel.h
STYLE_HEADERS += \
    appletspacestyle.h \
    clockstyle.h \
    desktopstyle.h \
    launcherstyle.h \
    launcherbuttonstyle.h \
    notificationareastyle.h \
    statusareastyle.h \
    switcherbuttonstyle.h \
    switcherstyle.h \
    statusindicatorimagestyle.h \
    statusindicatorlabelstyle.h

include(notifications/notifications.pri)
