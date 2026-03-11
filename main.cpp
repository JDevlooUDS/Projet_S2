#include "header/game.h"
#include "header/prisonScene.h"
#include <QApplication>
using namespace std;

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    PrisonScene scene;
    Game game(&scene);
    game.resize(800, 600);
    game.show();
    return app.exec();
}