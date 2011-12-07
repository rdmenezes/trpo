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
    CollisionDetector * detector=new CollisionDetector(exctypes,excobjs,obj);
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
    DiagramObject * removedobj=NULL;
    for (int i=0;i<m_objects.size() && !removed;i++)
    {
        tobj=m_objects[i];
        if (tobj->dieIfEqualTo(obj))
        {
            removedobj=tobj;
            delete tobj;
            removed=true;
        }
    }
    for (int i=0;i<m_objects.size();i++)
    {
     if (m_objects[i]==removedobj)
     {
        m_objects.remove(i);
        --i;
     }
    }
}




QRectF enlarge(const QRectF & rect)
{
  return QRectF(rect.x()-QRECTF_ENLARGE_RANGE,rect.y()-QRECTF_ENLARGE_RANGE,
                rect.width()+QRECTF_ENLARGE_RANGE*2,rect.height()+QRECTF_ENLARGE_RANGE*2);
}


void Diagram::exportTo(QImage & img)
{
   img=QImage(this->scene()->width(),this->scene()->height(),QImage::Format_ARGB32);
   img.fill(0xFFFFFFFF);
   QPainter painter(&img);
   for (int i=0;i<m_objects.size();i++)
   {
       painter.translate(m_objects[i]->pos());
       m_objects[i]->paint(&painter,NULL,NULL);
       painter.resetTransform();
   }
}

Diagram::~Diagram()
{
  clear();
}

