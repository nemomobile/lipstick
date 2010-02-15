include(../../duiconfig.pri)
TEMPLATE=subdirs

suites.path=$$TEST_SUITE_INSTALL_LOCATION/HomeStatusIndicatorMenu
suites.files=ts_homestatusindicatormenu.rb \

cases.path=$$TEST_SUITE_INSTALL_LOCATION/HomeStatusIndicatorMenu/cases
cases.files=cases/tc_homestatusindicatormenu.rb \

INSTALLS += \
    cases \
    suites \

include(../check.pri)
