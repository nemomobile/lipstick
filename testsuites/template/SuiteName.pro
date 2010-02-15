include(../../duiconfig.pri)
TEMPLATE=subdirs

suites.path=$$TEST_SUITE_INSTALL_LOCATION/SuiteName
suites.files=ts_suitename.rb \

cases.path=$$TEST_SUITE_INSTALL_LOCATION/SuiteName/cases
cases.files=cases/tc_suitename.rb \

INSTALLS += \
    cases \
    suites \

include(../check.pri)
