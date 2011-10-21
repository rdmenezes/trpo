#include "alineitem.h"
#include "diagram.h"
#include "arrowsegment.h"
#include "arrowpoint.h"
#include <QGraphicsScene>

ALineItem::ALineItem(const QPointF & bindedpoint,const QPointF & freepoint)
{
    m_bindedpoint=new QPointF(bindedpoint);
    m_freepoint=freepoint;
    m_isbindedtoline=false;
}

ALineItem::ALineItem(QPointF * bindedpoint,const QPointF & freepoint)
{
  m_bindedpoint=bindedpoint;
  m_freepoint=freepoint;
  m_isbindedtoline=true;
}

void ALineItem::deattachFromLine()
{
  if (m_isbindedtoline)
  {
      ArrowPoint * p=static_cast<ArrowPoint *>(m_bindedpoint);
      m_bindedpoint=new QPointF(m_bindedpoint->x(),m_bindedpoint->y());
      m_isbindedtoline=false;
      p->removeAnnotation(this);
  }
}

ALineItem::~ALineItem()
{
  if (!m_isbindedtoline)
      delete m_bindedpoint;
}

void ALineItem::die()
{
    m_diagram->removeAnnotationLine(this);
    this->scene()->removeItem(this);
}

#define D_LEFT 4
#define D_RIGHT 4
QRectF ALineItem::boundingRect() const
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
