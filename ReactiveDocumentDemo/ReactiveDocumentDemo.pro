TEMPLATE = app

QT += qml quick

SOURCES += main.cpp \
    reactivetexthelper.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

CONFIG += c++11

# QMAKE_CXXFLAGS += -std=c++11  -stdlib=libc++

# Default rules for deployment.
include(deployment.pri)

DISTFILES +=

HEADERS += \
    reactivetexthelper.h
