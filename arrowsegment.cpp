#include "arrowsegment.h"
#include "arrowpoint.h"
#include "compare.h"

ArrowSegment::ArrowSegment(ArrowPoint * in, ArrowPoint * out)
{
  m_in=in;
  m_out=out;
  m_in->addOutputSegment(this);
  m_out->addInputSegment(this);
}


ArrowSegment::~ArrowSegment()
{

}

ArrowDirection direction(const QPointF & in,const QPointF & out)
{
    qreal dy=fabs(in.y()-out.y());
    qreal dx=fabs(in.x()-out.x());
    if (dy<dx)
    {
        if (out.x()>in.x()) return AD_RIGHT;
        else return AD_LEFT;
    }
    else
    {
        if (out.y()>in.y()) return AD_BOTTOM;
        else return AD_TOP;
    }
    return AD_TOP;
}

ArrowDirection ArrowSegment::direction()
{
    return ::direction(*m_in,*m_out);
}


bool tooSmall(ArrowPoint * in, const QPointF & out)
{
    ArrowDirection d=direction(*in,out);
    if (d==AD_LEFT || d==AD_RIGHT) return fabs(in->x()-out.x())<MIN_LENGTH_OF_SEGMENT;
    return fabs(in->y()-out.y())<MIN_LENGTH_OF_SEGMENT;
}

#define D_LEFT 4
#define D_RIGHT 3
QRectF ArrowSegment::boundingRect()
{
  ArrowDirection dir=direction();
  QRectF result;
  if (dir==AD_LEFT)
      result=QRectF(m_in->x(),m_in->y()-D_LEFT,m_out->x()-m_in->x(),D_LEFT+D_RIGHT);
  if (dir==AD_RIGHT)
      result=QRectF(m_out->x(),m_in->y()-D_LEFT,m_in->x()-m_out->x(),D_LEFT+D_RIGHT);
  if (dir==AD_TOP)
      result=QRectF(m_out->x()-D_LEFT,m_out->y(),D_LEFT+D_RIGHT,m_in->y()-m_out->y());
  if (dir==AD_BOTTOM)
      result=QRectF(m_out->x()-D_LEFT,m_in->y(),D_LEFT+D_RIGHT,m_out->y()-m_in->y());

  return result;
}
