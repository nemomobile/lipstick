TEMPLATE = subdirs
SUBDIRS = \
          ut_batterynotifier \
          ut_categorydefinitionstore \
          ut_closeeventeater \
          ut_devicelock \
          ut_diskspacenotifier \
          ut_launchermodel \
          ut_lipsticksettings \
          ut_lowbatterynotifier \
          ut_lipsticknotification \
          ut_notificationfeedbackplayer \
          ut_notificationlistmodel \
          ut_notificationmanager \
          ut_notificationpreviewpresenter \
          ut_qobjectlistmodel \
          ut_screenlock \
          ut_shutdownscreen \
          ut_thermalnotifier \
          ut_usbmodeselector \
          ut_volumecontrol \

support_files.commands += $$PWD/gen-tests-xml.sh > $$OUT_PWD/tests.xml
support_files.target = support_files
support_files.files += $$OUT_PWD/tests.xml
support_files.path = /opt/tests/lipstick-tests/test-definition
support_files.CONFIG += no_check_exist

INSTALLS += support_files
