#include "debug.h"

static QList<QLabel*> debugLabels;

void debugText(QGraphicsView* view, const QString& info, int y, int x) {
    QLabel* label = new QLabel(info, view);  // parent = la view position fixe écran
    label->setStyleSheet("color: black; background: transparent; font-size: 14px;");
    label->move(x, y);
    label->show();
    debugLabels.append(label);
}

void clearDebug(QGraphicsView* view) {
    for (QLabel* label : debugLabels) {
        label->deleteLater();
    }
    debugLabels.clear();
}