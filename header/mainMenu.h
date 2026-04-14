#include "baseScene.h"
#include "menuButton.h"
#include "QGraphicsView"
#include "QApplication"
#include <vector>
#include "audioManager.h"
#include <QGraphicsProxyWidget>
#include <QSlider>
using std::vector;

class MainMenu : public BaseScene {
	Q_OBJECT
public:
	MainMenu();
	~MainMenu();
	void init();
	void updateScene(double deltaTime, const Inputs& inputs) override;
signals:
	void setVolume(int volume);
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
	MenuButton* exit = nullptr;
	MenuButton* back = nullptr;
	QGraphicsProxyWidget* slider = nullptr;

	vector<MenuButton*> buttons;
	vector<MenuButton*>::iterator it;

	MenuButton* selectedButton = nullptr;

	double selectTimer = 0.0;
	const double SELECT_SPEED = 0.08;
	double changeSelectTimer = 0.0;
	const double CHANGE_SELECT_LIMIT = 0.2;

	bool toggleSettings = false;

};