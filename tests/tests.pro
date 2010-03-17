TEMPLATE = subdirs
SUBDIRS = \
    ut_mainwindow \
    ut_home \
    ut_clock \
    ut_clockview \
    ut_desktop \
    ut_desktopview \
    ut_launcher \
    ut_launcherview \
    ut_launcherpage \
    ut_launcherpageview \
    ut_launcherbuttonview \
    ut_launcherdatastore \
    ut_statusarea \
    ut_statusareaview \
    ut_statusareawindow \
    ut_statusindicator \
    ut_switcher \
    ut_switcherview \
    ut_switcherbutton \
    ut_switcherbuttonview \
    ut_switcherphysicsintegrationstrategy \
    ut_quicklaunchbar \
    ut_quicklaunchbarview \
    ut_notificationparameter \
    ut_notificationparameters \
    ut_notification \
    ut_notificationgroup \
    ut_notificationmanager \
    ut_notificationarea \
    ut_notificationareaview \
    ut_duihomeshortcutmanager \
    ut_dbusinterfacenotificationsource \
    ut_duihomesystemnotificationpluginbase \
    ut_systemnotificationsource \
    ut_homenotificationsink \
    ut_notificationareasink \
    ut_widgetnotificationsink \
    ut_duicompositornotificationsink \
    ut_duifeedbacknotificationsink \
    ut_notifiernotificationsink \
    ut_notifier \
    ut_statusindicatorimageview \
    ut_statusindicatorlabelview \
    ut_homeapplication \
    ut_windowinfo \
    ut_x11helper \
    ut_eventtypestore \
    ut_appletspace \
    ut_appletspaceview \
    ut_pagedviewport \
    ut_test

QMAKE_STRIP = echo

check.target = check
check.CONFIG = recursive
QMAKE_EXTRA_TARGETS += check

check-xml.target = check-xml
check-xml.CONFIG = recursive
QMAKE_EXTRA_TARGETS += check-xml

support_files.commands += $$PWD/gen-tests-xml.sh > $$OUT_PWD/tests.xml
support_files.target = support_files
support_files.files += $$OUT_PWD/tests.xml
support_files.path = /usr/share/duihomescreen-tests
support_files.CONFIG += no_check_exist

INSTALLS += support_files

QMAKE_CLEAN += **/*.log.xml ./coverage.log.xml **/*.log

