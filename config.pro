TEMPLATE = vcapp
TARGET = Jeu
CONFIG += warn_on qt debug windows console
HEADERS += ./header/gameObject.h ./header/game.h
SOURCES += main.cpp ./src/gameObject.cpp ./src/game.cpp
INCLUDEPATH += ./header ./src