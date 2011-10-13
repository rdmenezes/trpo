#include "diagramscene.h"
#include "helpwindow.h"
#include <QTextEdit>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

DiagramScene::DiagramScene(QObject *parent) :
    QGraphicsScene(parent)
{

}

void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos=event->buttonDownScenePos(event->button());
    if (this->items(pos).size()==0)
    {
     QTextEdit * txte=new QTextEdit();
     txte->setGeometry(pos.x(),pos.y(),100,100);
     this->addWidget(txte);
    }
    else
    {
        this->QGraphicsScene::mousePressEvent(event);
    }
}
void DiagramScene::keyPressEvent(QKeyEvent * event)
{
  bool handled=false;
  if (event->key()==Qt::Key_F1)
  {
      HelpWindow d;
      d.exec();
      handled=true;
  }
  if (!handled)
      this->QGraphicsScene::keyPressEvent(event);
}
