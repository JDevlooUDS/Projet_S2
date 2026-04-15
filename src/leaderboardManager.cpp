#include "../header/leaderboardManager.h"

LeaderboardManager& LeaderboardManager::getInstance() {
	static LeaderboardManager instance;
	return instance;
}

void LeaderboardManager::saveScore(QString name, double time) {
    loadFromFile();

    if (!scores.contains(name) || time < scores[name]) {
        scores[name] = time;
        writeToFile();
    }
}

QList<PlayerScore> LeaderboardManager::getTopTen() {
    loadFromFile();
    QList<PlayerScore> list;

    for (auto it = scores.begin(); it != scores.end(); ++it) {
        list.append({ it.key(), it.value() });
    }

    std::sort(list.begin(), list.end(), [](const PlayerScore& a, const PlayerScore& b) {
        return a.time < b.time;
    });

    return list.mid(0, 10);
}

void LeaderboardManager::loadFromFile() {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    scores.clear();
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(";");
        if (parts.size() == 2) {
            scores[parts[0]] = parts[1].toDouble();
        }
    }
    file.close();
}

void LeaderboardManager::writeToFile() {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;

    QTextStream out(&file);
    for (auto it = scores.begin(); it != scores.end(); ++it) {
        out << it.key() << ";" << it.value() << "\n";
    }
    file.close();
}