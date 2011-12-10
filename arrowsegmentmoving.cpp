#include "selecttool.h"
#include  "diagramscene.h"
#include "mainwindow.h"


ArrowSegmentMoving::ArrowSegmentMoving(Diagram * diagram,
                                       const QPointF & clickpos,
                                       Arrow * obj)
                   :DynamicEditState(diagram,clickpos),
                    ObjectChangingData()

{
    m_obj=obj;
    m_startingpos=*(obj->model());
    m_direction=obj->model()->direction();
    m_collinear=obj->model()->canMoveCollinear();
    static_cast<MainWindow*>(m_diagram->scene()->view()->window())->setActionText("Release arrow where you want to place it...");
}


void ArrowSegmentMoving::clearState()
{
  m_obj->model()->setLine(m_startingpos.x1(),m_startingpos.y1(),m_startingpos.x2(),m_startingpos.y2());
  m_obj->regenerate();
}


void ArrowSegmentMoving::onRelease(const QPointF &p)
{
  QPointF position=p-m_clickpos;
  if (!m_collinear)
  {
      if (m_direction==D_LEFT || m_direction==D_RIGHT) position.setX(0);
      else  position.setY(0);
  }
  else
  {
      if (fabs(position.x())>fabs(position.y()))
          position.setY(0);
      else
          position.setX(0);
  }
  m_obj->model()->setLine(m_startingpos.x1(),m_startingpos.y1(),m_startingpos.x2(),m_startingpos.y2());
  if (m_obj->model()->testCanMove(position))
  {
    QLineF line=m_startingpos;
    line.setP1(line.p1()+position);
    line.setP2(line.p2()+position);
    m_obj->model()->setLine(line.x1(),line.y1(),line.x2(),line.y2());
    m_obj->model()->moveSelfRegeneratingOrResizing();
    m_obj->setDrawInput(true);
    m_diagram->commit();
  }
  m_obj->regenerate();
  this->m_obj->scene()->update();
}

void ArrowSegmentMoving::onMove(const QPointF &p)
{
    QPointF position=p-m_clickpos;
    if (!m_collinear)
    {
        if (m_direction==D_LEFT || m_direction==D_RIGHT) position.setX(0);
        else  position.setY(0);
    }
    else
    {
        if (fabs(position.x())>fabs(position.y()))
            position.setY(0);
        else
            position.setX(0);
    }
    m_obj->model()->setLine(m_startingpos.x1(),m_startingpos.y1(),m_startingpos.x2(),m_startingpos.y2());
    if (m_obj->model()->testCanMove(position))
    {
      QLineF line=m_startingpos;
      QPointF delta=position;
      line.setP1(line.p1()+delta);
      line.setP2(line.p2()+delta);
      m_obj->model()->setLine(line.x1(),line.y1(),line.x2(),line.y2());
      m_obj->setDrawInput(false);
    }
    m_obj->regenerate();
    this->m_obj->scene()->update();
}
