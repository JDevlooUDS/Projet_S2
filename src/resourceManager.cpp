#include "../header/resourceManager.h"
#include <QFile>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QByteArray>
#include <QDirIterator>

ResourceManager::ResourceManager() {}
ResourceManager::~ResourceManager() {}

ResourceManager& ResourceManager::getInstance() {
	static ResourceManager instance;
	return instance;
}

bool ResourceManager::loadPrisonSceneResources() {
	playerSprite.load(":/sprites/placeHolderSprite.png");
	if (playerSprite.isNull()) return false;

	QPixmap tileSet(":/sprites/tilesets.png");
	int height = tileSet.height() / TILE_SIZE;
	int width = tileSet.width() / TILE_SIZE;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			QPixmap tile = tileSet.copy(j * TILE_SIZE,i * TILE_SIZE, TILE_SIZE, TILE_SIZE);
			if (tile.isNull()) {
				qDebug() << "failed to load tile";
				return false;
			}
			tiles.push_back(tile);
		}
	}

	QFile file(":/map/test_map.json");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return false;
	}
	else {
		QByteArray rawData = file.readAll();
		file.close();

		QJsonDocument doc = QJsonDocument::fromJson(rawData);
		if (!doc.isNull()) {
			QJsonObject jsonObject = doc.object();
			QJsonArray array = jsonObject.value("layers").toArray();
			if (array.isEmpty()) {
				qDebug() << "failed to load layers";
				return false;
			}
			for (int i = 0; i < array.size(); i++) {
				QJsonObject layer = array[i].toObject();

				bool collides = false;
				QJsonArray properties = layer.value("properties").toArray();
				for (int j = 0; j < properties.size(); j++) {
					QJsonObject property = properties[j].toObject();
					if (property.value("name").toString() == "collide") {
						collides = property.value("value").toBool();
					}
				}
				
				QJsonArray layerArray = layer.value("objects").toArray();
				for (int j = 0; j < layerArray.size(); j++) {
					QJsonObject item = layerArray[j].toObject();
					Tile tile(item.value("x").toInt(),item.value("y").toInt(),item.value("height").toInt(),item.value("width").toInt(),collides,tiles[item.value("gid").toInt()]);
					prisonMap.push_back(tile);
				}
			}
		}
		else {
			return false;
		}
	}

	return true;
}

QPixmap& ResourceManager::getPlayerSprite() {
	return playerSprite;
}

vector<Tile> ResourceManager::getTiles() {
	return prisonMap;
}