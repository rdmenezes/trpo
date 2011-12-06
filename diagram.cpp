#include "diagram.h"
#include "box.h"
#include "collision.h"
#include "freecomment.h"
#include "commentline.h"
#include "arrowpoint.h"
#include "arrowsegment.h"
#include "diagramset.h"
#include "attachedcomment.h"
#include "collisiondetector.h"

int Diagram::id() const
{
  return m_set->find(this);
}

int Diagram::getBoxNumber(DiagramObject * b)
{
    if (m_boxes.contains(static_cast<Box*>(b)))
    {
        return m_boxes[static_cast<Box*>(b)];
    }
    int i=0;
    bool hasID=false;
    do
    {
        hasID=m_boxes.values().contains(i);
        if (hasID)
            ++i;
    }
    while (hasID);
    return i;
}

void Diagram::save(QDomDocument * /* doc */,
                      QDomElement *  /* element */)
{
    //!< TODO: Implement this later
}

void Diagram::load(QDomElement * /* element */,
                      QMap<void *, Serializable *> & /* addressMap */ )
{
    //!< TODO: Implement this later
}

void Diagram::resolvePointers(QMap<void *, Serializable *> &
                                 /* adressMap */)
{
    //!< TODO: Implement this later
}


bool Diagram::canPlace(DiagramObject * obj,
                        const QVector<int> & exctypes,
                        const QVector<CollisionObject *> & excobjs
                      )
{
    bool can=true;
    CollisionDetector * detector=new CollisionDetector(exctypes,excobjs);
    for (int i=0;i<m_objects.size();i++)
    {
        DiagramObject * d=m_objects[i];
        can= can && !( detector->test(obj,d) );
    }
    delete detector;
    return can;
}



bool Diagram::isCorrect()
{
  return true;
}

Diagram::Diagram()
{
    m_parent=DiagramParent(-1,-1);
}

Diagram::Diagram(const DiagramParent&  l)
{
    m_parent=l;
}


bool Diagram::empty() const
{
  return m_boxes.size()==0;
}

void Diagram::add(DiagramObject * obj)
{
  if (m_objects.contains(obj))
      return;
  if (obj->type()==IsBox)
  {
      int id=getBoxNumber(obj);
      m_boxes.insert(static_cast<Box*>(obj),id);
  }
  m_objects<<obj;
}


void Diagram::clear()
{
    m_boxes.clear();
    for (int i=0;i<m_objects.size();i++)
    {
        if (m_objects[i]->deleteOnRemoval())
            this->scene()->removeItem(m_objects[i]);
        else
            delete m_objects[i];
    }
    m_objects.clear();
}

void Diagram::items(const QPointF & pos, QList<QGraphicsItem *>  & lst)
{
    for (int i=0;i<m_objects.size();i++)
    {
        if (m_objects[i]->type()==IsAttachedComment)
        {
            AttachedComment * ac=static_cast<AttachedComment *>(m_objects[i]);
            if (ac->comment())
                if (ac->comment()->contains(pos-ac->comment()->pos()) && !(lst.contains(ac->comment())))
                    lst<<ac->comment();
            if (ac->line())
                if (ac->line()->contains(pos-ac->line()->pos()) && !(lst.contains(ac->line())))
                    lst<<ac->line();
        }
        else
        {
          if (m_objects[i]->contains(pos-m_objects[i]->pos()) && !lst.contains(m_objects[i]))
              lst<<m_objects[i];
        }
    }
}

void Diagram::changeBlockNumber(int delta, Box * b)
{
    int newid=m_boxes[b]+delta;
    //Undo swap, if it was
    if (m_swaps.size())
    {
        //Undo swap with this box, if it was
        if (clock()-m_swaps[m_swaps.size()-1].m_time<HISTORY_CLEAR_TIME*CLOCKS_PER_SEC)
        {
           SwapEntry entry=m_swaps[m_swaps.size()-1];
           if ((entry.m_box1==b || entry.m_box2==b)
               && m_boxes.contains(entry.m_box1)
               && m_boxes.contains(entry.m_box2))
           {
            int pid1=m_boxes[entry.m_box1];
            int pid2=m_boxes[entry.m_box2];
            m_boxes[entry.m_box1]=pid2;
            m_boxes[entry.m_box2]=pid1;
            entry.m_box1->setNumber(pid2);
            entry.m_box2->setNumber(pid1);
            m_swaps.remove(m_swaps.size()-1);
           }
        }  else m_swaps.clear();
    }
    //Swap numbers if we need to
    int oldid=m_boxes[b];
    int pos=m_boxes.values().indexOf(newid);
    if (pos!=-1)
    {
        Box * swappedbox=m_boxes.keys()[pos];
        m_boxes[swappedbox]=oldid;
        swappedbox->setNumber(oldid);
        m_swaps<<SwapEntry(clock(),swappedbox,b);
    }
    //Set new id
    m_boxes[b]=newid;
    b->setNumber(newid);
}

void Diagram::remove(DiagramObject * obj)
{
    bool removed=false;
    DiagramObject * tobj=NULL;
    for (int i=0;i<m_objects.size() && !removed;i++)
    {
        tobj=m_objects[i];
        if (tobj->dieIfEqualTo(obj))
        {
            delete tobj;
            removed=true;
        }
    }
    for (int i=0;i<m_objects.size();i++)
    {
     if (m_objects[i]==tobj)
     {
        m_objects.remove(i);
        --i;
     }
    }
}




bool Diagram::canAddBoxes() const
{
 return true;
}

int Diagram::getBoxID() const
{
  return 0;
}

void Diagram::addBox(Box * box)
{
  //this->m_boxes[box->id()]=box;
}

void Diagram::addAnnotationLabel(FreeComment * label)
{
  this->m_alabels<<label;
}

void Diagram::removeAnnotationLabel(FreeComment * label)
{
    for (int i=0;i<m_alabels.size();i++)
    {
        if (m_alabels[i]==label)
        {
            m_alabels.remove(i);
            --i;
        }
    }
}

bool Diagram::testForNoBlockCollision(const QRectF & rect, QGraphicsItem * pointer)
{
    //Scan blocks
    /*
    for (int i=0;i<DIAGRAM_MAX_BLOCKS;i++)
    {
      if (m_boxes[i]!=NULL && m_boxes[i]!=pointer)
      {
          if (collides(rect,m_boxes[i]->boundingRect()))
              return false;
      }
    }
    */
    return true;
}

bool Diagram::doesntCollideWithLines(const QRectF & rect)
{
    for (int i=0;i<m_arrow_segments.size();i++)
    {
        ArrowPoint * iin=m_arrow_segments[i]->in();
        ArrowPoint * iout=m_arrow_segments[i]->out();
        QPointF * in=m_arrow_segments[i]->in();
        QPointF * out=m_arrow_segments[i]->out();
        //if (collides(rect,*in,*out) && !boundaryCollides(rect,iin,iout))
        //{
        //    return false;
        //}
    }
    for (int i=0;i<m_alines.size();i++)
    {
        QPointF * p1=m_alines[i]->begin();
        QPointF * p2=m_alines[i]->end();
       // if (collides(rect,*p1,*p2) && !boundaryCollides(rect,p1,p2))
        //{
       //      return false;
       // }
    }
    return true;
}

bool Diagram::testForNoALabelCollision(const QRectF &rect, QGraphicsItem *pointer)
{
    //Scan annotation label
    for (int i=0;i<m_alabels.size();i++)
    {
        if (m_alabels[i]!=NULL && m_alabels[i]!=pointer)
        {
            if (collides(rect,m_alabels[i]->boundingRect()))
                return false;
        }
    }
    return true;
}

bool Diagram::canBePlaced(const QRectF &rect, QGraphicsItem *pointer)
{
  return testForNoALabelCollision(rect,pointer)
          && testForNoBlockCollision(rect,pointer);
}

bool Diagram::canBePlaced(const QRectF &rect, FreeComment *pointer)
{
    return canBePlaced(rect,(QGraphicsItem*)pointer) && doesntCollideWithLines(rect);
}
bool Diagram::canBePlaced(const QRectF & rect, Box * pointer)
{
    return canBePlaced(rect,(QGraphicsItem*)pointer) && doesntCollideWithLines(rect);
}


Box * Diagram::getBlockByID(int id)
{
   //return m_boxes[id];
   return 0;
}



void Diagram::setBlockID(Box * item, char pos)
{
    /*
    if (item)
    {
     for(int i=0;i<DIAGRAM_MAX_BLOCKS;i++)
        if (m_boxes[i]==item)
              m_boxes[i]=NULL;
    }
    m_boxes[pos]=item;
    if (item)
        item->setID(pos);
    */
}

void Diagram::addAnnotationLine(CommentLine * line)
{
    m_alines<<line;
    line->setDiagram(this);
}

void Diagram::addArrowPoint(ArrowPoint *point)
{
    m_arrow_points<<point;
    point->setDiagram(this);

}

void Diagram::addArrowSegment(ArrowSegment * segment)
{
    m_arrow_segments<<segment;
    segment->setDiagram(this);
}

void Diagram::removeAnnotationLine(CommentLine * line)
{
   for (int i=0;i<m_alines.size();i++)
   {
      if (m_alines[i]==line)
      {
        m_alines.remove(i);
        --i;
      }
   }
}

void Diagram::removeArrowPoint(ArrowPoint * point)
{
   for (int i=0;i<m_arrow_points.size();i++)
   {
      if (m_arrow_points[i]==point)
      {
        m_arrow_points.remove(i);
        --i;
      }
   }
}

void Diagram::removeArrowSegment(ArrowSegment * segment)
{
   for (int i=0;i<m_arrow_segments.size();i++)
   {
      if (m_arrow_segments[i]==segment)
      {
        m_arrow_segments.remove(i);
        --i;
      }
   }
}

QRectF enlarge(const QRectF & rect)
{
  return QRectF(rect.x()-QRECTF_ENLARGE_RANGE,rect.y()-QRECTF_ENLARGE_RANGE,
                rect.width()+QRECTF_ENLARGE_RANGE*2,rect.height()+QRECTF_ENLARGE_RANGE*2);
}

QVector<ArrowPoint *> Diagram::getNearArrowPoints(const QRectF & rect)
{
    QVector<ArrowPoint *> result;
    QRectF enl=enlarge(rect);
    for (int i=0;i<m_arrow_points.count();i++)
    {
        QPointF * p=m_arrow_points[i];
        if (p->x()>= enl.left() && p->x()<=enl.right() && p->y()<=enl.bottom()
                                                       && p->x()<=enl.top())
        {
          if (m_arrow_points[i]->isBeginPoint() || m_arrow_points[i]->isEndingPoint())
            result<<m_arrow_points[i];
        }
    }
    return result;
}

bool Diagram::canBePlacedAroundPoints(const QRectF & rect, const QVector<ArrowPoint *> pts)
{
    unsigned int refs[BLOCK_SIDES]={0};
    bool correct=true;
    for (int i=0;(i<pts.size()) && correct;i++)
    {
        BoxSide bis=Box::sideOfPoint(pts[i],rect);
        if (bis==BIS_LEFT || bis==BIS_TOP)
        {
            if (pts[i]->isEndingPoint()) ++(refs[bis]); else correct=false;
        }
        if (bis==BIS_RIGHT)
        {
            if (pts[i]->isBeginPoint()) ++(refs[bis]); else correct=false;
        }
        if (bis==BIS_BOTTOM)  ++(refs[bis]);
    }
    for (int i=0;i<BLOCK_SIDES;i++)
        correct=correct && refs[i]<MAX_LINE_REFERENCES;
    return correct;
}

bool Diagram::canPlaceSegment(ArrowPoint * point1, ArrowPoint * point2,
                              Box * block, bool always_check_bounds)
{
    /*
    for (int i=0;i<DIAGRAM_MAX_BLOCKS;i++)
    {
     if (m_boxes[i]!=NULL)
     {
       QRectF rect=m_boxes[i]->boundingRect();
       bool result=false;
       //if ((result && (m_boxes[i]==block
       //           || point1->attachedBlock()==m_boxes[i]
       //           || point2->attachedBlock()==m_boxes[i])) || always_check_bounds)
       //     result=result && !boundaryCollides(rect,point1,point2);
       if (result) return false;
     }
    }
    for (int i=0;i<m_alabels.size();i++)
    {
        //if (collides(m_alabels[i]->boundingRect(),*point1,*point2))
        //    return false;
    }
    */
    return true;
}


bool Diagram::canPlaceAnnotationLine(const QPointF * point1,const QPointF * point2)
{
    ArrowPoint * p1=new ArrowPoint(point1->x(),point1->y());
    ArrowPoint * p2=new ArrowPoint(point2->x(),point2->y());
    bool collides=canPlaceSegment(p1,p2,NULL,true);
    delete p1;
    delete p2;
    return collides;
}

Diagram::~Diagram()
{
  clear();
}

