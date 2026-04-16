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

bool ResourceManager::loadPrisonSceneResources(QString path) {
	tiles.clear();
	prisonMap.clear();
	runAnimation.clear();
	dashAnimation.clear();
	jumpAnimation.clear();
	idleAnimation.clear();

	if (!loadPlayerSprites()) return false;
	playerSprite.load(":/sprites/placeHolderSprite.png");
	if (playerSprite.isNull()) return false;

	QPixmap health;
	health.load(":/sprites/health.png");
	if (health.isNull()) return false;
	filledHealth = health.copy(0,0, TILE_SIZE, TILE_SIZE);
	emptyHealth = health.copy(TILE_SIZE,0, TILE_SIZE, TILE_SIZE);
	if (filledHealth.isNull() || emptyHealth.isNull()) return false;

	auto loadTileSet = [&](const QString& path) -> bool {
		QPixmap tileSet(path);
		if (tileSet.isNull()) return false;
		int h = tileSet.height() / TILE_SIZE;
		int w = tileSet.width() / TILE_SIZE;
		for (int i = 0; i < h; i++)
			for (int j = 0; j < w; j++)
				tiles.push_back(tileSet.copy(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE));
		return true;
	};
	if (!loadTileSet(":/sprites/tilesets.png")) return false;  // jutilise pu mais imp pour avoir bon affichage
	if (!loadTileSet(":/sprites/tileset_b.png")) return false;

	//QFile file(":/map/tuto.json");
	//QFile file("C:/Users/capro/OneDrive - USherbrooke/S2/P3/JeuV6/resources/map/compet.json");
	QFile file(":/map/compet.json");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return false;
	}
	else {
		QByteArray rawData = file.readAll();
		file.close();

		QJsonDocument doc = QJsonDocument::fromJson(rawData);
		if (!doc.isNull()) {
			QJsonObject jsonObject = doc.object();

			int firstgid = 1; // tilesets.png commence à GID 1, tileset_b à GID 9

			QJsonArray array = jsonObject.value("layers").toArray();
			if (array.isEmpty()) {
				qDebug() << "failed to load layers";
				return false;
			}
			int mapWidth = jsonObject.value("width").toInt();
			for (int i = 0; i < array.size(); i++) {
				QJsonObject layer = array[i].toObject();
				QString name = layer.value("name").toString();
				QString type = layer.value("type").toString();

				if (name == "player") {
					QJsonArray objects = layer.value("objects").toArray();
					if (!objects.isEmpty()) {
						QJsonObject obj = objects[0].toObject();
						playerSpawnPoint = QPointF(
							obj.value("x").toDouble(),
							obj.value("y").toDouble()
						);
					}
					continue;
				}

				bool collides = false;
				QJsonArray properties = layer.value("properties").toArray();
				for (int j = 0; j < properties.size(); j++) {
					QJsonObject property = properties[j].toObject();
					if (property.value("name").toString() == "collide") {
						collides = property.value("value").toBool();
					}
				}

				if (type == "tilelayer") {
					QString encoding = layer.value("encoding").toString();
					QString compression = layer.value("compression").toString();
					int layerHeight = layer.value("height").toInt();

					QByteArray rawData;
					if (encoding == "base64") {
						rawData = QByteArray::fromBase64(layer.value("data").toString().toLatin1());
					}
					else {
						qDebug() << "Unsupported tilelayer encoding:" << encoding;
						continue;
					}

					QByteArray tileData;
					if (compression == "zlib") {
						// qUncompress attend un prefixe de 4 octets big-endian avec la taille decompressee
						int expectedSize = mapWidth * layerHeight * 4;
						QByteArray withHeader(4, 0);
						withHeader[0] = (expectedSize >> 24) & 0xFF;
						withHeader[1] = (expectedSize >> 16) & 0xFF;
						withHeader[2] = (expectedSize >> 8) & 0xFF;
						withHeader[3] = expectedSize & 0xFF;
						withHeader.append(rawData);
						tileData = qUncompress(withHeader);
						if (tileData.isEmpty()) {
							qDebug() << "Failed to decompress tilelayer:" << name;
							continue;
						}
					}
					else if (compression.isEmpty()) {
						tileData = rawData;
					}
					else {
						qDebug() << "Unsupported tilelayer compression:" << compression;
						continue;
					}

					// Chaque GID est un uint32 little-endian (4 octets)
					int tileCount = mapWidth * layerHeight;
					for (int idx = 0; idx < tileCount; idx++) {
						if (idx * 4 + 3 >= tileData.size()) break;
						quint32 gid = (quint8)tileData[idx * 4]
							| ((quint8)tileData[idx * 4 + 1] << 8)
							| ((quint8)tileData[idx * 4 + 2] << 16)
							| ((quint8)tileData[idx * 4 + 3] << 24);
						if (gid == 0) continue; // case vide
						int tileIndex = (int)gid - firstgid;
						if (tileIndex < 0 || tileIndex >= (int)tiles.size()) {
							qDebug() << "GID out of range:" << gid;
							continue;
						}
						int tileX = (idx % mapWidth) * TILE_SIZE;
						int tileY = (idx / mapWidth) * TILE_SIZE;
						Tile tile(tileX, tileY, TILE_SIZE, TILE_SIZE, collides, tiles[tileIndex], name);
						prisonMap.push_back(tile);
					}
				}
				else if (type == "objectgroup") {
					QJsonArray layerArray = layer.value("objects").toArray();
					for (int j = 0; j < layerArray.size(); j++) {
						QJsonObject item = layerArray[j].toObject();
						if (!item.contains("gid")) continue;
						int gid = item.value("gid").toInt();
						int tileIndex = gid - firstgid;
						if (tileIndex < 0 || tileIndex >= (int)tiles.size()) {
							qDebug() << "objectgroup GID out of range:" << gid << "/ tiles:" << tiles.size();
							continue;
						}
						int objH = item.value("height").toInt();
						Tile tile(
							item.value("x").toInt(),
							item.value("y").toInt() - objH,
							objH,
							item.value("width").toInt(),
							collides,
							tiles[tileIndex],
							name
						);
						prisonMap.push_back(tile);
					}
				}
			}
		}
		else {
			return false;
		}
	}
}

bool ResourceManager::loadPlayerSprites() {
	QPixmap sheet;
	sheet.load(":/sprites/Run_Voltron.png");
	if (sheet.isNull()) return false;
	int frameQuantity = sheet.width() / 48;
	for (int i = 0; i < frameQuantity; i++) {
		QPixmap frame = sheet.copy(i * 48, 0, 48, 48);
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

bool ResourceManager::loadMenuResources() {
	QPixmap sprite("./resources/sprites/Space_background.png");
	if (sprite.isNull()) {
		qDebug() << "failed to load the background\n";
		return false;
	}
	background = sprite.copy();
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

QPixmap ResourceManager::getFilledhealth() {
	return filledHealth;
}

QPixmap ResourceManager::getEmptyHealth() {
	return emptyHealth;
}

vector<QPixmap> ResourceManager::getFallingStarAnimation() {
	return fallingStarAnimation;
}

QPixmap ResourceManager::getBackground() {
	return background;
}