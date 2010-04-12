TEMPLATE = subdirs
SUBDIRS = \
    ut_appletspace \
    ut_appletspaceview \
    ut_desktop \
    ut_desktopview \
    ut_home \
    ut_homeapplication \
    ut_launcher \
    ut_launcherdatastore \
    ut_launcherpage \
    ut_launcherpageview \
    ut_launcherview \
    ut_mainwindow \
    ut_pagedpanning \
    ut_pagedviewport \
    ut_plaindesktopbackgroundextension \
    ut_plaindesktopbackgroundpixmap \
    ut_quicklaunchbar \
    ut_quicklaunchbarview \
    ut_statusindicator \
    ut_statusindicatorlabelview \
    ut_switcher \
    ut_switcherbutton \
    ut_switcherbuttonview \
    ut_switcherview \
    ut_windowinfo \
    ut_x11helper \
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

