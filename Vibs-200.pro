TEMPLATE = app
CONFIG += console c++
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += Include

SOURCES += main.cpp \
    Src/testbench.cpp \

HEADERS += Include/regs.H \
           Include/alu.H \
           Include/instruction_memory.h \
           Include/testbench.h \
           Include/register_file.h \
           Include/data_memory.h \
    Include/vibs200.h

unix:!macx: LIBS += -L$$PWD/../systemc-2.3.2/lib-linux64/ -lsystemc

INCLUDEPATH += $$PWD/../systemc-2.3.2/include
DEPENDPATH += $$PWD/../systemc-2.3.2/include

unix:!macx: PRE_TARGETDEPS += $$PWD/../systemc-2.3.2/lib-linux64/libsystemc.a
