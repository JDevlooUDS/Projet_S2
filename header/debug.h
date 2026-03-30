#pragma once
#include <QGraphicsView>
#include <QLabel>
#include <QString>

void debugText(QGraphicsView* view, const QString& info, int y = 10, int x = 10);
void clearDebug(QGraphicsView* view);