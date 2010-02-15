include(../../duiconfig.pri)
TEMPLATE=subdirs

suites.path=$$TEST_SUITE_INSTALL_LOCATION/HomeSwitcher
suites.files=ts_homeswitcher.rb \

cases.path=$$TEST_SUITE_INSTALL_LOCATION/HomeSwitcher/cases
cases.files=cases/tc_homeswitcher.rb \

INSTALLS += \
    cases \
    suites \

include(../check.pri)
