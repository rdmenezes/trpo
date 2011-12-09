#include "objectconnector.h"
#include "arrow.h"
#include <math.h>
#include <assert.h>
#include <QMessageBox>
#include "saveload.h"


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


bool ObjectConnector::canMove(ObjectConnector * sender,const QLineF & newpos)
{
     Connection contn=this->connectionFor(sender);
     QPointF connectionpoint=newpos.p1();
     if (contn==C_INPUT)
         connectionpoint=newpos.p2();
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

void ObjectConnector::save(QDomDocument *  doc,
                      QDomElement *  element)
{
    QDomElement objectConnector;
    objectConnector=doc->createElement("ObjectConnector");
    QString buf, bufName;

    buf=::save(this);
    objectConnector.setAttribute("selfPointer", buf);

    buf=::save(m_parent);   // DiagramObject   *   m_parent
    objectConnector.setAttribute("diagramObject", buf);

    // 1 vector of box connectors    QVector< QPair<qreal,ObjectConnector*> > m_connected[2]
    buf=::save(m_connected[1]);
    objectConnector.setAttribute("m_connected_1", buf);

    // 2 vector of box connectors
    buf=::save(m_connected[2]);
    objectConnector.setAttribute("m_connected_2", buf);

    element->appendChild(objectConnector);
}

void ObjectConnector::load(QDomElement * /* element */,
                      QMap<void *, Serializable *> & /* addressMap */ )
{
    //!< TODO: Implement this later
}

void ObjectConnector::resolvePointers(QMap<void *, Serializable *> &
                                 /* adressMap */)
{
    //!< TODO: Implement this later
}

