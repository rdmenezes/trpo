#include "selecttool.h"
#include "diagramscene.h"
#include "mainwindow.h"

#define CORNER_SENSIVITY 5

BlockCorner determineCorner(const QPointF & clickpos,const QRectF & r)
{
  QPointF test[4]=       {r.topLeft(),r.topRight(),r.bottomLeft(),r.bottomRight()};
  BlockCorner corners[4]={BC_UPPERLEFT,BC_UPPERRIGHT,BC_LOWERLEFT,BC_LOWERRIGHT};
  for (int i=0;i<4;i++)
  {
      if (fabs(test[i].x()-clickpos.x())<CORNER_SENSIVITY && fabs(test[i].y()-clickpos.y())<CORNER_SENSIVITY )
          return corners[i];
  }
  return BC_CENTER;
}


BoxResize::BoxResize(Diagram * diagram,
                     const QPointF & clickpos,
                     Box * obj, BlockCorner bc)
          :DynamicEditState(diagram,clickpos),BoxChangingData(obj)
{
  m_bc=bc;
  m_boxrect=obj->collisionRect();
  if (bc==BC_LOWERLEFT) m_cornerstartingpos=m_boxrect.bottomLeft();
  if (bc==BC_LOWERRIGHT) m_cornerstartingpos=m_boxrect.bottomRight();
  if (bc==BC_UPPERLEFT) m_cornerstartingpos=m_boxrect.topLeft();
  if (bc==BC_UPPERRIGHT) m_cornerstartingpos=m_boxrect.topRight();
   static_cast<MainWindow*>(m_diagram->scene()->view()->window())->setActionText("Release box corner to resize it");
}

void BoxResize::clearState()
{
    m_box->setRect(m_boxrect);
}


void BoxResize::onMove(const QPointF &p)
{
    QPointF newcorner=p-m_clickpos+m_cornerstartingpos;
    m_box->setRect(m_boxrect);
    if (m_box->canResize(m_bc,newcorner))
    {
        QRectF newrect=resize(m_boxrect,m_bc,newcorner);
        m_box->setRect(newrect);
    }
}


void BoxResize::onRelease(const QPointF &p)
{
    QPointF newcorner=p-m_clickpos+m_cornerstartingpos;
    m_box->setRect(m_boxrect);
    if (m_box->canResize(m_bc,newcorner))
    {
        m_box->resize(m_bc,newcorner);
        m_diagram->commit();
    }
}
