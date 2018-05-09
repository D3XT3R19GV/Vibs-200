TEMPLATE = app
CONFIG += console c++
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

unix:!macx: LIBS += -L$$PWD/../systemc-2.3.2/lib-linux64/ -lsystemc

INCLUDEPATH += $$PWD/../systemc-2.3.2/include
DEPENDPATH += $$PWD/../systemc-2.3.2/include

unix:!macx: PRE_TARGETDEPS += $$PWD/../systemc-2.3.2/lib-linux64/libsystemc.a

HEADERS += \
    Include/regs.H
