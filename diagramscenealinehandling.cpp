#include "diagramscene.h"
#include "arrowpoint.h"
#include "arrowsegment.h"
#include "commentline.h"
#include "diagram.h"
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <math.h>

void DiagramScene::enterAnnotationLineResize(const QPointF & pos,
                                             CommentLine * item)
{
 (void)pos;
  m_dragstate=DS_ALINE_RESIZE;
  m_resizing_aline=item;
}

void DiagramScene::leaveAnnotationLineResize(const QPointF & pos)
{
  QPointF beg=*(m_resizing_aline->begin());
  ArrowDirection dir=direction(*(m_resizing_aline->begin()),
                               *(m_resizing_aline->end()));
  QPointF newpos=pos;
  if (dir==AD_TOP || dir==AD_BOTTOM) newpos.setX(m_resizing_aline->begin()->x());
  else                               newpos.setY(m_resizing_aline->begin()->y());

  bool nonequaldirection=  dir!=direction(*(m_resizing_aline->begin()),newpos);
  bool toosmall=false;
  //bool toosmall=(fabs(beg.x()-newpos.x())+
  //               fabs(beg.y()-newpos.y()))<MINIMAL_LINE_LENGTH;
  if (!toosmall || !nonequaldirection)
  {
      m_resizing_aline->end()->setX(newpos.x());
      m_resizing_aline->end()->setY(newpos.y());
      this->update();
  }
  m_dragstate=DS_NONE;
  m_resizing_aline=NULL;
}
