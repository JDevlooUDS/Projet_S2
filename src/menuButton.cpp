#include "../header/menuButton.h"

MenuButton::MenuButton(QString text, qreal witdh, qreal height, QGraphicsItem* parent)
	: QObject(), QGraphicsRectItem(0,0,witdh,height,parent){
	setAcceptHoverEvents(true);
	setBrush(QBrush(Qt::white));

	label = new QGraphicsTextItem(text, this);
	label->setDefaultTextColor(Qt::black);

	qreal textX = (witdh - label->boundingRect().width()) / 2;
	qreal textY = (height - label->boundingRect().height()) / 2;

	label->setPos(textX,textY);
}

MenuButton::~MenuButton() {}

void MenuButton::select() {
	setBrush(QBrush(QColor(220, 220, 220)));
}

void MenuButton::unSelect() {
	setBrush(QBrush(Qt::white));
}

void MenuButton::hoverEnterEvent(QGraphicsSceneHoverEvent* event) {
	setBrush(QBrush(QColor(220, 220, 220)));
	setCursor(Qt::PointingHandCursor);
	QGraphicsRectItem::hoverEnterEvent(event);
}

void MenuButton::hoverLeaveEvent(QGraphicsSceneHoverEvent* event) {
	setBrush(QBrush(Qt::white));
	unsetCursor();
	QGraphicsRectItem::hoverLeaveEvent(event);
}

void MenuButton::mousePressEvent(QGraphicsSceneMouseEvent* event) {
	emit clicked();
	QGraphicsRectItem::mousePressEvent(event);
}