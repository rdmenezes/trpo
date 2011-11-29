#include "diagram.h"
#include "box.h"
#include "collision.h"
#include "alabelitem.h"
#include "alineitem.h"
#include "arrowpoint.h"
#include "arrowsegment.h"
#include "diagramset.h"

int Diagram::id() const
{
  return m_set->find(this);
}

int Diagram::getBoxNumber(DiagramObject * b)
{
    //!< TODO: Implement this later.
    return 0;
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



bool Diagram::isCorrect()
{
    for (int i=0;i<DIAGRAM_MAX_BLOCKS;i++)
        if (m_boxes[i]) return true;
    return false;
}

Diagram::Diagram()
{
    m_parent=DiagramParent(-1,-1);
    nullifyBoxes();
}

Diagram::Diagram(const DiagramParent&  l)
{
    m_parent=l;
    nullifyBoxes();
}

void Diagram::nullifyBoxes()
{
  for(int i=0;i<DIAGRAM_MAX_BLOCKS;i++)
  {
    m_boxes[i]=NULL;
  }
}

bool Diagram::empty() const
{
  //!< TODO: Implement this later
  return false;
}

bool Diagram::canAddBoxes() const
{
 for (int i=0;i<DIAGRAM_MAX_BLOCKS;i++)
 {
     if (!m_boxes[i]) return true;
 }
 return false;
}

int Diagram::getBoxID() const
{
 for (int i=0;i<DIAGRAM_MAX_BLOCKS;i++)
 {
     if (!m_boxes[i]) return i;
 }
 return false;
}

void Diagram::addBox(Box * box)
{
  this->m_boxes[box->id()]=box;
}

void Diagram::addAnnotationLabel(ALabelItem * label)
{
  this->m_alabels<<label;
}

void Diagram::removeAnnotationLabel(ALabelItem * label)
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
    for (int i=0;i<DIAGRAM_MAX_BLOCKS;i++)
    {
      if (m_boxes[i]!=NULL && m_boxes[i]!=pointer)
      {
          if (collides(rect,m_boxes[i]->boundingRect()))
              return false;
      }
    }
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
        if (collides(rect,*in,*out) && !boundaryCollides(rect,iin,iout))
        {
            return false;
        }
    }
    for (int i=0;i<m_alines.size();i++)
    {
        QPointF * p1=m_alines[i]->begin();
        QPointF * p2=m_alines[i]->end();
        if (collides(rect,*p1,*p2) && !boundaryCollides(rect,p1,p2))
        {
             return false;
        }
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

bool Diagram::canBePlaced(const QRectF &rect, ALabelItem *pointer)
{
    return canBePlaced(rect,(QGraphicsItem*)pointer) && doesntCollideWithLines(rect);
}
bool Diagram::canBePlaced(const QRectF & rect, Box * pointer)
{
    return canBePlaced(rect,(QGraphicsItem*)pointer) && doesntCollideWithLines(rect);
}


Box * Diagram::getBlockByID(int id)
{
   return m_boxes[id];
}



void Diagram::setBlockID(Box * item, char pos)
{
    if (item)
    {
     for(int i=0;i<DIAGRAM_MAX_BLOCKS;i++)
        if (m_boxes[i]==item)
              m_boxes[i]=NULL;
    }
    m_boxes[pos]=item;
    if (item)
        item->setID(pos);
}

void Diagram::addAnnotationLine(ALineItem * line)
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

void Diagram::removeAnnotationLine(ALineItem * line)
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
    for (int i=0;i<DIAGRAM_MAX_BLOCKS;i++)
    {
     if (m_boxes[i]!=NULL)
     {
       QRectF rect=m_boxes[i]->boundingRect();
       bool result=collides(rect,*point1,*point2);
       if ((result && (m_boxes[i]==block
                  || point1->attachedBlock()==m_boxes[i]
                  || point2->attachedBlock()==m_boxes[i])) || always_check_bounds)
            result=result && !boundaryCollides(rect,point1,point2);
       if (result) return false;
     }
    }
    for (int i=0;i<m_alabels.size();i++)
    {
        if (collides(m_alabels[i]->boundingRect(),*point1,*point2))
            return false;
    }
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

void Diagram::clear()
{
 for (int i=0;i<m_objects.size();i++)
      delete m_objects[i];
 m_objects.clear();


 for (int i=0;i<m_arrow_points.size();i++)
     delete m_arrow_points[i];
 m_alabels.clear();
 m_alines.clear();
 m_arrow_points.clear();
 m_arrow_segments.clear();
 nullifyBoxes();
 //m_history.clear();
}
