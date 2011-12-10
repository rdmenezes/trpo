#include "diagram.h"
#include "box.h"
#include "collision.h"
#include "freecomment.h"
#include "commentline.h"
#include "diagramset.h"
#include "attachedcomment.h"
#include "collisiondetector.h"
#include "saveload.h"
#include "arrow.h"
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

void Diagram::save(QDomDocument *doc,
                      QDomElement *  element)
{
    QDomElement diagram;
    diagram=doc->createElement("diagram");
    pushThis(diagram);
    diagram.setAttribute("set",::save(m_set));
    diagram.setAttribute("parent",::save(m_parent));
    diagram.setAttribute("boxhash",::save(m_boxes));
    diagram.setAttribute("swaps",::save(m_swaps));
    diagram.setAttribute("objects",::save(m_objects));

    for (int i=0;i<m_objects.size();i++)
        m_objects[i]->save(doc,&diagram);

    element->appendChild(diagram);
}

void Diagram::load(QDomElement *  element ,
                      QMap<void *, Serializable *> &  addressMap  )
{
    QDomNamedNodeMap attributes=element->attributes();
    if (attributes.contains("this"))
    {
     addressMap.insert(::load<void*>(getValue(attributes,"this")),
                       this);
    }
    if (attributes.contains("set"))
    {
        m_set=::load<DiagramSet*>(getValue(attributes,"set"));
    }
    if (attributes.contains("parent"))
    {
        m_parent=::load<DiagramParent>(getValue(attributes,"parent"));
    }
    if (attributes.contains("boxhash"))
    {
        m_boxes=::load< QHash<Box *, int> >(getValue(attributes,"boxhash"));
    }
    if (attributes.contains("swaps"))
    {
        m_swaps=::load< QVector<SwapEntry> >(getValue(attributes,"swaps"));
        clock_t stamp=0;
        for (int i=m_swaps.size()-1;i>-1;i--)
        {
            if (i==m_swaps.size()-1)
            { stamp=m_swaps[i].m_time; m_swaps[i].m_time=clock(); }
            else
            { m_swaps[i].m_time=clock()+m_swaps[i].m_time-stamp;}
        }
    }
    if (attributes.contains("objects"))
    {
        m_objects=::load< QVector<DiagramObject *> >(getValue(attributes,"objects"));
    }

    //Proceed loading objects
    QDomNode diagram=element->firstChild();
    while(! (diagram.isNull()))
    {
        if (diagram.isElement())
        {
            QDomElement el=diagram.toElement();
            if (el.tagName()=="box")
            {
                Box * box=new Box();
                box->load(&el,addressMap);
            }
            if (el.tagName()=="attachedcomment")
            {
                AttachedComment * ac=new AttachedComment();
                ac->load(&el,addressMap);
            }
            if (el.tagName()=="arrow")
            {
                Arrow * ac=new Arrow();
                ac->load(&el,addressMap);
            }
        }
        diagram=diagram.nextSibling();
    }
}

void Diagram::resolvePointers(QMap<void *, Serializable *> &
                                  adressMap )
{
    m_set=static_cast<DiagramSet*>(adressMap[m_set]);
    for (int i=0;i<m_objects.size();i++)
    {
        //DiagramObject * tmpa=m_objects[i];
        m_objects[i]=static_cast<DiagramObject*>(adressMap[m_objects[i]]);
        //DiagramObject * tmpp=m_objects[i];
        m_objects[i]->resolvePointers(adressMap);
    }
    QHash<Box*,int> newhash;
    for (QHash<Box*,int>::iterator it=m_boxes.begin();
           it!=m_boxes.end();it++)
    {
        Serializable * ser=adressMap[it.key()];
        Box * bx=static_cast<Box*>(ser);
        newhash.insert(bx,it.value());
    }
    m_boxes=newhash;
    for (int i=0;i<m_swaps.size();i++)
    {
        Serializable * s1=adressMap[m_swaps[i].m_box1];
        Serializable * s2=adressMap[m_swaps[i].m_box2];
        m_swaps[i].m_box1=static_cast<Box*>(s1);
        m_swaps[i].m_box2=static_cast<Box*>(s2);
    }
}

void Diagram::fillScene(DiagramScene * scene)
{
  scene->setDiagram(this);
  this->m_scene=scene;
  for (int i=0;i<m_objects.size();i++)
      m_objects[i]->addToScene(scene);
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
    m_historyiterator=0;
}

Diagram::Diagram(const DiagramParent&  l)
{
    m_parent=l;
    m_historyiterator=0;
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
      m_objects[i]->deleteOnRemoval();
      if (m_scene==m_objects[i]->scene())
          m_scene->removeItem(m_objects[i]);
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


QDomDocument * Diagram::getStateDocument()
{
    QDomDocument *  doc =new QDomDocument("IDEFML");
    QDomElement  root=doc->createElement("root");
    doc->appendChild(root);
    save(doc,&root);
    return doc;
}

void Diagram::restoreFromDocument(QDomDocument * doc)
{
    scene()->tool()->clearState();
    clear();
    scene()->update();

    QDomElement root=doc->documentElement();

    QDomElement el=root.firstChildElement("diagram");

    DiagramSet * set=m_set;

    QMap<void*,Serializable*> addressMap;
    this->load(&el,addressMap);
    m_set=set;
    this->resolvePointers(addressMap);

    fillScene(this->scene());
    scene()->tool()->initState();
    scene()->update();
}

void Diagram::commit()
{
    for (int i=m_historyiterator;i<m_history.size();i++)
        delete m_history[i];
    m_history<<getStateDocument();
    ++m_historyiterator;
}


void Diagram::rollback()
{
    if (m_historyiterator>1)
        restoreFromDocument(m_history[m_historyiterator-2]);
    if (m_historyiterator>1)
        --m_historyiterator;
}

void Diagram::redo()
{
    //! TODO: Implement it
}

Diagram::~Diagram()
{
  clear();
  for (int i=0;i<m_history.size();i++)
      delete m_history[i];
}

