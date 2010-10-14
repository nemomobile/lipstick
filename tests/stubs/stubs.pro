TEMPLATE = subdirs

INSTALL_HEADERS += \
  applicationpackagemonitorlistener_stub.h \
  applicationpackagemonitor_stub.h \
  desktop_stub.h \
  desktopview_stub.h \
  homeapplication_stub.h \
  homewindowmonitor_stub.h \
  launcheraction_stub.h \
  launcherbuttonprogressindicator_stub.h \
  launcherbutton_stub.h \
  launcherdatastore_stub.h \
  launcher_stub.h \
  launcherview_stub.h \
  mainwindow_stub.h \
  mapplication_stub.h \
  methodcall.h \
  mfeedback_stub.h \
  mockdatastore.h \
  mprogressindicator_stub.h \
  mscenemanager_stub.h \
  mwindow_stub.h \
  pagedviewport_stub.h \
  parameter.h \
  quicklaunchbar_stub.h \
  stubbase.cpp \
  stubbase.h \
  switcher_stub.h \
  switcherview_stub.h \
  testcontextitem.cpp \
  testcontextitem.h \
  transformlayoutanimation_stub.h \
  windowinfo_stub.h \
  x11wrapper_stub.h \
  xeventlistener_stub.h

# Input
headers.path += /usr/include/meegotouchhome/stubs
headers.files += $$INSTALL_HEADERS
INSTALLS += headers
