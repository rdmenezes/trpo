#include "diagramscene.h"
#include "arrowpoint.h"
#include "arrowsegment.h"
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>


void DiagramScene::processArrowEscapePress(const QPointF & pos)
{
  (void)pos;
  if (m_last_arrow_point->isSeparated())
      m_last_arrow_point->die();
  m_arrow_state=AES_NONE;
  m_last_arrow_point=NULL;
}

void DiagramScene::processArrowClickOnBlankSpace(QGraphicsSceneMouseEvent * event)
{
    if (m_arrow_state==AES_NONE)
    {
        m_last_arrow_point=new ArrowPoint(event->scenePos().x(),event->scenePos().y());
        m_arrow_state=AES_EDIT;
        m_diag->addArrowPoint(m_last_arrow_point);
    }
    else
    {
        QPointF point2=constructDirectedLine(*m_last_arrow_point,event->scenePos());
        if (tooSmall(m_last_arrow_point,point2)) return;
        ArrowPoint * p=new ArrowPoint(point2.x(),point2.y());
        bool placementtest=m_diag->canPlaceSegment(m_last_arrow_point,p,NULL);
        bool correctness=!(m_last_arrow_point->hasOppositeSegment(m_last_arrow_point,p));
        if (placementtest && correctness)
        {
            m_diag->addArrowPoint(p);
            ArrowSegment * seg=new ArrowSegment(m_last_arrow_point,p);
            m_diag->addArrowSegment(seg);
            this->addItem(seg);
            m_last_arrow_point->update();
            seg->update();
            m_last_arrow_point=p;
        }
        else delete p;
    }
}

void DiagramScene::processArrowClickOnBox(QGraphicsSceneMouseEvent * event,
                                          BoxItem * box)
{

}

void DiagramScene::processArrowClickOnLine(QGraphicsSceneMouseEvent * event,
                                           ArrowSegment * seg)
{

}
