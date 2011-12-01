#include "objectconnector.h"
#include <math.h>
#include <assert.h>
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

bool ObjectConnector::addConnector(ObjectConnector * c, qreal point,Connection type)
{
    if (!isNotCollinear(c->direction(),this->direction()))
    {
        assert("ERROR AT OBJECTCONNECTOR");
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
