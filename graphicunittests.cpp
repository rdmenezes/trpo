#include "graphicunittests.h"
#include "diagramscene.h"
#include "diagram.h"
#include "box.h"
#include "freecomment.h"
#include "arrow.h"
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
  FreeComment * fc=new FreeComment(QPointF(100,100),m_diag);
  m_diag->scene()->addItem(fc);
  FreeComment * fc2=new FreeComment(QPointF(100,100),m_diag);
  m_diag->scene()->addItem(fc2);
  fc2->setText("Complex comment\nnow simple");
  FreeComment * fc3=new FreeComment(QPointF(200,100),m_diag);
  m_diag->scene()->addItem(fc3);
  fc3->setText("Complex comment\nnow simple");
  fc3->moveTo(QPointF(200,200));
}

void GraphicUnitTests::performArrowTest()
{
   ObjectConnector * connector1=new ObjectConnector(50,100,150,100);
   Arrow    * arr1=new Arrow(connector1,m_diag,true,true);
   m_diag->scene()->addItem(arr1);

   ObjectConnector * connector2=new ObjectConnector(150,200,50,200);
   Arrow    * arr2=new Arrow(connector2,m_diag,true,true);
   m_diag->scene()->addItem(arr2);

   ObjectConnector * connector3=new ObjectConnector(250,50,250,200);
   Arrow    * arr3=new Arrow(connector3,m_diag,true,true);
   m_diag->scene()->addItem(arr3);

   ObjectConnector * connector4=new ObjectConnector(300,200,300,50);
   Arrow    * arr4=new Arrow(connector4,m_diag,true,true);
   m_diag->scene()->addItem(arr4);
}
