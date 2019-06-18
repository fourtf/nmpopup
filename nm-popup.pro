#-------------------------------------------------
#
# Project created by QtCreator 2019-06-17T19:30:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = nm-popup
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++14

SOURCES += \
        src/config.cpp \
        src/function-event-filter.cpp \
        src/main.cpp \
        src/main-window.cpp \
        src/make-widget.cpp \
        src/network-table-model.cpp \
        src/networking.cpp \
        src/theme.cpp

HEADERS += \
        src/config.hpp \
        src/function-event-filter.hpp \
        src/main-window.hpp \
        src/make-widget.hpp \
        src/network-table-model.hpp \
        src/networking.hpp \
        src/theme.hpp \
        src/util.hpp

RESOURCES += \
        resources/resources.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
