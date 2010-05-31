QMAKE_EXTRA_TARGETS += check
# check.depends = $$QMAKE_TARGET
check.commands = $$system(true)

QMAKE_EXTRA_TARGETS += check-xml
# check-xml.depends = $$QMAKE_TARGET
check-xml.commands = $$system(true)

