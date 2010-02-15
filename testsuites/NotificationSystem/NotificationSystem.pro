include(../../duiconfig.pri)
TEMPLATE=subdirs

suites.path=$$TEST_SUITE_INSTALL_LOCATION/NotificationSystem
suites.files=ts_notificationsystem.rb \

cases.path=$$TEST_SUITE_INSTALL_LOCATION/NotificationSystem/cases
cases.files=cases/tc_persistentnotifications.rb \
    cases/tc_notificationsystem.rb \
    cases/tc_eventtypestore.rb

INSTALLS += \
    cases \
    suites \

include(../check.pri)
