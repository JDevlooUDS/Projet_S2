#include "baseScene.h"
#include "menuButton.h"
#include "QGraphicsView"
#include "QApplication"

class MainMenu : public BaseScene {
public:
	MainMenu();
	~MainMenu();
	void init();
	void updateScene(double deltaTime, const Inputs& inputs) override;
protected:

private:
	void playClicked();
	void tutoClicked();
	void leaderboardClicked();
	void settingsClicked();
	void exitClicked();

	MenuButton* play = nullptr;
	MenuButton* tutorial = nullptr;
	MenuButton* leaderboard = nullptr;
	MenuButton* settings = nullptr;
	MenuButton* exit;

};