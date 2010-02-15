include(../../duiconfig.pri)
TEMPLATE=subdirs

suites.path=$$TEST_SUITE_INSTALL_LOCATION/HomeMashupCategories
suites.files=ts_homemashupcategories.rb \

cases.path=$$TEST_SUITE_INSTALL_LOCATION/HomeMashupCategories/cases
cases.files=cases/tc_homemashupcategories.rb \

INSTALLS += \
    cases \
    suites \

include(../check.pri)
