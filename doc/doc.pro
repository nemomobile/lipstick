# Stupid qmake wants to link everything.
QMAKE_LINK = @: IGNORE THIS LINE

QMAKE_EXTRA_TARGETS += doc notification_doc
QMAKE_EXTRA_COMPILER += doc notification_doc

doc.target        = html/index.html
doc.config       += no_check_exist no_link explicit_dependencies
doc.commands      = doxygen doxygen.cfg
doc.input         = $${doc.depends}
doc.output        = $${doc.target}

notification_doc.target        = notifications/html/index.html
notification_doc.config       += no_check_exist no_link explicit_dependencies
notification_doc.commands      = doxygen notifications/doxygen.cfg
notification_doc.input         = $${notification_doc.depends}
notification_doc.output        = $${notification_doc.target}

## Make are doc target build by default If 'make' is executed in doc folder
PRE_TARGETDEPS += $${doc.output} $${notification_doc.output}

# Install rules
htmldocs.files = html/*
htmldocs.path = /usr/share/doc/lipstick
htmldocs.CONFIG += no_check_exist

notification_htmldocs.files = notifications/html/*
notification_htmldocs.path = /usr/share/doc/lipstick-notification
notification_htmldocs.CONFIG += no_check_exist

INSTALLS += htmldocs notification_htmldocs
