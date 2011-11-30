#include "graphicunittests.h"
#include "diagramscene.h"
#include "diagram.h"
#include "box.h"
#include "freecomment.h"
#include <iostream>

void GraphicUnitTests::performBoxVerticalTextRegenTest()
{
    Box * t=new Box(QPointF(100,25),m_diag);
    QRectF tmp=t->boundingRect();
    m_diag->scene()->addItem(t);

    for (int i=1;i<5;i++)
    {
      Box * box=new Box(QPointF (100,50
                                     +(tmp.height()+20)*i),
                        m_diag);
      m_diag->scene()->addItem(box);
      QRectF r=box->boundingRect();
      r.moveLeft(box->x());
      r.moveTop(box->y());
      r.setWidth(r.width()+4*i);
      r.setHeight(r.height()+4*i);
      box->setRect(r);
      box->setText("Text\ngrazedaaaaa!");
    }

}

void GraphicUnitTests::performFreeCommentTest()
{
  FreeComment * fc=new FreeComment(QPointF(50,50),m_diag);
  m_diag->scene()->addItem(fc);
  /*
  FreeComment * fc2=new FreeComment(QPointF(100,100),m_diag);
  m_diag->scene()->addItem(fc2);
  fc2->setText("Complex comment\nnow simple");
  FreeComment * fc3=new FreeComment(QPointF(200,100),m_diag);
  m_diag->scene()->addItem(fc3);
  fc3->setText("Complex comment\nnow simple");
  fc3->moveTo(QPointF(200,200));
  */
}
