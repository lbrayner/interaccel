TEMPLATE = app
CONFIG += console
CONFIG += qt
QT     -= gui
HEADERS += accel.h
HEADERS += interception.h
HEADERS += settings.h
HEADERS += utils.h
SOURCES += accel.cpp
SOURCES += main.cpp
SOURCES += settings.cpp
SOURCES += utils.cpp
unix:LIBS += -L libs/x64 -l interception
TARGET = accel
target.path = /
INSTALLS += target
