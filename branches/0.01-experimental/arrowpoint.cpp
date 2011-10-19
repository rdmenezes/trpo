#include "arrowpoint.h"

ArrowPoint::ArrowPoint(qreal x, qreal y): QPointF(x,y)
{
    m_block=NULL;
}


bool ArrowPoint::isBeginPoint() { return m_out.size()==0; }
bool ArrowPoint::isEndingPoint() { return m_in.size()==0; }

bool ArrowPoint::mustBeDecomposed() { return isEndingPoint() && m_out.size()>1; }
bool ArrowPoint::isSeparated()      { return isBeginPoint() && isEndingPoint(); }

ArrowPoint::~ArrowPoint()
{

}
