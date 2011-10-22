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
 bool canPlace =  m_diag->canPlaceAnnotationLine(&m_aline_firstpoint,&second);
 error= error || toosmall || canPlace;
 if (error)
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

}

void DiagramScene::processAnnotationLineToSegment(const QPointF & pos,
                                                  ArrowSegment * seg)
{

}

void DiagramScene::enterAnnotationLineResize(const QPointF & pos,
                                             ALineItem * item)
{

}

void DiagramScene::leaveAnnotationLineResize(const QPointF & pos)
{

}

void DiagramScene::removeAnnotationLine(ALineItem * line)
{

}
