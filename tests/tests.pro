TEMPLATE = subdirs
SUBDIRS = ut_categorydefinitionstore ut_notification ut_notificationmanager ut_notificationlistmodel ut_lipsticksettings ut_notificationpreviewpresenter ut_closeeventeater ut_eventeater ut_screenlock ut_volumecontrol ut_notificationfeedbackplayer ut_batterynotifier ut_lowbatterynotifier ut_usbmodeselector ut_shutdownscreen

support_files.commands += $$PWD/gen-tests-xml.sh > $$OUT_PWD/tests.xml
support_files.target = support_files
support_files.files += $$OUT_PWD/tests.xml
support_files.path = /opt/tests/lipstick-tests/test-definition
support_files.CONFIG += no_check_exist

INSTALLS += support_files
