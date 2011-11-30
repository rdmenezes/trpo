#include "commentline.h"
#include "diagram.h"
#include "arrowsegment.h"
#include "arrowpoint.h"
#include <QGraphicsScene>
#include <QPainter>

CommentLine::CommentLine(const QPointF & bindedpoint,const QPointF & freepoint)
{
    m_bindedpoint=new QPointF(bindedpoint);
    m_freepoint=freepoint;
    m_isbindedtoline=false;
}

CommentLine::CommentLine(QPointF * bindedpoint,const QPointF & freepoint)
{
  m_bindedpoint=bindedpoint;
  m_freepoint=freepoint;
  static_cast<ArrowPoint *>(bindedpoint)->attachAnnotation(this);
  m_isbindedtoline=true;
}

void CommentLine::deattachFromLine()
{
  if (m_isbindedtoline)
  {
      ArrowPoint * p=static_cast<ArrowPoint *>(m_bindedpoint);
      m_bindedpoint=new QPointF(m_bindedpoint->x(),m_bindedpoint->y());
      m_isbindedtoline=false;
      p->removeAnnotation(this);
  }
}

CommentLine::~CommentLine()
{
  if (!m_isbindedtoline)
      delete m_bindedpoint;
}

void CommentLine::die()
{
    m_diagram->removeAnnotationLine(this);
    this->scene()->removeItem(this);
}

#define D_LEFT 4
#define D_RIGHT 4
QRectF CommentLine::boundingRect() const
{
  if (!m_bindedpoint) return QRectF();
  QPointF * m_in=m_bindedpoint;
  QPointF * m_out=const_cast<QPointF*>(&m_freepoint);
  ArrowDirection dir=direction(*m_bindedpoint,m_freepoint);
  QRectF result;
  if (dir==AD_LEFT)
      result=QRectF(m_out->x(),m_in->y()-D_LEFT,m_in->x()-m_out->x(),D_LEFT+D_RIGHT);
  if (dir==AD_RIGHT)
      result=QRectF(m_in->x(),m_in->y()-D_LEFT,m_out->x()-m_in->x(),D_LEFT+D_RIGHT);
  if (dir==AD_TOP)
      result=QRectF(m_out->x()-D_LEFT,m_out->y(),D_LEFT+D_RIGHT,m_in->y()-m_out->y());
  if (dir==AD_BOTTOM)
      result=QRectF(m_out->x()-D_LEFT,m_in->y(),D_LEFT+D_RIGHT,m_out->y()-m_in->y());

  return result;
}


void CommentLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *widget)
{
    (void)option,widget;
    QPainterPath path;
    ArrowDirection dir=direction(*m_bindedpoint,m_freepoint);
    if (dir==AD_TOP || dir==AD_BOTTOM)    this->drawTopBottom(path);
    if (dir==AD_LEFT || dir==AD_RIGHT)   this->drawLeftRight(path);
    painter->drawPath(path);
}


void CommentLine::drawTopBottom(QPainterPath & path)
{
  double midy=(m_bindedpoint->y()+m_freepoint.y())/2.0;
  {
   QPointF p0(m_bindedpoint->x()+D_RIGHT,m_bindedpoint->y());
   QPointF p1(m_bindedpoint->x()-0.75*D_LEFT,midy);
   QPointF p2(m_bindedpoint->x()-0.5*D_LEFT,m_bindedpoint->y()*0.48+m_freepoint.y()*0.52);
   QPointF p3(m_bindedpoint->x(),midy);
   drawCubicCurve(p0,p1,p2,p3,0.50,0.75,path);
  }
  {
    QPointF p0(m_bindedpoint->x(),midy);
    QPointF p1(m_bindedpoint->x()+0.5*D_RIGHT,m_bindedpoint->y()*0.52+m_freepoint.y()*0.48);
    QPointF p2(m_bindedpoint->x()+0.75*D_RIGHT,midy);
    QPointF p3(m_freepoint.x()-D_LEFT,m_freepoint.y());
    drawCubicCurve(p0,p1,p2,p3,0.25,0.50,path);
  }
}



void CommentLine::drawLeftRight(QPainterPath & path)
{
 double midx=(m_bindedpoint->x()+m_freepoint.x())/2.0;
 {
  QPointF p0(m_bindedpoint->x(),m_freepoint.y()-D_LEFT);
  QPointF p1(m_bindedpoint->x()*0.52+m_freepoint.x()*0.48,m_freepoint.y()-0.75*D_LEFT);
  QPointF p2(m_bindedpoint->x()*0.48+m_freepoint.x()*0.52,m_freepoint.y()-0.50*D_LEFT);
  QPointF p3(midx,m_bindedpoint->y());
  drawCubicCurve(p0,p1,p2,p3,0.50,0.75,path);
 }
 {

   QPointF p0(midx,m_freepoint.y());
   QPointF p1(m_bindedpoint->x()*0.52+m_freepoint.x()*0.48,m_freepoint.y()+0.50*D_RIGHT);
   QPointF p2(midx,m_freepoint.y()+0.75*D_RIGHT);
   QPointF p3(m_freepoint.x(),m_freepoint.y()+D_RIGHT);
   drawCubicCurve(p0,p1,p2,p3,0.25,0.50,path);
 }
}


void CommentLine::drawCubicCurve(const QPointF & p0,
                               const QPointF & p1,
                               const QPointF & p2,
                               const QPointF & p3,
                               double t0, double t1,
                               QPainterPath & path)
{
    double t01=1.0-t0,t11=1.0-t1;
    double a0=t01*t01*t01,a1=3*t0*t01*t01,a2=3*t0*t0*t01,a3=t0*t0*t0;
    double b0=t11*t11*t11,b1=3*t1*t11*t11,b2=3*t1*t1*t11,b3=t1*t1*t1;
    QPointF F0=p1-p0*a0-p3*a3;
    QPointF F1=p2-p0*b0-p3*b3;
    double D=a1*b2-a2*b1;
    QPointF D1=F0*b2-F1*a2;
    QPointF D2=F1*a1-F0*b1;
    D1/=D;
    D2/=D;
    path.moveTo(p0);
    path.cubicTo(D1,D2,p3);
}

/*! Returns a type of segment
 */
int CommentLine::type() const
{
    return CommentLine::USERTYPE;
}

