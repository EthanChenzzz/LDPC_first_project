TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        channel.cpp \
        decoder.cpp \
        encoder.cpp \
        ldpc.cpp \
        main.cpp \
        stat_ldpc.cpp

HEADERS += \
    channel.h \
    decoder.h \
    encoder.h \
    ldpc.h \
    stat_ldpc.h

