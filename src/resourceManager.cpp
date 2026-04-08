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
	if (!loadPlayerSprites()) return false;
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

	QFile file(":/map/test_map3.json");
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
				QString name = layer.value("name").toString();

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
					Tile tile(item.value("x").toInt(),item.value("y").toInt(),item.value("height").toInt(),item.value("width").toInt(),collides,tiles[item.value("gid").toInt() - 1], name);
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

bool ResourceManager::loadPlayerSprites() {
	QPixmap sheet;
	sheet.load(":/sprites/Run_Voltron.png");

	if (sheet.isNull()) return false;

	int frameQuantity = sheet.width() / 48;

	for (int i = 0; i < frameQuantity; i++) {
		QPixmap frame = sheet.copy(i * 48, 0, 48,48);
		runAnimation.push_back(frame);
	}

	sheet.load(":/sprites/dash.png");
	if (sheet.isNull()) return false;

	frameQuantity = sheet.width() / 48;

	for (int i = 0; i < frameQuantity; i++) {
		QPixmap frame = sheet.copy(i * 48, 0, 48, 48);
		dashAnimation.push_back(frame);
	}

	sheet.load(":/sprites/jump.png");
	if (sheet.isNull()) return false;

	frameQuantity = sheet.width() / 48;

	for (int i = 0; i < frameQuantity; i++) {
		QPixmap frame = sheet.copy(i * 48, 0, 48, 48);
		jumpAnimation.push_back(frame);
	}

	sheet.load(":/sprites/idle.png");
	if (sheet.isNull()) return false;

	frameQuantity = sheet.width() / 48;

	for (int i = 0; i < frameQuantity; i++) {
		QPixmap frame = sheet.copy(i * 48, 0, 48, 48);
		idleAnimation.push_back(frame);
	}

	return true;
}

vector<Tile> ResourceManager::getTiles() {
	return prisonMap;
}

vector<QPixmap> ResourceManager::getRunAnimation() {
	
	return runAnimation;
}
vector<QPixmap> ResourceManager::getJumpAnimation() {
	vector<QPixmap> sprites;
	return jumpAnimation;
}

vector<QPixmap> ResourceManager::getIdleAnimation() {
	vector<QPixmap> sprites;
	return idleAnimation;
}

vector<QPixmap> ResourceManager::getDashAnimation() {
	vector<QPixmap> sprites;
	return dashAnimation;
}