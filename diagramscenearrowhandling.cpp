#include "diagramscene.h"
#include "arrowpoint.h"
#include "arrowsegment.h"
#include "alineitem.h"
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <math.h>

void DiagramScene::processArrowEscapePress(const QPointF & pos)
{
  (void)pos;
  bool handled=false;
  if (m_last_arrow_point->isSeparated())
  {  m_last_arrow_point->die(); handled=true; }
  if (!handled && m_last_arrow_point->canBeRemoved())
  {
      ArrowSegment * out=m_last_arrow_point->outputSegments()[0];
      ArrowSegment * in=m_last_arrow_point->outputSegments()[0];
      m_last_arrow_point->tryRemoveSegment(out);
      m_last_arrow_point->tryRemoveSegment(in);
      m_last_arrow_point->die();
      in->setOut(out->out());
      out->die();
      in->update();
      handled=true;
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
       processArrowJoinSegments(pos,seg);
      }
      return;
  }
}

void DiagramScene::processArrowJoinSegments(const QPointF & pos, ArrowSegment * seg)
{
  QPointF realpos=constructDirectedLine(*m_last_arrow_point,pos);
  QRectF rect=seg->boundingRect();
  if (realpos.x()>rect.right() || realpos.x()<rect.left() || realpos.y()<rect.top()
      || realpos.y()>rect.bottom()) return;
  ArrowDirection dir=direction(*m_last_arrow_point,realpos);
  {
   bool hasopposed=m_last_arrow_point->hasOppositeSegment(m_last_arrow_point,&realpos);
   if (hasopposed)
      return;
  }
  if (nearToPoint(*(seg->in()),realpos) && seg->in()->isBeginPoint())
  {
      bool hasop2=seg->in()->hasOppositeSegment(m_last_arrow_point,&realpos);
      if (hasop2)
           return;
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
        if ( (seg->direction()==AD_LEFT && dir==AD_LEFT)
             || (seg->direction()==AD_LEFT && dir==AD_RIGHT)
             || (seg->direction()==AD_RIGHT && dir==AD_LEFT)
             || (seg->direction()==AD_RIGHT && dir==AD_RIGHT))
            return ;
        if ( (seg->direction()==AD_TOP && dir==AD_TOP)
             || (seg->direction()==AD_TOP && dir==AD_BOTTOM)
             || (seg->direction()==AD_BOTTOM && dir==AD_TOP)
             || (seg->direction()==AD_BOTTOM && dir==AD_BOTTOM))
            return ;
        if  (seg->direction()==AD_TOP && seg->direction()==AD_BOTTOM)
            realpos.setX(seg->in()->x());
        if  (seg->direction()==AD_LEFT && seg->direction()==AD_RIGHT)
            realpos.setY(seg->in()->y());
        ArrowPoint * tmp=new ArrowPoint(realpos.x(),realpos.y());
        if (m_diag->canPlaceSegment(m_last_arrow_point,tmp,NULL))
        {
         processArrowJoin(realpos,seg);
         m_last_arrow_point=NULL;
         m_arrow_state=AES_NONE;
        }
        delete tmp;
  }
}

void DiagramScene::processArrowJoin(const QPointF & pos,ArrowSegment * seg)
{
 ArrowPoint * p0=seg->in();
 ArrowPoint * p1=new ArrowPoint(pos.x(),pos.y());
 ArrowPoint * p2=seg->out();
 ArrowSegment * seg_p1=new ArrowSegment(p0,p1);
 ArrowSegment * seg_p2=new ArrowSegment(p1,p2);
 ArrowSegment * seg_p3=new ArrowSegment(m_last_arrow_point,p1);
 m_diag->addArrowSegment(seg_p1);
 m_diag->addArrowSegment(seg_p2);
 m_diag->addArrowSegment(seg_p3);
 this->addItem(seg_p1);
 this->addItem(seg_p2);
 this->addItem(seg_p3);
 p0->tryRemoveSegment(seg);
 p2->tryRemoveSegment(seg);
 seg->die();
 p1->update();
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

void DiagramScene::enterArrowMove(const QPointF & pos,ArrowSegment * seg)
{
  (void)pos;
  m_dragstate=DS_ARROW_MOVE;
  m_moving_segment=seg;
}


bool testForIncidentCollision(Diagram * diag,ArrowPoint * p,ArrowPoint * pos)
{
 const QVector<ArrowSegment *> & in=p->inputSegments();
 const QVector<ArrowSegment *> & out=p->outputSegments();
 for (int i=0;i<in.size();i++)
 {
   if (!diag->canPlaceSegment(in[i]->in(),pos))
   {
      return false;
   }
 }
 for (int i=0;i<out.size();i++)
 {
   if (!diag->canPlaceSegment(pos,out[i]->out()))
   {
      return false;
   }
 }
 return true;
}

void DiagramScene::arrowMoveLeave(const QPointF &pos)
{
 QPointF p1(m_moving_segment->in()->x(),m_moving_segment->in()->y());
 QPointF p2(m_moving_segment->out()->x(),m_moving_segment->out()->y());
 ArrowDirection dir=m_moving_segment->direction();
 if (dir==AD_LEFT || dir==AD_RIGHT) { p1.setY(pos.y()); p2.setY(pos.y()); }
 else { p1.setX(pos.x()); p2.setX(pos.x());}
 ArrowPoint * tmp1=new ArrowPoint(p1.x(),p1.y());
 ArrowPoint * tmp2=new ArrowPoint(p1.x(),p2.y());
 bool canPlace=m_diag->canPlaceSegment(tmp1,tmp2);
 bool canMove1=true,canMove2=true;
 canMove1=m_moving_segment->in()->canMoveTo(p1,m_moving_segment);
 canMove2=m_moving_segment->out()->canMoveTo(p2,m_moving_segment);
 if (canPlace && canMove1 && canMove2)
 {
     bool canMoveLeft=testForIncidentCollision(m_diag,m_moving_segment->in(),tmp1);
     bool canMoveRight=testForIncidentCollision(m_diag,m_moving_segment->in(),tmp2);
     //Check, whether moving every point makes collision with blocks
     delete tmp1;
     delete tmp2;
     if (canMoveLeft && canMoveRight)
     {
        m_moving_segment->in()->setX(p1.x());
        m_moving_segment->in()->setY(p1.y());
        m_moving_segment->out()->setX(p2.x());
        m_moving_segment->out()->setY(p2.y());
        m_moving_segment->in()->update();
        m_moving_segment->out()->update();
        update();
     }
     else
     {
         delete tmp1;
         delete tmp2;
     }
 }
 m_dragstate=DS_NONE;
 m_moving_segment=NULL;
}

void DiagramScene::removeArrowSegment(ArrowSegment * seg)
{
  ArrowPoint * in=seg->in();
  ArrowPoint * out=seg->out();
  if (seg->in()->tryRemoveSegment(seg,false))
  {
    if (seg->out()->tryRemoveSegment(seg,false))
    {
        seg->die();
        if (in->isSeparated())
        {
            in->die();
        }
        if (out->isSeparated())
        {
            out->die();
        }
        update();
    }
    else in->addOutputSegment(seg);;
  }
}
