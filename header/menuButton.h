#pragma once

#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QColor>
#include <QBrush>
#include <QObject>
#include <QCursor>

class MenuButton : public QObject, public QGraphicsRectItem {
	Q_OBJECT

public:
	MenuButton(QString text, qreal witdh, qreal height, QGraphicsItem* parent = nullptr);
	~MenuButton();

signals:
	void clicked();

protected:
	void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
	void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
	QGraphicsTextItem* label;
};