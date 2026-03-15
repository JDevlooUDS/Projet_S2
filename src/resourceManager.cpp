#include "../header/resourceManager.h"
#include <QFile>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QDirIterator>

ResourceManager::ResourceManager() {
	QFile file(":/map/test_map.json");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "Failed to load file\n";
	}
	else {
		QByteArray rawData = file.readAll();
		file.close();

		QJsonDocument doc = QJsonDocument::fromJson(rawData);
		if (!doc.isNull()) {
			QJsonObject jsonObject = doc.object();
			int height = jsonObject.value("height").toInt();
			qDebug() << height;
		}
		else {
			qDebug() << "Failed to parse JSON document\n";
		}
	}
}

ResourceManager::~ResourceManager() {}