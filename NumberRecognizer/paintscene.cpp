#include "paintscene.h"

paintScene::paintScene(QObject *parent) : QGraphicsScene(parent) {

}

paintScene::~paintScene() {

}

void paintScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    addEllipse(event->scenePos().x() - 5,
               event->scenePos().y() - 5,
               37,
               37,
               QPen(Qt::NoPen),
               QBrush(Qt::black));
    previousPoint = event->scenePos();
}

void paintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    addLine(previousPoint.x(),
            previousPoint.y(),
            event->scenePos().x(),
            event->scenePos().y(),
            QPen(Qt::black,37,Qt::SolidLine,Qt::RoundCap));
    previousPoint = event->scenePos();
}
