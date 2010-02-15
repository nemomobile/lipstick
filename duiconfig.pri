# Load more defines from the dui_defines...
load(dui_defines)
# duihome specific directories
unix {
     DUIHOME_DIR = $$DUI_INSTALL_DATA/duihome
     DUIHOME_THEMES_DIR = /usr/share/themes/base/dui/duihome

     TEST_SUITE_INSTALL_LOCATION=/usr/share/duifw-home-tests
     TEST_APP_INSTALL_LOCATION=/usr/lib/duifw-home-tests
     TEST_THEME_ROOT_LOCATION=/usr/share/themes/base/dui
}
mac {
     # Do mac stuff here
}
win32 {
     # Do win32 stuff here
}
