#include "diagramscene.h"
#include "helpwindow.h"
#include <QTextEdit>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

DiagramScene::DiagramScene(QObject *parent) :
    QGraphicsScene(parent)
{
  m_tooltype=TT_BLOCK;
}

void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos=event->buttonDownScenePos(event->button());
    if (this->items(pos).size()==0)
    {

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
  if (event->key()==Qt::Key_Shift)
  {

      QPointF  panel_pos;
      QPoint local_pos=m_view->mapFromGlobal(QCursor::pos());
      panel_pos=m_view->mapToScene(local_pos);
      QTextEdit * txte=new QTextEdit();
      txte->setGeometry(panel_pos.x(),panel_pos.y(),100,100);
      this->addWidget(txte);
  }
  if (!handled)
      this->QGraphicsScene::keyPressEvent(event);
}
