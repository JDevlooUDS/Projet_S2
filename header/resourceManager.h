#pragma once
#include <QPixmap>
#include <vector>
#include "tile.h"
using std::vector;

class ResourceManager {


public:
	static ResourceManager& getInstance();
	bool loadPrisonSceneResources(QString path);
	bool loadMenuResources();
	vector<Tile> getTiles();
	vector<QPixmap> getRunAnimation();
	vector<QPixmap> getJumpAnimation();
	vector<QPixmap> getIdleAnimation();
	vector<QPixmap> getDashAnimation();
	QPixmap getFilledhealth();
	QPixmap getEmptyHealth();
	vector<QPixmap> getFallingStarAnimation();
	QPixmap getBackground();


	QPointF playerSpawnPoint;

private:
	ResourceManager();
	~ResourceManager();
	bool loadPlayerSprites();
	bool loadMap(QString path);

	const int TILE_SIZE = 32;
	QPixmap playerSprite;
	vector<Tile> prisonMap;
	vector<Tile> tutorialMap;
	vector<QPixmap> tiles;
	vector<QPixmap> runAnimation;
	vector<QPixmap> dashAnimation;
	vector<QPixmap> jumpAnimation;
	vector<QPixmap> idleAnimation;
	vector<QPixmap> fallingStarAnimation;
	QPixmap filledHealth;
	QPixmap emptyHealth;

	QPixmap background;
};