TEMPLATE = vcapp
TARGET = Jeu

SOUNDS = $$files($$PWD/resources/sounds/sfx/*.wav)
MAPS = $$files($$PWD/resources/map/*.json)
SPRITES = $$files($$PWD/resources/sprites/*png)

resources.base = resources
resources.files += $$SOUNDS $$MUSICS $$MAPS $$SPRITES
CONFIG += warn_on qt debug windows console resources_big
HEADERS += $$files($$PWD/header/*.h)

SOURCES += main.cpp $$files($$PWD/src/*.cpp)
INCLUDEPATH += ./header ./src ./map ./image


music_files.path = $$OUT_PWD/resources/sounds/music
music_files.files = $$files($$PWD/resources/sounds/music/*.wav)

INSTALLS += music_files

MY_SOURCE = $$PWD/resources
MY_DEST = $$OUT_PWD/debug/resources

QMAKE_POST_LINK += xcopy \"$$MY_SOURCE\" \"$$MY_DEST\" /E /I /Y /D


RESOURCES += resources
QT += widgets serialport multimedia
