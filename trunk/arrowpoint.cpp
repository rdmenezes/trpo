#include "arrowpoint.h"
#include "alineitem.h"
#include "arrowsegment.h"
#include "diagram.h"
#include "box.h"
#include <QMessageBox>

ArrowPoint::ArrowPoint(qreal x, qreal y): QPointF(x,y)
{
    m_block=NULL;
}


void ArrowPoint::die()
{
    if (m_block)
        m_block->removePointReference(this);
    m_diag->removeArrowPoint(this);
    delete this;
}

bool ArrowPoint::canBeRemoved()
{
 if (m_in.size()!=1 || m_out.size()!=1 || m_lines.size()!=0) return false;
 return m_in[0]->direction()==m_out[0]->direction();
}

bool ArrowPoint::isBeginPoint()  const { return m_in.size()==0; }
bool ArrowPoint::isEndingPoint() const { return m_out.size()==0; }

bool ArrowPoint::mustBeDecomposed() const { return isEndingPoint() && m_out.size()>1; }
bool ArrowPoint::isSeparated()      const { return isBeginPoint() && isEndingPoint(); }


void ArrowPoint::addInputSegment(ArrowSegment *segment)
{
    m_in<<segment;
}

void ArrowPoint::addOutputSegment(ArrowSegment * segment)
{
    m_out<<segment;
}

void ArrowPoint::update()
{
 for (int i=0;i<m_lines.size();i++)
    this->m_lines[i]->update();
 for (int i=0;i<m_in.size();i++)
    this->m_in[i]->update();
 for (int i=0;i<m_out.size();i++)
    this->m_out[i]->update();
}

ArrowPoint::~ArrowPoint()
{

}

bool ArrowPoint::hasSameInputSegment(QPointF * in, QPointF * out)
{
    if (m_in.count()!=1) return false;
    ArrowDirection dir=direction(*in,*out);
    if (m_in[0]->direction()==dir) return true;
    else return false;
}

bool ArrowPoint::hasOppositeSegment(QPointF * in, QPointF * out)
{
    ArrowDirection dir=direction(*in,*out);
    for (int i=0;i<m_in.size();i++)
    {
        ArrowDirection dir2=m_in[i]->direction();
        if ( (dir==AD_LEFT && dir2==AD_RIGHT)
            || (dir2==AD_LEFT && dir==AD_RIGHT)
            || (dir==AD_TOP && dir2==AD_BOTTOM)
            || (dir2==AD_TOP && dir==AD_BOTTOM))
        {
            return true;
        }
    }
    return false;
}

bool ArrowPoint::hasSameDirectedSegment(QPointF * in, QPointF * out)
{
    ArrowDirection dir=direction(*in,*out);
    for (int i=0;i<m_out.size();i++)
        if (m_out[i]->direction()==dir)
            return true;
    return false;
}

bool ArrowPoint::hasInputSegment(ArrowDirection * d)
{
    for (int i=0;i<m_in.size();i++)
    {
        if (m_in[i]->direction()==*d)
            return true;
    }
    return false;
}

bool ArrowPoint::hasOutputSegment(ArrowDirection * d)
{
    for (int i=0;i<m_out.size();i++)
    {
        if (m_out[i]->direction()==*d)
            return true;
    }
    return false;
}

bool ArrowPoint::tryRemoveSegment(ArrowSegment * segment, bool correct_arcs)
{
    QVector<int> poses_in_in;
    QVector<int> poses_in_out;
    for (int i=0;i<m_in.size();i++)
    {
        if (m_in[i]==segment)
            poses_in_in<<i;
    }
    for (int i=0;i<m_out.size();i++)
    {
        if (m_out[i]==segment)
            poses_in_out<<i;
    }
    //Check, whether point becomes separated
    bool becomesSeparated= poses_in_in.size()==m_in.size()
                           && poses_in_out.size()==m_out.size();
    bool canDie=becomesSeparated;
    if (canDie)
    {
        if (m_lines.size()!=0)
        {
            QMessageBox::StandardButton b=QMessageBox::question(NULL,"Question",
                                                                "Removing segment leads to deleting some annotation lines. Proceed?",
                                                                QMessageBox::Ok | QMessageBox::Cancel,QMessageBox::Ok);
            if (b==QMessageBox::Cancel)
                canDie=false;
        }
    }
    if (  !becomesSeparated || canDie)
    {
      for (int i=0;i<poses_in_in.size();i++)
      {
        int index=poses_in_in[i]-i;
        if (correct_arcs)
        {
         if (m_in[index]->in()==this) m_in[index]->setIn(NULL);
         if (m_in[index]->out()==this) m_in[index]->setOut(NULL);
        }
        m_in.remove(index);
      }
      for (int i=0;i<poses_in_out.size();i++)
      {
        int index=poses_in_out[i]-i;
        if (correct_arcs)
        {
        if (m_out[index]->in()==this) m_out[index]->setIn(NULL);
        if (m_out[index]->out()==this) m_out[index]->setOut(NULL);
        }
        m_out.remove(index);
      }
      if (canDie)
      {
          for (int i=0;i<m_lines.size();i++)
              m_lines[i]->die();
      }
      return true;
    }
    return false;
}

bool ArrowPoint::isIncident(ArrowSegment * s)
{
    for (int i=0;i<m_in.size();i++)
        if (m_in[i]==s)
            return true;
    for (int i=0;i<m_out.size();i++)
        if (m_out[i]==s)
            return true;
    return false;
}

MoveRange ArrowPoint::moveRange(ArrowSegment * exc)
{
    MoveRange result(-1E+6,-1E+6,2E+6,2E+6);
    if (m_lines.size()!=0)
        return MoveRange(x(),y(),0,0);
    for (int i=0;i<m_in.size();i++)
    {
      if (m_in[i]!=exc)
      {
        ArrowDirection dir=m_in[i]->direction();
        generatePolicy(result,m_in[i]->in(),dir,false);
      }
    }
    for (int i=0;i<m_out.size();i++)
    {
      if (m_out[i]!=exc)
      {
        ArrowDirection dir=m_out[i]->direction();
        generatePolicy(result,m_out[i]->out(),dir,true);
      }
    }
    if (m_block)
    {
        MoveRange m=m_block->getRange(this);
        if (result.top()>m.top()) result.setTop(m.top());
        if (result.bottom()<m.bottom()) result.setBottom(m.bottom());
        if (result.left()>m.left()) result.setLeft(m.left());
        if (result.right()<m.right()) result.setRight(m.right());
    }
    return result;
}

void ArrowPoint::generatePolicy(MoveRange & result,ArrowPoint * pivot,ArrowDirection dir, bool reversed)
 {
    if (dir==AD_LEFT || dir==AD_RIGHT)
    {
        qreal yrestriction=pivot->y();
        result.setTop(yrestriction);
        result.setBottom(yrestriction);
        if (dir==AD_LEFT || (dir==AD_RIGHT && reversed))
        {
            qreal right_restrx=pivot->x();
            if (result.right()>right_restrx) result.setRight(right_restrx);
        }
        else
        {
            qreal left_restrx=pivot->x();
            if (result.left()<left_restrx) result.setLeft(left_restrx);
        }
    }
    else
    {
        qreal xrestriction=pivot->x();
        result.setLeft(xrestriction);
        result.setRight(xrestriction);
        if (dir==AD_TOP || (dir==AD_BOTTOM && reversed))
        {
            qreal bottom_restrx=pivot->y();
            if (result.bottom()>bottom_restrx) result.setBottom(bottom_restrx);
        }
        else
        {
            qreal top_restrx=pivot->y();
            if (result.top()<top_restrx) result.setTop(top_restrx);
        }
    }
 }

bool ArrowPoint::canMoveTo(const QPointF & pos, ArrowSegment * exc)
{
     MoveRange r=moveRange(exc);
     return r.isWithin(pos);
}

void ArrowPoint::removeAnnotation(ALineItem * line)
{
    for (int i=0;i<m_lines.size();i++)
    {
        if (m_lines[i]==line)
        {
            m_lines.remove(i);
            --i;
        }
    }
}

void ArrowPoint::attachAnnotation(ALineItem * line)
{
    m_lines<<line;
}
