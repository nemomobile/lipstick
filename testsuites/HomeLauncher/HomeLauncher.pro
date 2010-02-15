include(../../duiconfig.pri)
TEMPLATE=subdirs

suites.path=$$TEST_SUITE_INSTALL_LOCATION/HomeLauncher
suites.files=ts_homelauncher.rb \

cases.path=$$TEST_SUITE_INSTALL_LOCATION/HomeLauncher/cases
cases.files=cases/tc_homelauncher.rb \

INSTALLS += \
    cases \
    suites \

include(../check.pri)
