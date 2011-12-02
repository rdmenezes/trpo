#include "graphicunittests.h"
#include "diagramscene.h"
#include "diagram.h"
#include "box.h"
#include "freecomment.h"
#include "arrow.h"
#include "commentline.h"
#include "diagramobjecttest.h"
#define  _USE_MATH_DEFINES
#include <math.h>
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
}

void GraphicUnitTests::performArrowTest2()
{
    {
        ObjectConnector * connector1=new ObjectConnector(50,50,150,50);
        ObjectConnector * connector2=new ObjectConnector(150,50,150,150);
        ObjectConnector * connector3=new ObjectConnector(150,150,300,150);
        connector1->addConnector(connector2,1,C_OUTPUT);
        connector2->addConnector(connector1,0,C_INPUT);
        connector2->addConnector(connector3,1,C_OUTPUT);
        connector3->addConnector(connector2,0,C_INPUT);
        Arrow    * arr1=new Arrow(connector1,m_diag);
        Arrow    * arr2=new Arrow(connector2,m_diag);
        Arrow    * arr3=new Arrow(connector3,m_diag);
        m_diag->scene()->addItem(arr1);
        m_diag->scene()->addItem(arr2);
        m_diag->scene()->addItem(arr3);
    }
    {
        ObjectConnector * connector1=new ObjectConnector(50,250,150,250);
        ObjectConnector * connector2=new ObjectConnector(150,250,150,252);
        ObjectConnector * connector3=new ObjectConnector(150,252,300,252);
        connector1->addConnector(connector2,1,C_OUTPUT);
        connector2->addConnector(connector1,0,C_INPUT);
        connector2->addConnector(connector3,1,C_OUTPUT);
        connector3->addConnector(connector2,0,C_INPUT);
        Arrow    * arr1=new Arrow(connector1,m_diag);
        Arrow    * arr2=new Arrow(connector2,m_diag);
        Arrow    * arr3=new Arrow(connector3,m_diag);
        m_diag->scene()->addItem(arr1);
        m_diag->scene()->addItem(arr2);
        m_diag->scene()->addItem(arr3);
    }
}

void GraphicUnitTests::performArrowTest3()
{
    {
        ObjectConnector * connector1=new ObjectConnector(50,150,150,150);
        ObjectConnector * connector2=new ObjectConnector(150,150,150,50);
        ObjectConnector * connector3=new ObjectConnector(150,50,300,50);
        connector1->addConnector(connector2,1,C_OUTPUT);
        connector2->addConnector(connector1,0,C_INPUT);
        connector2->addConnector(connector3,1,C_OUTPUT);
        connector3->addConnector(connector2,0,C_INPUT);
        Arrow    * arr1=new Arrow(connector1,m_diag);
        Arrow    * arr2=new Arrow(connector2,m_diag);
        Arrow    * arr3=new Arrow(connector3,m_diag);
        m_diag->scene()->addItem(arr1);
        m_diag->scene()->addItem(arr2);
        m_diag->scene()->addItem(arr3);
    }
    {
        ObjectConnector * connector1=new ObjectConnector(50,252,150,252);
        ObjectConnector * connector2=new ObjectConnector(150,252,150,250);
        ObjectConnector * connector3=new ObjectConnector(150,250,300,250);
        connector1->addConnector(connector2,1,C_OUTPUT);
        connector2->addConnector(connector1,0,C_INPUT);
        connector2->addConnector(connector3,1,C_OUTPUT);
        connector3->addConnector(connector2,0,C_INPUT);
        Arrow    * arr1=new Arrow(connector1,m_diag);
        Arrow    * arr2=new Arrow(connector2,m_diag);
        Arrow    * arr3=new Arrow(connector3,m_diag);
        m_diag->scene()->addItem(arr1);
        m_diag->scene()->addItem(arr2);
        m_diag->scene()->addItem(arr3);
    }
}

void GraphicUnitTests::performArrowTest4()
{
    {
        ObjectConnector * connector1=new ObjectConnector(350,150,150,150);
        ObjectConnector * connector2=new ObjectConnector(150,150,150,50);
        ObjectConnector * connector3=new ObjectConnector(150,50,50,50);
        connector1->addConnector(connector2,1,C_OUTPUT);
        connector2->addConnector(connector1,0,C_INPUT);
        connector2->addConnector(connector3,1,C_OUTPUT);
        connector3->addConnector(connector2,0,C_INPUT);
        Arrow    * arr1=new Arrow(connector1,m_diag);
        Arrow    * arr2=new Arrow(connector2,m_diag);
        Arrow    * arr3=new Arrow(connector3,m_diag);
        m_diag->scene()->addItem(arr1);
        m_diag->scene()->addItem(arr2);
        m_diag->scene()->addItem(arr3);
    }
    {
        ObjectConnector * connector1=new ObjectConnector(300,252,150,252);
        ObjectConnector * connector2=new ObjectConnector(150,252,150,250);
        ObjectConnector * connector3=new ObjectConnector(150,250,50,250);
        connector1->addConnector(connector2,1,C_OUTPUT);
        connector2->addConnector(connector1,0,C_INPUT);
        connector2->addConnector(connector3,1,C_OUTPUT);
        connector3->addConnector(connector2,0,C_INPUT);
        Arrow    * arr1=new Arrow(connector1,m_diag);
        Arrow    * arr2=new Arrow(connector2,m_diag);
        Arrow    * arr3=new Arrow(connector3,m_diag);
        m_diag->scene()->addItem(arr1);
        m_diag->scene()->addItem(arr2);
        m_diag->scene()->addItem(arr3);
    }
}


void GraphicUnitTests::performArrowTest5()
{
    {
        ObjectConnector * connector1=new ObjectConnector(300,50,150,50);
        ObjectConnector * connector2=new ObjectConnector(150,50,150,150);
        ObjectConnector * connector3=new ObjectConnector(150,150,50,150);
        connector1->addConnector(connector2,1,C_OUTPUT);
        connector2->addConnector(connector1,0,C_INPUT);
        connector2->addConnector(connector3,1,C_OUTPUT);
        connector3->addConnector(connector2,0,C_INPUT);
        Arrow    * arr1=new Arrow(connector1,m_diag);
        Arrow    * arr2=new Arrow(connector2,m_diag);
        Arrow    * arr3=new Arrow(connector3,m_diag);
        m_diag->scene()->addItem(arr1);
        m_diag->scene()->addItem(arr2);
        m_diag->scene()->addItem(arr3);
    }
    {
        ObjectConnector * connector1=new ObjectConnector(300,250,150,250);
        ObjectConnector * connector2=new ObjectConnector(150,250,150,252);
        ObjectConnector * connector3=new ObjectConnector(150,252,50,252);
        connector1->addConnector(connector2,1,C_OUTPUT);
        connector2->addConnector(connector1,0,C_INPUT);
        connector2->addConnector(connector3,1,C_OUTPUT);
        connector3->addConnector(connector2,0,C_INPUT);
        Arrow    * arr1=new Arrow(connector1,m_diag);
        Arrow    * arr2=new Arrow(connector2,m_diag);
        Arrow    * arr3=new Arrow(connector3,m_diag);
        m_diag->scene()->addItem(arr1);
        m_diag->scene()->addItem(arr2);
        m_diag->scene()->addItem(arr3);
    }
}

void GraphicUnitTests::performCommentLineTest()
{
  QPointF pivot(100,100);
  for (qreal i=0;i<6.28;i+=0.3 )
  {
       int len=i*6.0+1;
        QPointF  p2=pivot+len*QPointF(cos(i),-1*sin(i));
        CommentLine * cml=new CommentLine(pivot,p2,NULL,NULL,m_diag);
        m_diag->scene()->addItem(cml);
  }
}


void GraphicUnitTests::performCommentLineTest2()
{
    qreal angles[]={M_PI/4, 3*M_PI/4, 5*M_PI/4, 7*M_PI/4,0,M_PI/2,M_PI,3*M_PI/2 };
    QPointF pivot(100,100);
    for (int  i=4;i<8;i++)
    {
        QPointF  p2=pivot+50*QPointF(cos(angles[i]),-1*sin(angles[i]));
        CommentLine * cml=new CommentLine(pivot,p2,NULL,NULL,m_diag);
        m_diag->scene()->addItem(cml);
    }
}

void GraphicUnitTests::performDefaultObjectTest()
{
    DiagramObjectTest * diag=new DiagramObjectTest(QPointF(100,100),m_diag);
    m_diag->scene()->addItem(diag);
}

