include(../../duiconfig.pri)
TEMPLATE=subdirs

suites.path=$$TEST_SUITE_INSTALL_LOCATION/HomeStartup
suites.files=ts_homestartup.rb \

cases.path=$$TEST_SUITE_INSTALL_LOCATION/HomeStartup/cases
cases.files=cases/tc_homestartup.rb \

INSTALLS += \
    cases \
    suites \

include(../check.pri)
