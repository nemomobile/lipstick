defineReplace(srcList) {
    LIST=$$1
    for(item,LIST) {
        ITEMLIST=$$item,$${ITEMLIST}
    }
    return($${ITEMLIST})
}

QMAKE_CXXFLAGS += -ftest-coverage -fprofile-arcs
LIBS += -lgcov

# OBJECTS_DIR = .obj
MOC_DIR = .moc

QMAKE_EXTRA_TARGETS += coverage
coverage.depends = check-xml
coverage.commands = @../coverage.py $$srcList($$TEST_SOURCES) .obj 90


QMAKE_EXTRA_TARGETS += coverage-xml
coverage-xml.depends = check-xml
coverage-xml.commands = @../coverage.py $$srcList($$TEST_SOURCES) .obj 90


QMAKE_CLEAN += *.gcda *.gcno *.gcov *.log *.xml ./.moc/*
QMAKE_DISTCLEAN += *.gcda *.gcno *.gcov *.log *.xml ./.moc/*
