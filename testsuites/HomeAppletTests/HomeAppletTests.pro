include(../../duiconfig.pri)
TEMPLATE=subdirs

suites.path=$$TEST_SUITE_INSTALL_LOCATION/HomeAppletTests
suites.files=ts_homeapplettests.rb \

cases.path=$$TEST_SUITE_INSTALL_LOCATION/HomeAppletTests/cases
cases.files=cases/tc_*.rb \

INSTALLS += \
    cases \
    suites \

include(../check.pri)
