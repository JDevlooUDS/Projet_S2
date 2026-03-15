#include "header/game.h"
#include "header/prisonScene.h"
#include <QApplication>
#include "header/sceneManager.h"
#include "header/resourceManager.h"
using namespace std;

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    Game game;
    game.resize(800, 600);
    ResourceManager resourceManager;
    SceneManager sceneManager(&game);
    sceneManager.setScene(Prison);
    game.show();
    return app.exec();
}