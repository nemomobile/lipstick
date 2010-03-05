include(../../duiconfig.pri)
TEMPLATE=subdirs

INSTALL_ROOT=$$TEST_SUITE_INSTALL_LOCATION/acceptancetests

rakefile.path=$$INSTALL_ROOT
rakefile.files=rakefile

features.path=$$INSTALL_ROOT/features
features.files=features/*.feature

step_definitions.path=$$INSTALL_ROOT/features/step_definitions
step_definitions.files=features/step_definitions/*.rb

INSTALLS += \
    rakefile \
    features \
    step_definitions

include(../check.pri)
