#include "baseScene.h"
#include "menuButton.h"
#include "QGraphicsView"
#include "QApplication"
#include <vector>
#include "audioManager.h"
#include <QGraphicsProxyWidget>
#include <QSlider>
#include "resourceManager.h"
#include <QLineEdit>
#include <QKeyEvent>
#include "game.h"
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
	void drawBackground(QPainter* painter, const QRectF& rect) override;
	void keyPressEvent(QKeyEvent* event) override;

private:
	void playClicked();
	void tutoClicked();
	void leaderboardClicked();
	void settingsClicked();
	void exitClicked();
	void showNamePrompt();
	void onNameConfirmed();
	void cleanNamePrompt();

	MenuButton* play = nullptr;
	MenuButton* tutorial = nullptr;
	MenuButton* leaderboard = nullptr;
	MenuButton* settings = nullptr;
	MenuButton* exit = nullptr;
	MenuButton* back = nullptr;
	QGraphicsProxyWidget* slider = nullptr;
	MenuButton* returnButton = nullptr;

	vector<MenuButton*> buttons;
	vector<MenuButton*>::iterator it;

	MenuButton* selectedButton = nullptr;

	double selectTimer = 0.0;
	const double SELECT_SPEED = 0.1;
	double changeSelectTimer = 0.0;
	const double CHANGE_SELECT_LIMIT = 0.2;

	bool toggleSettings = false;

	QPixmap background;

	QGraphicsProxyWidget* nameInputProxy;
	QLineEdit* nameEdit;
	MenuButton* confirmNameBtn;
	QGraphicsRectItem* overlay;
	bool showingNamePrompt = false;

};