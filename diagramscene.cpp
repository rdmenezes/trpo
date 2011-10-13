#include "diagramscene.h"

DiagramScene::DiagramScene(QObject *parent) :
    QGraphicsScene(parent)
{

}

void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    this->addText("Hello world!");
    this->addLine(0,0,this->width(),this->height());
    int a=1+1;
}
