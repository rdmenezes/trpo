#include "selecttool.h"
#include "diagramscene.h"
#include "mainwindow.h"


BoxMoving::BoxMoving(Diagram * diagram,
                     const QPointF & clickpos,
                     Box * obj)
          :DynamicEditState(diagram,clickpos),BoxChangingData(obj)
{
  static_cast<MainWindow*>(m_diagram->scene()->view()->window())->setActionText("Release box where you want to place it...");
}


void BoxMoving::clearState()
{
  m_box->setPos(m_boxpos);
  m_box->scene()->update();
}


void BoxMoving::onRelease(const QPointF &p)
{
  QPointF position=p-m_clickpos;
  if (fabs(position.x())>fabs(position.y()))
        position.setY(0);
  else
        position.setX(0);
  m_box->setPos(m_boxpos);
  if (m_box->canMove(position))
  {
    m_box->moveBy(position);
    m_diagram->commit();
  }
  this->m_box->scene()->update();
}

void BoxMoving::onMove(const QPointF &p)
{
    QPointF position=p-m_clickpos;
    if (fabs(position.x())>fabs(position.y()))
          position.setY(0);
    else
          position.setX(0);
    m_box->setPos(m_boxpos);
    if (m_box->canMove(position))
    {
      m_box->setPos(m_boxpos+position);
    }
    this->m_box->scene()->update();
}
