#include "alabelitem.h"
#include <QPainter>
ALabelItem::ALabelItem()
{
}


QRectF ALabelItem::boundingRect() const
{
  return m_rect;
}

void ALabelItem::paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget)
{
    painter->drawText(m_rect,m_string);
    (void)option,widget;
}
