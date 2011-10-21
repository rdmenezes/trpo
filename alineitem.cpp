#include "alineitem.h"
#include "diagram.h"
#include <QGraphicsScene>

ALineItem::ALineItem(const QPointF & bindedpoint,const QPointF & freepoint)
{
    m_bindedpoint=new QPointF(bindedpoint);
    m_freepoint=freepoint;
    m_isbindedtoline=false;
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
