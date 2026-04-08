#pragma once
#include <QPixmap>
#include <vector>
#include "tile.h"
using std::vector;

class ResourceManager {


public:
	static ResourceManager& getInstance();
	bool loadPrisonSceneResources();
	vector<Tile> getTiles();
	vector<QPixmap> getRunAnimation();
	vector<QPixmap> getJumpAnimation();
	vector<QPixmap> getIdleAnimation();
	vector<QPixmap> getDashAnimation();
	QPixmap getFilledhealth();
	QPixmap getEmptyHealth();

private:
	ResourceManager();
	~ResourceManager();
	bool loadPlayerSprites();

	const int TILE_SIZE = 32;
	QPixmap playerSprite;
	vector<Tile> prisonMap;
	vector<QPixmap> tiles;
	vector<QPixmap> runAnimation;
	vector<QPixmap> dashAnimation;
	vector<QPixmap> jumpAnimation;
	vector<QPixmap> idleAnimation;
	QPixmap filledHealth;
	QPixmap emptyHealth;

};