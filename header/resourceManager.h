#pragma once
#include <QPixmap>
#include <vector>
#include "tile.h"
using std::vector;

class ResourceManager {


public:
	static ResourceManager& getInstance();
	bool loadPrisonSceneResources();
	QPixmap& getPlayerSprite();
	vector<Tile> getTiles();

private:
	ResourceManager();
	~ResourceManager();

	const int TILE_SIZE = 32;
	QPixmap playerSprite;
	vector<Tile> prisonMap;
	vector<QPixmap> tiles;
};