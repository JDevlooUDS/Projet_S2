#include "header/game.h"
#include "header/prisonScene.h"
#include <QApplication>
#include "header/sceneManager.h"
#include "header/resourceManager.h"
using namespace std;

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    Game game;
    game.resize(1220, 680); //1920, 1080
    SceneManager sceneManager(&game);
    sceneManager.setScene(Menu);
    game.show();
    return app.exec();
}