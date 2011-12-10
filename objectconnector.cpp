#include "objectconnector.h"
#include "arrow.h"
#include <math.h>
#include <assert.h>
#include <QMessageBox>
#include "saveload.h"
#include "attachedcomment.h"

Direction direction(const QLineF & line)
{
    Direction dir=D_LEFT;
     if (fabs(line.p1().x()-line.p2().x())<0.0001)
     {
        if (line.p1().y()>line.p2().y()) dir=D_TOP;
        else dir=D_BOTTOM;
     }
     else
     {
         if (line.p2().x()>line.p1().x()) dir=D_RIGHT;
     }
     return dir;
}


Direction ObjectConnector::direction() const
{
    return ::direction(*this);
}

bool ObjectConnector::addConnector(ObjectConnector * c, qreal point,Connection type,bool debug)
{
    if (!isNotCollinear(c->direction(),this->direction()) && debug)
    {
        Q_ASSERT("ERROR AT OBJECTCONNECTOR" && false);
        return false;
    }
    QVector< QPair<qreal,ObjectConnector *> > * connected=&(this->m_connected[(int)type]);
    for (int i=0;i<connected->size();i++)
        if ((*connected)[i].second==c || ( fabs(point-(*connected)[i].first)<0.001
                                        && c->direction()==(*connected)[i].second->direction()
                                       ))
            return false;
    (*connected)<<QPair<qreal,ObjectConnector*>(point,c);
    return true;
}

QVector<ObjectConnector *>  ObjectConnector::getConnected(qreal point,Connection type)
{
    QVector< QPair<qreal,ObjectConnector *> > & connected=m_connected[(int)type];
    QVector<ObjectConnector *> result;
    for (int i=0;i<connected.size();i++)
        if (fabs(connected[i].first-point)<0.001)
            result<<connected[i].second;

    return result;
}

bool ObjectConnector::hasConnected(ObjectConnector * o)
 {
     for (int i=0;i<2;i++)
     {
         for (int j=0;j<m_connected[i].size();j++)
         {
             if (m_connected[i][j].second==o)
                 return true;
         }
     }
     return false;
 }


void ObjectConnector::remove(ObjectConnector *o)
{
    for (int i=0;i<2;i++)
    {
        for (int j=0;j<m_connected[i].size();j++)
        {
            if (m_connected[i][j].second==o)
            {
               m_connected[i].remove(j);
               --j;
            }
        }
    }
}


void ObjectConnector::enlarge(const QPointF & newend,bool end)
{
  QLineF oldline(p1(),p2());
  QLineF newline;
  if (end) newline=QLineF(p1(),newend); else newline=QLineF(newend,p2());
  for (int i=0;i<2;i++)
  {
      for (int j=0;j<m_connected[i].size();j++)
      {
        QPointF oldpos=position(oldline,m_connected[i][j].first);
        m_connected[i][j].first=position(newline,oldpos);
      }
  }
  if (end)
    this->setP2(newend);
  else
    this->setP1(newend);
}

void ObjectConnector::clear()
{
  for (int i=0;i<2;i++)
      m_connected[i].clear();
}


bool hasWay(ObjectConnector * start, ObjectConnector * goal)
{
  if (start==goal)
      return true;
  {
    bool result=false;
    const QVector< QPair<qreal, ObjectConnector*> > & v=start->getConnected(C_INPUT);
    for (int i=0;i<v.size() && !result;i++)
    {
        result=result || hasWay(v[i].second,goal);
    }
    return result;
  }
}

void removeReferencesAndKillLines(ObjectConnector * removing,ObjectConnector * o,Connection dir,Diagram * d)
{
    QVector< QPair<qreal, ObjectConnector *> > cts=o->getConnected(dir);
    for (int i=0;i<cts.size();i++)
    {
        cts[i].second->remove(removing);
        DiagramObject * parent=cts[i].second->parent();
        if (parent)
            if (parent->type()==IsCommentLine)
            {
                d->remove(parent);
            }
    }
}


qreal ObjectConnector::getPosition(ObjectConnector * c)
{
    QVector< QPair<qreal,ObjectConnector*> > v=m_connected[0];
    for (int i=0;i<v.size();i++)
        if (v[i].second==c)
            return v[i].first;
    v=m_connected[1];
    for (int i=0;i<v.size();i++)
        if (v[i].second==c)
            return v[i].first;
    return -1;
}

void ObjectConnector::regeneratePositions()
{
    for (int i=0;i<m_connected[0].size();i++)
    {
        QPair<qreal,ObjectConnector*> tmp=m_connected[0][i];
        tmp.first=position(*this,tmp.second->p2());
        m_connected[0][i]=tmp;
    }
    for (int i=0;i<m_connected[1].size();i++)
    {
        QPair<qreal,ObjectConnector*> tmp=m_connected[1][i];
        tmp.first=position(*this,tmp.second->p1());
        m_connected[1][i]=tmp;
    }
}


bool ObjectConnector::canMoveCollinear()
{
    bool can=true;
    for (int i=0;i<m_connected[0].size();i++)
    {
         QPair<qreal,ObjectConnector*> tmp=m_connected[0][i];
         if (tmp.second->isAttachedToBox()
             ||
             ( (fabs(tmp.first)<0.001
             || fabs(tmp.first-1)<0.001)
             && tmp.second->isAttachedToSegment()))
             can=false;
    }
    for (int i=0;i<m_connected[1].size();i++)
    {
         QPair<qreal,ObjectConnector*> tmp=m_connected[1][i];
         if (tmp.second->isAttachedToBox()
             ||
             ( (fabs(tmp.first)<0.001
             || fabs(tmp.first-1)<0.001)
             && tmp.second->isAttachedToSegment()))
             can=false;
    }
    return can;
}

bool contains(const QLineF & line, const QPointF & point);


bool ObjectConnector::canMove(ObjectConnector * sender,const QPointF & connectionpoint)
{
     if (isAttachedToCommentLine())
         return true;
     if (isAttachedToBox())
         return contains(*this,connectionpoint);
     if (isAttachedToSegment())
     {
         qreal pos=getPosition(sender);
         if (fabs(pos)>0.0001
            && fabs(pos-1)>0.0001)
             return contains(*this,connectionpoint);

         QLineF line(this->p1(),connectionpoint);
         if (fabs(pos)<0.001)
             line=QLineF(connectionpoint,this->p2());
         if (::direction(line)!=direction())
             return false;
         return canResize(sender,line);
     }
     return true;
}


bool ObjectConnector::canResize(ObjectConnector * sender,
                                const QLineF & line )
{
    bool can=true;
    for (int i=0;i<2;i++)
    {
        for (int j=0;j<m_connected[i].size();j++)
        {
            QPair<qreal,ObjectConnector *> test=m_connected[i][j];
            if (test.second!=sender)
            {
                bool cont=contains(line,position(*this,test.first));
                can=can && cont;
            }
        }
    }
    return can;
}

void ObjectConnector::setPosition(ObjectConnector * c, qreal pos)
{
 for (int i=0;i<2;i++)
 {
   for (int j=0;j<m_connected[i].size();j++)
   {
      if (m_connected[i][j].second==c)
          m_connected[i][j].first=pos;
   }
 }
}


void ObjectConnector::moveOrResize(ObjectConnector * sender,const QPointF & connectionpoint)
{
    if (isAttachedToCommentLine())
    {
        setP1(connectionpoint);
        CommentLine * cline=static_cast<CommentLine*>(parent());
        cline->setLine(connectionpoint.x(),
                       connectionpoint.y(),
                       cline->out().x(),
                       cline->out().y());
        cline->parentComment()->comment()->setPosWithoutCheck(
                cline->parentComment()->comment()->pos()
                );
    }
    if (isAttachedToBox())
    {
        qreal pos=position(*this,connectionpoint);
        setPosition(sender,pos);
    }
    if (isAttachedToSegment())
    {
        qreal pos=getPosition(sender);
        if (fabs(pos)>0.0001
           && fabs(pos-1)>0.0001)
        {
            qreal pos=position(*this,connectionpoint);
            setPosition(sender,pos);
        }
        else
        {
         QLineF line(this->p1(),connectionpoint);
         if (fabs(pos)<0.001)
            line=QLineF(connectionpoint,this->p2());
         setPosition(sender,pos);
         resize(sender,line);
         static_cast<Arrow*>(parent())->regenerate();
        }
    }
}

void ObjectConnector::moveSelfRegeneratingOrResizing()
{
  for(int i=0;i<2;i++)
  {
    for (int j=0;j<m_connected[i].size();j++)
    {
        QPair<qreal,ObjectConnector*> pair=m_connected[i][j];
        QPointF pos=position(*this,pair.first);
        pair.second->moveOrResize(this,pos);
    }
  }
}

void ObjectConnector::resize(ObjectConnector * sender, const QLineF & line)
{
    for (int i=0;i<2;i++)
    {
        for (int j=0;j<m_connected[i].size();j++)
        {
            if (m_connected[i][j].second!=sender)
            {
                QPointF pos=position(*this,m_connected[i][j].first);
                m_connected[i][j].first=position(line,pos);
            }
        }
    }
    setLine(line.x1(),line.y1(),line.x2(),line.y2());
}


bool ObjectConnector::testCanMove(const QPointF & vector,ObjectConnector * exc)
{
    bool can=true;
    for (int i=0;i<2;i++)
    {
        for (int j=0;j<m_connected[i].size();j++)
        {
          if (m_connected[i][j].second!=exc)
          {
            QPointF pos=position(*this,m_connected[i][j].first);
            bool test=m_connected[i][j].second->canMove(this,pos+vector);
            can= can && test;
          }
        }
    }
    return can;
}

bool ObjectConnector::canShiftCollinear(const QPointF & vector)
{
    bool can=true;
    for (int i=0;i<2;i++)
    {
        for (int j=0;j<m_connected[i].size();j++)
        {
          if (!(m_connected[i][j].second->isAttachedToCommentLine()))
          {
            bool test=m_connected[i][j].second->testCanMove(vector,this);
            can= can && test;
          }
        }
    }
    return can;
}

void ObjectConnector::shiftCollinear(const QPointF & vector)
{
    setLine(x1()+vector.x(),y1()+vector.y(),x2()+vector.x(),y2()+vector.y());
    for (int i=0;i<2;i++)
    {
        for (int j=0;j<m_connected[i].size();j++)
        {
          if (!(m_connected[i][j].second->isAttachedToCommentLine()))
          {
            ObjectConnector * tmp=m_connected[i][j].second;
            tmp->setLine(tmp->x1()+vector.x(),tmp->y1()+vector.y(),
                         tmp->x2()+vector.x(),tmp->y2()+vector.y());
            m_connected[i][j].second->moveSelfRegeneratingOrResizing();
            if (tmp->isAttachedToSegment())
                static_cast<Arrow*>(tmp->parent())->regenerate();
          }
          else
          {
            ObjectConnector * tmp=m_connected[i][j].second;
            CommentLine * cline=static_cast<CommentLine*>(tmp->parent());
            QPointF connectionpoint=cline->in()+vector;
            cline->setLine(connectionpoint.x(),
                             connectionpoint.y(),
                             cline->out().x(),
                             cline->out().y());
            cline->parentComment()->comment()->setPosWithoutCheck(
                      cline->parentComment()->comment()->pos()
                      );
          }
        }
    }
}

void ObjectConnector::save(QDomDocument *  doc,
                      QDomElement *  element)
{
    QDomElement objectConnector;
    objectConnector=doc->createElement("object_connector");

    objectConnector.setAttribute("this", ::save(this));
    objectConnector.setAttribute("parent", ::save(m_parent));
    // Vectors of box connectors    QVector< QPair<qreal,ObjectConnector*> > m_connected[2]
    objectConnector.setAttribute("connected0",::save(m_connected[0]));
    objectConnector.setAttribute("connected1",::save(m_connected[1]));

    objectConnector.setAttribute("p1",::save(this->p1()));
    objectConnector.setAttribute("p2", ::save(this->p2()));

    element->appendChild(objectConnector);
}

void ObjectConnector::load(QDomElement *  element,
                      QMap<void *, Serializable *> &  addressMap  )
{
    QDomNamedNodeMap attributes=element->attributes();
    if (attributes.contains("this"))
    {
     addressMap.insert(::load<void*>(getValue(attributes,"this")),
                       this);
    }
    if (attributes.contains("parent"))
    {
        m_parent=::load<DiagramObject*>(getValue(attributes,"parent"));
    }
    if (attributes.contains("connected0"))
    {
        m_connected[0]=::load< QVector< QPair<qreal,ObjectConnector*> > >(getValue(attributes,"connected0"));
    }
    if (attributes.contains("connected1"))
    {
        m_connected[1]=::load< QVector< QPair<qreal,ObjectConnector*> > >(getValue(attributes,"connected1"));
    }
}

void ObjectConnector::resolvePointers(QMap<void *, Serializable *> &
                                  adressMap )
{
    m_parent=reinterpret_cast<DiagramObject*>(adressMap[m_parent]);
    for (int i=0;i<2;i++)
    {
        for (int j=0;j<m_connected[i].size();j++)
        {
            ObjectConnector * np=m_connected[i][j].second;
            np=reinterpret_cast<ObjectConnector*>(adressMap[np]);
            m_connected[i][j].second=np;
        }
    }
}

