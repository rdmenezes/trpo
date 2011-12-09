#include "selecttool.h"
#include "drawingconstants.h"

bool contains(const QLineF & line, const QPointF & point)
{
    bool point_x=fabs(line.x1()-line.x2())<0.01;
    bool point_y=fabs(line.y1()-line.y2())<0.01;
    if (point_x && point_y)
    {
        point_x=fabs(line.x1()-point.x())<0.01;
        point_y=fabs(line.y1()-point.y())<0.01;
        return point_x && point_y;
    }
    else
    {
        double px=(point.x()-line.x2())/(line.x1()-line.x2());
        double py=(point.y()-line.y2())/(line.y1()-line.y2());
        if (fabs(px-py)<0.01)
        {
            return 0<=px && px<=1;
        }
        else return false;
    }
}

bool ConnectorChangingData::checkResize(const QLineF & moveposition, const QLineF & resizeposition)
{
  QPointF p=position(moveposition,m_position_on_moved);
  return contains(resizeposition,p);
}

bool ArrowChangingData::checkResize(const QLineF & moveposition, const QLineF & resizeposition)
{
  bool ok=true;
  for (int i=0;i<m_commentlines.size();i++)
  {
       QPointF p=position(moveposition,m_commentlinesonline[i]);
       bool canResize=contains(resizeposition,p);
       ok=ok && canResize;
  }
 for (int i=0;i<m_incident_segment_changes.size();i++)
  {
     bool canResize=m_incident_segment_changes[i].checkResize(
                                moveposition,resizeposition);
     ok=ok && canResize;
 }
 return ok;
}

/*
void ObjectChangingData::extractData(ObjectConnector * c)
{
 QVector< QPair<qreal,ObjectConnector*> > input=c->getConnected(C_INPUT);
 extractData(c,C_INPUT,input);
 QVector< QPair<qreal,ObjectConnector*> > output=c->getConnected(C_OUTPUT);
 extractData(c,C_OUTPUT,input);
}


void ObjectChangingData::extractData(ObjectConnector * c,
                                     Connection ct,
                                     const QVector< QPair<qreal,ObjectConnector *> > & from)
{
  for (int i=0;i<from.size();i++)
  {
    QPair<qreal,ObjectConnector *> temp=from[i];
    qreal pos=temp.first;
    ObjectConnector * con=temp.second;
    if (con->parent() && con->parent()->type()==IsCommentLine)
    {
        CommentLine * cl=static_cast<CommentLine*>(con->parent());
        m_commentlines<<cl;
        m_commentlinesposition<<QLineF(cl->in().x(),cl->in().y(),cl->out().x(),cl->out().y());
    }
    else
    {
        m_attachedconnectors<<c;
        m_connected_objects<<con;
        m_connection_direction<<ct;
        m_poses<<pos;
        if (ct==C_INPUT)
            m_reachedpos<<c->p2();
        else
            m_reachedpos<<c->p1();
    }
  }
}
*/
