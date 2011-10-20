#include "arrowpoint.h"
#include "alineitem.h"
#include "arrowsegment.h"
#include "diagram.h"

ArrowPoint::ArrowPoint(qreal x, qreal y): QPointF(x,y)
{
    m_block=NULL;
}


void ArrowPoint::die()
{
    m_diag->removeArrowPoint(this);
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
        if ( (dir==AD_LEFT && dir2==AD_RIGHT)  || (dir==AD_TOP && dir2==AD_BOTTOM))
        {
            return true;
        }
    }
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
