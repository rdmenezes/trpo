#include "objectconnector.h"
#include "arrow.h"
#include <math.h>
#include <assert.h>
#include <QMessageBox>
#include "saveload.h"

Direction ObjectConnector::direction() const
{
   Direction dir=D_LEFT;
    if (fabs(this->p1().x()-this->p2().x())<0.0001)
    {
       if (p1().y()>p2().y()) dir=D_TOP;
       else dir=D_BOTTOM;
    }
    else
    {
        if (p2().x()>p1().x()) dir=D_RIGHT;
    }
    return dir;
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

void ObjectConnector::save(QDomDocument *  doc,
                      QDomElement *  element)
{
    QDomElement objectConnector;
    objectConnector=doc->createElement("ObjectConnector");
    QString buf, bufName;

    buf=::save(m_parent);   // DiagramObject   *   m_parent
    objectConnector.setAttribute("diagram object", buf);

    // 1 vector of box connectors    QVector< QPair<qreal,ObjectConnector*> > m_connected[2]
    for (int i = 0; i < m_connected[1].size(); ++i)
    {
        buf=::save(m_connected[1].at(i));
        bufName.clear();
        bufName.setNum(i);
        objectConnector.setAttribute(bufName.prepend("m_connected_1 "), buf);

    }

    // 2 vector of box connectors
    for (int j = 0; j < m_connected[2].size(); ++j)
    {
        buf=::save(m_connected[2].at(j));
        bufName.clear();
        bufName.setNum(j);
        objectConnector.setAttribute(bufName.prepend("m_connected_2 "), buf);
    }

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

