TEMPLATE = subdirs
SUBDIRS = \
    stubs \
    ut_desktop \
    ut_desktopview \
    ut_extendedbuttonview \
    ut_home \
    ut_homeapplication \
    ut_homescreenservice \
    ut_homewindowmonitor \
    ut_launcher \
    ut_launcherbutton \
    ut_launcherbuttonview \
    ut_launcherdatastore \
    ut_launcherpage \
    ut_launcherpageview \
    ut_launcherview \
    ut_layoutvisualizationwrapper \
    ut_mainwindow \
    ut_pagedpanning \
    ut_pagedviewport \
    ut_panningwatcher \
    ut_plaindesktopbackgroundextension \
    ut_plaindesktopbackgroundpixmap \
    ut_quicklaunchbar \
    ut_quicklaunchbarview \
    ut_switcher \
    ut_switcherbutton \
    ut_switcherbuttonview \
    ut_switcherbuttonwithtitlebarview \
    ut_switcherview \
    ut_transformlayoutanimation \
    ut_windowinfo \
    ut_pagepositionindicatorview \
    ut_applicationpackagemonitor \
    ut_applicationpackagemonitorlistener \
    ut_xeventlistener \
    ut_test \
    ut_launcherbuttonprogressindicatorview

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
support_files.path = /usr/share/meegotouchhome-tests
support_files.CONFIG += no_check_exist

INSTALLS += support_files

QMAKE_CLEAN += **/*.log.xml ./coverage.log.xml **/*.log

