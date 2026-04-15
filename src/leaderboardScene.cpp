#include "../header/leaderboardScene.h"

LeaderboardScene::LeaderboardScene() {
	ResourceManager::getInstance().loadMenuResources();
	setSceneRect(0, 0, 1920, 1080);

	background = ResourceManager::getInstance().getBackground();
    showLeaderboardOverlay();
}

LeaderboardScene::~LeaderboardScene() {}

void LeaderboardScene::updateScene(double deltaTime, const Inputs& inputs) {
	QPointF target(1920 / 2, 1080 / 2);
	QGraphicsView* view = views().first();
	view->centerOn(target);
}

void LeaderboardScene::drawBackground(QPainter* painter, const QRectF& rect) {
	painter->drawPixmap(sceneRect().toRect(), background);
}

void LeaderboardScene::showLeaderboardOverlay() {
    overlay = new QGraphicsRectItem(0, 0, 1920, 1080);
    overlay->setBrush(QColor(0, 0, 0, 200));
    overlay->setZValue(10);
    addItem(overlay);

    QList<PlayerScore> topTen = LeaderboardManager::getInstance().getTopTen();

    QString leaderboardText = "--- TOP 10 VOLTRON ---\n\n";
    int rank = 1;
    for (const auto& entry : topTen) {
        leaderboardText += QString("%1. %2 - %3s\n")
            .arg(rank++)
            .arg(entry.name)
            .arg(entry.time, 0, 'f', 2);
    }

    QGraphicsTextItem* boardItem = new QGraphicsTextItem(leaderboardText);
    boardItem->setDefaultTextColor(Qt::cyan);
    boardItem->setFont(QFont("Monospace", 24, QFont::Bold));
    boardItem->setPos((1920 / 2) - (boardItem->boundingRect().width()/2), 300);
    boardItem->setZValue(11);
    addItem(boardItem);

    MenuButton* closeBtn = new MenuButton("Fermer", 200, 60);
    closeBtn->setPos(1920 / 2 - 100, 800);
    closeBtn->setZValue(11);
    addItem(closeBtn);

    connect(closeBtn, &MenuButton::clicked, this, [=]() {
        AudioManager::getInstance().playButtonSelectSFX();
        emit changeScene(Menu);
    });
}