#include "selecttool.h"

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
