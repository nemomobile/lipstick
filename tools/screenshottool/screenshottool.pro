TEMPLATE = subdirs
INSTALLS += screenshottool desktop

screenshottool.files = screenshottool
screenshottool.path = /usr/bin
desktop.files = screenshottool.desktop
desktop.path = /usr/share/applications
