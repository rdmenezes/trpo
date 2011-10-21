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
           if (!canPlace)
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
#define NEAR_BOUNDARY 7

inline bool nearToPoint(QPointF & pivot, QPointF & test)
{
    return  fabs(pivot.x()-test.x())<NEAR_BOUNDARY
             && fabs(pivot.y()-test.y())<NEAR_BOUNDARY;
}

void DiagramScene::processArrowClickOnLine(QGraphicsSceneMouseEvent * event,
                                           ArrowSegment * seg)
{
  QPointF pos=event->scenePos();
  if (m_arrow_state==AES_NONE)
  {
      bool neato=nearToPoint(*(seg->out()),pos);
      bool isend=seg->out()->isEndingPoint();
      if (neato && isend)
      {
          m_last_arrow_point=seg->out();
          m_arrow_state=AES_EDIT;
      }
      else
      {
          bool  constructedInSegIsTooSmall=nearToPoint(*(seg->in()),pos);
          bool  constructedOutSegIsTooSmall=nearToPoint(*(seg->out()),pos);
          if  (constructedInSegIsTooSmall || constructedOutSegIsTooSmall) return;
          ArrowDirection d=seg->direction();
          if (d==AD_LEFT || d==AD_RIGHT) pos.setY(seg->in()->y());
          if (d==AD_BOTTOM || d==AD_TOP) pos.setX(seg->in()->x());
          m_last_arrow_point=new ArrowPoint(pos.x(),pos.y());
          ArrowSegment * seg_in=new ArrowSegment(seg->in(),m_last_arrow_point);
          ArrowSegment * seg_out=new ArrowSegment(m_last_arrow_point,seg->out());
          seg->in()->tryRemoveSegment(seg);
          seg->out()->tryRemoveSegment(seg);
          seg->die();
          m_diag->addArrowSegment(seg_in);
          m_diag->addArrowSegment(seg_out);
          this->addItem(seg_in);
          this->addItem(seg_out);
          m_arrow_state=AES_EDIT;
          return;
      }
  }
  if (m_arrow_state==AES_EDIT)
  {
      if (!m_last_arrow_point->isIncident(seg))
      {
       processArrowJoin(pos,seg);
      }
      return;
  }
}

void DiagramScene::processArrowJoin(const QPointF & pos, ArrowSegment * seg)
{
  QPointF realpos=constructDirectedLine(*m_last_arrow_point,pos);
  QRectF rect=seg->boundingRect();
  if (realpos.x()>rect.right() || realpos.x()<rect.left() || realpos.y()<rect.top()
      || realpos.y()>rect.bottom()) return;
  ArrowDirection dir=direction(*m_last_arrow_point,realpos);
  if (m_last_arrow_point->hasOppositeSegment(m_last_arrow_point,&realpos)) return;
  if (nearToPoint(*(seg->in()),realpos) && seg->in()->isBeginPoint())
  {
      QPointF endpos(seg->in()->x(),seg->in()->y());
      ArrowDirection segdir=seg->direction();

      QPointF resultingmove;
      if (canBeMerged(m_last_arrow_point,endpos,segdir,dir,resultingmove))
      {
          ArrowPoint * tmp=new ArrowPoint(resultingmove.x(),resultingmove.y());
          bool canPlace=m_diag->canPlaceSegment(tmp,seg->in(),NULL);
          delete tmp;
          if (canPlace)
          {
              processArrowMerge(resultingmove,seg,&dir,&segdir);
              m_arrow_state=AES_NONE;
              m_last_arrow_point=NULL;
              return;
          }
      }
  }
  else //Join arrow
  {
        if (nearToPoint(*(seg->in()),realpos) || nearToPoint(*(seg->out()),realpos))
            return;
  }
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


bool threeSegmentsOnLineCanRemoveTwoPoints(ArrowPoint * m_last_arrow_point,
                                           ArrowSegment * seg,
                                           ArrowDirection * mydir,
                                           ArrowDirection * segdir)
{
 //Define all removing points
 ArrowPoint * p1=m_last_arrow_point;
 ArrowPoint * p2=seg->in();
 bool  sameDirectedWithAddedSegment=p1->hasInputSegment(mydir);
 bool  hasOneInputSegmentAndNoAnnotationsAndNoForks=p1->inputSegments().count()==1 &&
                                                   (!p1->hasAnnotations()) &&
                                                    p1->outputSegments().count()==0;
 bool  sameDirectedAddedAndNext= *mydir==*segdir;
 bool  hasOneOutputSegmentAndNoAnnotationsAndForks=p2->inputSegments().count()==0 &&
                                                   !(p2->hasAnnotations()) &&
                                                   p2->outputSegments().count()<=1;

 return sameDirectedWithAddedSegment && hasOneInputSegmentAndNoAnnotationsAndNoForks
        && sameDirectedAddedAndNext && hasOneOutputSegmentAndNoAnnotationsAndForks;
}

void mergeThreeSegmentsInOne(DiagramScene * scene,
                             ArrowPoint * m_last_arrow_point,
                             ArrowSegment * seg
                             )
{
  ArrowPoint * p0=m_last_arrow_point->inputSegments()[0]->in();
  ArrowPoint * p1=m_last_arrow_point;
  ArrowPoint * p2=seg->in();
  ArrowPoint * p3=seg->out();
  ArrowSegment * s1=m_last_arrow_point->inputSegments()[0];
  ArrowSegment * s3=seg;
  //Add new segment
  ArrowSegment * new_segment=new ArrowSegment(p0,p3);
  scene->addItem(new_segment);
  scene->diagram()->addArrowSegment(new_segment);
  p0->tryRemoveSegment(s1);
  p3->tryRemoveSegment(s3);
  s1->die();
  s3->die();
  p1->die();
  p2->die();
  p0->update();
  p3->update();
}

bool canRemoveLastArrowPointPreviousAndAddedOnLine(ArrowPoint * m_last_arrow_point,
                                                   ArrowDirection * mydir)
{
 ArrowPoint * p1=m_last_arrow_point;
 bool  sameDirectedWithAddedSegment=p1->hasInputSegment(mydir);
 bool  hasOneInputSegmentAndNoAnnotationsAndNoForks=p1->inputSegments().count()==1 &&
                                                    (!p1->hasAnnotations()) &&
                                                    p1->outputSegments().count()==0;


 return sameDirectedWithAddedSegment
         && hasOneInputSegmentAndNoAnnotationsAndNoForks;
}

void mergePreviousAndAdded(DiagramScene * scene,
                           ArrowPoint * m_last_arrow_point,
                           ArrowSegment * seg)
{
 ArrowPoint * p0=m_last_arrow_point->inputSegments()[0]->in();
 ArrowPoint * p1=m_last_arrow_point;
 ArrowPoint * p2=seg->in();
 ArrowSegment * s1=m_last_arrow_point->inputSegments()[0];
 //Add new segment
 ArrowSegment * new_segment=new ArrowSegment(p0,p2);
 scene->addItem(new_segment);
 scene->diagram()->addArrowSegment(new_segment);
 s1->die();
 p1->die();
 p0->update();
 p2->update();
}

bool canMergeAddedAndNext(ArrowSegment * seg,
                          ArrowDirection * mydir,
                          ArrowDirection * segdir)
{
    //Define all removing points
    ArrowPoint * p2=seg->in();
    bool  sameDirectedAddedAndNext= *mydir==*segdir;
    bool  hasOneOutputSegmentAndNoAnnotationsAndForks=p2->inputSegments().count()==0 &&
                                                      !(p2->hasAnnotations()) &&
                                                      p2->outputSegments().count()<=1;

    return sameDirectedAddedAndNext && hasOneOutputSegmentAndNoAnnotationsAndForks;
}

void mergeAddedAndNext(DiagramScene * scene,
                       ArrowPoint * m_last_arrow_point,
                       ArrowSegment * seg)
{
  ArrowPoint * p0=m_last_arrow_point;
  ArrowPoint * p1=seg->in();
  ArrowPoint * p2=seg->out();
  ArrowSegment * s3=seg;
  //Add new segment
  ArrowSegment * new_segment=new ArrowSegment(p0,p2);
  scene->addItem(new_segment);
  scene->diagram()->addArrowSegment(new_segment);
  s3->die();
  p1->die();
  p0->update();
  p2->update();
}

void DiagramScene::processArrowMerge(const QPointF & pos, ArrowSegment * seg, ArrowDirection * mydir, ArrowDirection  * segdir)
{
  bool handled=false;
  m_last_arrow_point->setX(pos.x());
  m_last_arrow_point->setY(pos.y());
  if (threeSegmentsOnLineCanRemoveTwoPoints(m_last_arrow_point,seg,mydir,segdir)
      && !handled)
  {
    handled=true;
    mergeThreeSegmentsInOne(this,m_last_arrow_point,seg);
  }
  if (canRemoveLastArrowPointPreviousAndAddedOnLine(m_last_arrow_point,mydir)
      && !handled)
  {
   handled=true;
   mergePreviousAndAdded(this,m_last_arrow_point,seg);
  }
  if (canMergeAddedAndNext(seg,mydir,segdir))
  {
   handled=true;
   mergeAddedAndNext(this,m_last_arrow_point,seg);
  }
  if (!handled)
  {
    ArrowSegment * newseg=new ArrowSegment(m_last_arrow_point,seg->in());
    m_diag->addArrowSegment(newseg);
    this->addItem(newseg);
    newseg->update();
  }

}
