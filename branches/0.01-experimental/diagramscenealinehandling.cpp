#include "diagramscene.h"
#include "arrowpoint.h"
#include "arrowsegment.h"
#include "alineitem.h"
#include "diagram.h"
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <math.h>

void DiagramScene::processAnnotationLineEscapePress(const QPointF & pos)
{
  (void)pos;
  m_alds=ALDS_SPECIFIEDNONE;
  m_aline_segment=NULL;
}

void DiagramScene::processAnnotationLinePointOnBlank(const QPointF & pos)
{
  m_alds=ALDS_SPECIFIEDFIRSTPOINT;
  m_aline_segment=NULL;
  m_aline_firstpoint=pos;
}

#define MINIMAL_LINE_LENGTH 5.0
void DiagramScene::processAnnotationLineSecondPointOnBlank(const QPointF & pos)
{
 QPointF second=constructDirectedLine(m_aline_firstpoint,pos);
 ArrowDirection dir=direction(m_aline_firstpoint,second);
 bool error=false;
 //Check, whether line oposes segment
 if (m_aline_segment)
 {
    ArrowDirection dir2=m_aline_segment->direction();
    bool equal= dir==dir2;
    bool horizontaloppose= (dir==AD_LEFT && dir2==AD_RIGHT) || (dir2==AD_LEFT && dir==AD_RIGHT);
    bool verticaloppose=   (dir==AD_TOP && dir2==AD_BOTTOM) || (dir2==AD_TOP && dir==AD_BOTTOM);
    error= equal && horizontaloppose && verticaloppose;
 }
 bool toosmall = (fabs(m_aline_firstpoint.x()-second.x())+
                  fabs(m_aline_firstpoint.y()-second.y()))<MINIMAL_LINE_LENGTH;
 bool cannotPlace =  !(m_diag->canPlaceAnnotationLine(&m_aline_firstpoint,&second));
 error= error || toosmall || cannotPlace;
 if (!error)
 {
    ALineItem * aline=NULL;
    if (m_aline_segment)
    {
      ArrowPoint * p=new ArrowPoint(m_aline_firstpoint.x(),m_aline_firstpoint.y());
      m_diag->addArrowPoint(p);
      ArrowSegment * seg1=new ArrowSegment(m_aline_segment->in(),p);
      ArrowSegment * seg2=new ArrowSegment(p,m_aline_segment->out());
      m_diag->addArrowSegment(seg1);
      m_diag->addArrowSegment(seg2);
      this->addItem(seg1);
      this->addItem(seg2);
      m_aline_segment->in()->tryRemoveSegment(m_aline_segment,false);
      m_aline_segment->out()->tryRemoveSegment(m_aline_segment,false);
      m_aline_segment->die();
      aline=new ALineItem(p,second);
    }
    if (!aline)
         aline=new ALineItem(m_aline_firstpoint,second);
    this->addItem(aline);
    m_diag->addAnnotationLine(aline);
    this->update();

    m_alds=ALDS_SPECIFIEDNONE;
    m_aline_segment=NULL;
 }
}

void DiagramScene::processAnnotationLineToBox(const QPointF & pos,BoxItem * box)
{
  ArrowPoint * p=new ArrowPoint(pos.x(),pos.y());
  BoxItemSide side=box->sideOfPoint(p);
  QRectF rect=box->boundingRect();
  if (side==BIS_LEFT) p->setX(rect.left());
  if (side==BIS_RIGHT) p->setX(rect.right());
  if (side==BIS_TOP)   p->setY(rect.top());
  if (side==BIS_BOTTOM) p->setY(rect.bottom());
  m_alds=ALDS_SPECIFIEDFIRSTPOINT;
  m_aline_segment=NULL;
  m_aline_firstpoint=QPointF(p->x(),p->y());
  delete p;
}

void DiagramScene::processAnnotationLineToSegment(const QPointF & pos,
                                                  ArrowSegment * seg)
{
  m_alds=ALDS_SPECIFIEDFIRSTPOINT;
  m_aline_firstpoint=pos;
  m_aline_segment=seg;
  ArrowDirection dir=seg->direction();
  if (dir==AD_TOP || dir==AD_BOTTOM) m_aline_firstpoint.setX(seg->in()->x());
  if (dir==AD_LEFT || dir==AD_RIGHT) m_aline_firstpoint.setY(seg->in()->y());
}

void DiagramScene::enterAnnotationLineResize(const QPointF & pos,
                                             ALineItem * item)
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
  bool toosmall=(fabs(beg.x()-newpos.x())+
                 fabs(beg.y()-newpos.y()))<MINIMAL_LINE_LENGTH;
  if (!toosmall || !nonequaldirection)
  {
      m_resizing_aline->end()->setX(newpos.x());
      m_resizing_aline->end()->setY(newpos.y());
      this->update();
  }
}

void DiagramScene::removeAnnotationLine(ALineItem * line)
{
 if (line->isAttachedToLine())
 {
     ArrowPoint * p=static_cast<ArrowPoint *>(line->begin());
     p->removeAnnotation(line);
 }
 m_diag->removeAnnotationLine(line);
 this->removeItem(line);
 this->update();
}
