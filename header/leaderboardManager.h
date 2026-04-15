#pragma once
#include <QString>
#include <QList>
#include <QHash>
#include <QFile>
#include <QTextStream>

struct PlayerScore {
	QString name;
	double time;
};

class LeaderboardManager {
public:
	static LeaderboardManager& getInstance();
	void saveScore(QString name, double time);
	QList<PlayerScore> getTopTen();

private:
	void loadFromFile();
	void writeToFile();

	QHash<QString, double> scores;
	const QString filename = "./resources/leaderboard/board.txt";

};