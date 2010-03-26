# Load more defines from the dui_defines...
load(dui_defines)
# duihome specific directories
unix {
     DUIHOME_DIR = $$DUI_INSTALL_DATA/duihome
     DUIHOME_THEMES_DIR = /usr/share/themes/base/dui/duihome
     DUIHOME_SHADERS_DIR = $$DUIHOME_DIR/shaders

     TEST_SUITE_INSTALL_LOCATION=/usr/share/duifw-home-tests
     TEST_APP_INSTALL_LOCATION=/usr/lib/duifw-home-tests
     TEST_THEME_ROOT_LOCATION=/usr/share/themes/base/dui
     TEST_BENCHMARKS_INSTALL_LOCATION=/usr/share/duifw-home-benchmark-tests

     SUDOERS_DIR = /etc/sudoers.d
}
mac {
     # Do mac stuff here
}
win32 {
     # Do win32 stuff here
}
