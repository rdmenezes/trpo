#include "diagramscene.h"
#include "arrowpoint.h"
#include "arrowsegment.h"
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <math.h>

void DiagramScene::processArrowEscapePress(const QPointF & pos)
{
  (void)pos;
  if (m_last_arrow_point->isSeparated())
      m_last_arrow_point->die();
  if (m_last_arrow_point->canBeRemoved())
  {
      ArrowSegment * out=m_last_arrow_point->outputSegments()[0];
      ArrowSegment * in=m_last_arrow_point->outputSegments()[0];
      m_last_arrow_point->tryRemoveSegment(out);
      m_last_arrow_point->tryRemoveSegment(in);
      m_last_arrow_point->die();
      in->setOut(out->out());
      out->die();
      in->update();
  }
  m_arrow_state=AES_NONE;
  m_last_arrow_point=NULL;
}

void DiagramScene::processArrowClickOnBlankSpace(QGraphicsSceneMouseEvent * event)
{
    if (m_arrow_state==AES_NONE)
    {
        m_last_arrow_point=new ArrowPoint(event->scenePos().x(),event->scenePos().y());
        m_arrow_state=AES_EDIT;
        m_diag->addArrowPoint(m_last_arrow_point);
    }
    else
    {
        QPointF point2=constructDirectedLine(*m_last_arrow_point,event->scenePos());
        bool samedir=m_last_arrow_point->hasSameInputSegment(m_last_arrow_point,&point2);
        if (samedir)
        {
           QPointF tmp(m_last_arrow_point->x(),m_last_arrow_point->y());
           m_last_arrow_point->setX(point2.x());
           m_last_arrow_point->setY(point2.y());
           bool canPlace=m_diag->canPlaceSegment(m_last_arrow_point->inputSegments()[0]->in(),
                                                         m_last_arrow_point);
           if (canPlace)
           {
               m_last_arrow_point->setX(tmp.x());
               m_last_arrow_point->setY(tmp.y());
           }
           m_last_arrow_point->update();
        }
        else processArrowCommonBuilding(event->scenePos());
    }
}

void DiagramScene::processArrowCommonBuilding(const QPointF & scenePos)
{
    QPointF point2=constructDirectedLine(*m_last_arrow_point,scenePos);
    if (tooSmall(m_last_arrow_point,point2)) return;
    ArrowPoint * p=new ArrowPoint(point2.x(),point2.y());
    bool placementtest=m_diag->canPlaceSegment(m_last_arrow_point,p,NULL);
    bool correctness=!(m_last_arrow_point->hasOppositeSegment(m_last_arrow_point,p));
    if (placementtest && correctness)
    {
        m_diag->addArrowPoint(p);
        ArrowSegment * seg=new ArrowSegment(m_last_arrow_point,p);
        m_diag->addArrowSegment(seg);
        this->addItem(seg);
        m_last_arrow_point->update();
        seg->update();
        m_last_arrow_point=p;
    }
    else delete p;
}

void  movePointToBound(QPointF * p,BoxItem * box,BoxItemSide side)
{
    if (side==BIS_LEFT)   p->setX(box->boundingRect().left());
    if (side==BIS_RIGHT)   p->setX(box->boundingRect().right());
    if (side==BIS_TOP)   p->setY(box->boundingRect().top());
    if (side==BIS_BOTTOM)   p->setY(box->boundingRect().bottom());
}

void DiagramScene::processArrowClickOnBox(QGraphicsSceneMouseEvent * event,
                                          BoxItem * box)
{
   ArrowPoint * p=new ArrowPoint(event->scenePos().x(),event->scenePos().y());
   if (m_arrow_state==AES_NONE)
   {
     if(box->canAddPointReference(p,BET_OUTPUT))
     {
        BoxItemSide  side=box->sideOfPoint(p);
        m_diag->addArrowPoint(p);
        movePointToBound(p,box,side);
        box->addPointReference(p);
        m_arrow_state=AES_EDIT;
        m_last_arrow_point=p;
     }
     else delete p;
   }
   else
   {
     if (box->canAddPointReference(p,BET_INPUT))
     {
        processArrowPointingToBlock(p,box);
     }
     else delete p;
   }
}
#define NEAR_BOUNDARY 5

inline bool nearToPoint(QPointF & pivot, QPointF & test)
{
    return  fabs(pivot.x()-test.x())<NEAR_BOUNDARY
             && fabs(pivot.y()-test.y())<NEAR_BOUNDARY;
}

void DiagramScene::processArrowClickOnLine(QGraphicsSceneMouseEvent * event,
                                           ArrowSegment * seg)
{

}

void DiagramScene::processArrowPointingToBlock(ArrowPoint * p, BoxItem * box)
{
  QPointF tmp=constructDirectedLine(*m_last_arrow_point,*p);
  p->setX(tmp.x()); p->setY(tmp.y());
  BoxItemSide  side=box->sideOfPoint(p);
  movePointToBound(p,box,side);
  //Check for wrong collision type with the block
  ArrowDirection  dir=direction(*m_last_arrow_point,*p);
  if (side==BIS_TOP && dir!=AD_BOTTOM)    { delete p; p=NULL;}
  if (side==BIS_RIGHT)                    { delete p; p=NULL;}
  if (side==BIS_LEFT && dir!=AD_RIGHT)     { delete p; p=NULL;}
  if (side==BIS_BOTTOM && dir!=AD_TOP) { delete p; p=NULL;}
  if (p)
  {
   bool canPlace=m_diag->canPlaceSegment(m_last_arrow_point,p,box);
   if (canPlace)
   {
      bool samedir=m_last_arrow_point->hasSameInputSegment(m_last_arrow_point,p);
      if (samedir)
            prolongArrowToBlockEdge(p,box);
      else
          if (tooSmall(m_last_arrow_point,*p)
              || m_last_arrow_point->hasOppositeSegment(m_last_arrow_point,p))
              delete p;
          else
              addArrowPointingToBlock(p,box);

   }
   else delete p;
  }
}

void DiagramScene::prolongArrowToBlockEdge(ArrowPoint * p,BoxItem * box)
{
    m_last_arrow_point->setX(p->x());
    m_last_arrow_point->setY(p->y());
    delete p;
    m_last_arrow_point->update();
    m_arrow_state=AES_NONE;
    m_last_arrow_point=NULL;
    box->addPointReference(p);

}

void DiagramScene::addArrowPointingToBlock(ArrowPoint * p,BoxItem * box)
{
    m_diag->addArrowPoint(p);
    ArrowSegment * seg=new ArrowSegment(m_last_arrow_point,p);
    m_diag->addArrowSegment(seg);
    this->addItem(seg);
    m_last_arrow_point->update();
    seg->update();
    m_arrow_state=AES_NONE;
    m_last_arrow_point=NULL;
    box->addPointReference(p);

}
