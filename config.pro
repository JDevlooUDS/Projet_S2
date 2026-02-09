TEMPLATE = vcapp
TARGET = Jeu
CONFIG += warn_on qt debug windows console
HEADERS += ./header/gameObject.h ./header/game.h ./header/player.h ./header/inputs.h
SOURCES += main.cpp ./src/gameObject.cpp ./src/game.cpp ./src/player.cpp
INCLUDEPATH += ./header ./src ./map ./image