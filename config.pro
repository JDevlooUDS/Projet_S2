TEMPLATE = vcapp
TARGET = Jeu
CONFIG += warn_on qt debug windows console
HEADERS += ./header/gameObject.h
SOURCES += main.cpp ./src/gameObject.cpp
INCLUDEPATH += ./header ./src