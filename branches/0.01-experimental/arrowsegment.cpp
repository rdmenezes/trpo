#include "arrowsegment.h"
#include "arrowpoint.h"
#include "compare.h"
#include <QPainter>
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

#define ASEG_RADIUS 4
QPointF  computeInputPoint(const QPointF & pivot,ArrowDirection & dir)
{
  QPointF result(pivot);
  if (dir==AD_LEFT)   {result.setX(pivot.x()-ASEG_RADIUS);}
  if (dir==AD_RIGHT)  {result.setX(pivot.x()+ASEG_RADIUS);}
  if (dir==AD_TOP)    {result.setY(pivot.y()-ASEG_RADIUS);}
  if (dir==AD_BOTTOM) {result.setY(pivot.y()+ASEG_RADIUS);}
  return result;
}

QPointF  computeOutputPoint(const QPointF & pivot,ArrowDirection & dir)
{
  QPointF result(pivot);
  if (dir==AD_LEFT)   {result.setX(pivot.x()+ASEG_RADIUS);}
  if (dir==AD_RIGHT)  {result.setX(pivot.x()-ASEG_RADIUS);}
  if (dir==AD_TOP)    {result.setY(pivot.y()+ASEG_RADIUS);}
  if (dir==AD_BOTTOM) {result.setY(pivot.y()-ASEG_RADIUS);}
  return result;
}

#define D_ARROW_X 5
#define D_ARROW_Y 3
void ArrowSegment::paint(QPainter *painter,
                         const QStyleOptionGraphicsItem *option,
                         QWidget *widget)
{
  (void)option,widget;
  ArrowDirection mydir=direction();
  QPointF din=computeInputPoint(*m_in,mydir),dout=computeOutputPoint(*m_out,mydir);
  QPointF drawout=dout;
  if (m_in->isBeginPoint() || m_in->hasInputSegment(&mydir))
  {din=*m_in;}
  if (m_out->isEndingPoint() || m_out->hasOutputSegment(&mydir))
  {dout=*m_out;}
  painter->drawLine(din,dout);
  //Draw a fork on end
  if (m_out->isEndingPoint())
  {
      QPointF aseg1(m_out->x()+D_ARROW_X,m_in->y()+D_ARROW_Y);
      QPointF aseg2(m_out->x()+D_ARROW_X,m_in->y()-D_ARROW_Y);
      if (mydir==AD_RIGHT)
      {aseg1.setX(m_out->x()-D_ARROW_X); aseg2.setX(m_out->x()-D_ARROW_X);}
      if (mydir=AD_TOP)
      {
        aseg1=*m_out;
        aseg1.setY(m_out->y()+D_ARROW_X);
        aseg1.setX(aseg1.x()+D_ARROW_Y);
        aseg1.setX(aseg1.x()-D_ARROW_Y);
      }
      if (mydir=AD_BOTTOM)
      {
        aseg1=*m_out;
        aseg1.setY(m_out->y()-D_ARROW_X);
        aseg1.setX(aseg1.x()+D_ARROW_Y);
        aseg1.setX(aseg1.x()-D_ARROW_Y);
      }
      painter->drawLine(*m_out,aseg1);
      painter->drawLine(*m_out,aseg2);
  }
  //Draw output arcs
   const QVector<ArrowSegment *> segs=m_out->outputSegments();
   for (int i=0;i<segs.size();i++)
       drawOutputArc(*m_out,mydir,segs[i],painter);
}

void ArrowSegment::drawOutputArc(const QPointF & pivot, ArrowDirection mydir,
                                 ArrowSegment * seg,QPainter * painter)
{
  if (mydir==AD_LEFT)  drawOALeft(pivot,seg,painter);
  if (mydir==AD_RIGHT) drawOARight(pivot,seg,painter);
  if (mydir==AD_TOP)   drawOATop(pivot,seg,painter);
  if (mydir==AD_BOTTOM) drawOABottom(pivot,seg,painter);
}


void ArrowSegment::drawOALeft(const QPointF &pivot,
                              ArrowSegment *seg,
                              QPainter *painter)
{
  ArrowDirection segdir=seg->direction();
  QPointF to=computeInputPoint(*(seg->in()),segdir);
  if (segdir==AD_TOP)
  {
     painter->drawArc(to.x(),
                      2*to.y()-pivot.y(),
                      2*(pivot.x()-to.x()),
                      2*(pivot.y()-to.y()),180*16,90*16);
  }
  if (segdir==AD_BOTTOM)
  {
      painter->drawArc(to.x(),
                       pivot.y(),
                       2*(pivot.x()-to.x()),
                       2*(to.y()-pivot.y()),90*16,90*16);
  }
}

void ArrowSegment::drawOARight(const QPointF &pivot,
                              ArrowSegment *seg,
                              QPainter *painter)
{
  ArrowDirection segdir=seg->direction();
  QPointF to=computeInputPoint(*(seg->in()),segdir);
  if (segdir==AD_TOP)
  {
     painter->drawArc(2*pivot.x()-to.x(),
                      2*to.y()-pivot.y(),
                      2*(to.x()-pivot.x()),
                      2*(pivot.y()-to.y()),270*16,90*16);
  }
  if (segdir==AD_BOTTOM)
  {
      painter->drawArc(2*pivot.x()-to.x(),
                       pivot.y(),
                       2*(to.x()-pivot.x()),
                       2*(to.y()-pivot.y()),0*16,90*16);
  }
}

void ArrowSegment::drawOATop(const QPointF &pivot,
                              ArrowSegment *seg,
                              QPainter *painter)
{
  ArrowDirection segdir=seg->direction();
  QPointF to=computeInputPoint(*(seg->in()),segdir);
  if (segdir==AD_LEFT)
  {
     painter->drawArc(2*to.x()-pivot.x(),
                      to.y(),
                      2*(pivot.x()-to.x()),
                      2*(pivot.y()-to.y()),0*16,90*16);
  }
  if (segdir==AD_RIGHT)
  {
      painter->drawArc(pivot.x(),
                       to.y(),
                       2*(to.x()-pivot.x()),
                       2*(pivot.y()-to.y()),90*16,90*16);
  }
}

void ArrowSegment::drawOABottom(const QPointF &pivot,
                                ArrowSegment *seg,
                                QPainter *painter)
{
  ArrowDirection segdir=seg->direction();
  QPointF to=computeInputPoint(*(seg->in()),segdir);
  if (segdir==AD_LEFT)
  {
     painter->drawArc(2*to.x()-pivot.x(),
                      2*pivot.y()-to.y(),
                      2*(pivot.x()-to.x()),
                      2*(to.y()-pivot.y()),270*16,90*16);
  }
  if (segdir==AD_RIGHT)
  {
      painter->drawArc(pivot.x(),
                       2*pivot.y()-to.y(),
                       2*(to.x()-pivot.x()),
                       2*(to.y()-pivot.y()),180*16,90*16);
  }
}

QPointF constructDirectedLine(const QPointF & point1,const QPointF & point2)
{
  ArrowDirection dir=direction(point1,point2);
  if (dir==AD_LEFT || dir==AD_RIGHT) return QPointF(point2.x(),point1.y());
  return QPointF(point1.x(),point2.y());
}
