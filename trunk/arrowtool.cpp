#include "arrowtool.h"
#include <math.h>

ArrowTool::ArrowTool()
{
    //Init drawing array
    m_drawarr[0]=&ArrowTool::drawZeroDirected;
    m_drawarr[1]=&ArrowTool::drawRightLeftDirected;
    m_drawarr[2]=&ArrowTool::drawHVDirected;
    m_drawarr[3]=&ArrowTool::drawVHDirected;
    m_drawarr[4]=&ArrowTool::drawTopBottomDirected;
    m_drawarr[5]=&ArrowTool::drawVHDirected;
    m_drawarr[6]=&ArrowTool::drawHVDirected;
    m_drawarr[7]=&ArrowTool::drawRightLeftDirected;
    m_drawarr[8]=&ArrowTool::drawHVDirected;
    m_drawarr[9]=&ArrowTool::drawVHDirected;
    m_drawarr[10]=&ArrowTool::drawTopBottomDirected;
    m_drawarr[11]=&ArrowTool::drawVHDirected;
    m_drawarr[12]=&ArrowTool::drawHVDirected;
}


ArrowTool::~ArrowTool()
{
}

void   ArrowTool::initState()
{
    m_state=ATS_START;
}



void ArrowTool::clearState()
{
  //static_cast<MainWindow*>(m_scene->view()->window())->setActionText("");
  //if (m_obj)
  //  this->m_scene->removeItem(m_obj);
}


int ArrowTool::clockwiseDirection(const QPointF & pos,bool canReturnZero)
{
   QPointF pivot=m_points[0];
   //! If it's the same point return zero
   if (canReturnZero
       && fabs(pos.x()-pivot.x())<ARROW_SENSIVITY_X
       && fabs(pos.y()-pivot.y())<ARROW_SENSIVITY_Y)
   {
       return 0;
   }
   //! If this is vertical line with top direction return 4
   if (pos.y()<pivot.y() && fabs(pos.x()-pivot.x())<ARROW_SENSIVITY_X)
   {
       return 4;
   }
   //! If this is vertical line with bottom direction return 10
   if (pos.y()>pivot.y() && fabs(pos.x()-pivot.x())<ARROW_SENSIVITY_X)
   {
       return 10;
   }
   //! If this is horizontal line with right direction return 1
   if (pos.x()>pivot.x() && fabs(pos.y()-pivot.y())<ARROW_SENSIVITY_Y)
   {
       return 1;
   }
   //! If this is horizontal line with left direction return 7
   if (pos.x()<pivot.x() && fabs(pos.y()-pivot.y())<ARROW_SENSIVITY_Y)
   {
       return 7;
   }
   //! Right-top direction
   if (pos.x()>pivot.x() && pos.y()<pivot.y() &&  pos.x()-pivot.x()>pivot.y()-pos.y() )
   {
       return 2;
   }
   //! Top-right direction
   if (pos.x()>pivot.x() && pos.y()<pivot.y() &&  pos.x()-pivot.x()<=pivot.y()-pos.y() )
   {
       return 3;
   }
   //! Top-left direction
   if (pos.x()<=pivot.x() && pos.y()<pivot.y() &&  pivot.x()-pos.x()<=pivot.y()-pos.y() )
   {
       return 5;
   }
   //! Left-top direction
   if (pos.x()<=pivot.x() && pos.y()<pivot.y() &&  pivot.x()-pos.x()>pivot.y()-pos.y() )
   {
       return 6;
   }
   //! Left-bottom direction
   if (pos.x()<=pivot.x() && pos.y()>pivot.y() &&  pivot.x()-pos.x()>pos.y()-pivot.y() )
   {
       return 8;
   }
   //! Bottom-left direction
   if (pos.x()<=pivot.x() && pos.y()>pivot.y() &&  pivot.x()-pos.x()<=pos.y()-pivot.y() )
   {
       return 9;
   }
   //! Bottom-right direction
   if (pos.x()>pivot.x() && pos.y()>pivot.y() &&  pos.x()-pivot.x()<=pos.y()-pivot.y() )
   {
       return 11;
   }
   //! Right-bottom direction
   if (pos.x()>pivot.x() && pos.y()>pivot.y() &&  pos.x()-pivot.x()>pos.y()-pivot.y() )
   {
       return 12;
   }
   //! If control reached this point, it's some kind of bug
   return 13;
}


void ArrowTool::makeOneLineVisible()
{
  m_preview[0]->setVisible(true);
  m_preview[1]->setVisible(false);
  m_preview[0]->model()->remove(m_preview[1]->model());
  m_preview[1]->model()->remove(m_preview[0]->model());
}


void ArrowTool::makeAllVisible()
{
    m_preview[0]->setVisible(true);
    m_preview[1]->setVisible(true);
    if (!(m_preview[0]->model()->hasConnected(m_preview[1]->model())))
    {
        m_preview[0]->model()->addConnector(m_preview[1]->model(),1,C_OUTPUT);
        m_preview[1]->model()->addConnector(m_preview[0]->model(),0,C_INPUT);
    }
}

void    ArrowTool::drawZeroDirected(const QPointF & p1, const QPointF & p2)
{
   m_preview[0]->setVisible(false);
   m_preview[1]->setVisible(false);
   m_scene->update();
}

void  ArrowTool::drawRightLeftDirected(const QPointF &p1, const QPointF &p2)
{
    makeOneLineVisible();
    m_preview[0]->model()->setLine(p1.x(),p1.y(),p2.x(),p1.y());
    m_scene->update();
}

void ArrowTool::drawTopBottomDirected(const QPointF &p1, const QPointF &p2)
{
    makeOneLineVisible();
    m_preview[0]->model()->setLine(p1.x(),p1.y(),p1.x(),p2.y());
    m_scene->update();
}


void ArrowTool::drawHVDirected(const QPointF &p1, const QPointF &p2)
{
    makeAllVisible();
    m_preview[0]->model()->setLine(p1.x(),p1.y(),p2.x(),p1.y());
    m_preview[1]->model()->setLine(p2.x(),p1.y(),p2.x(),p2.y());
    m_scene->update();
}

void ArrowTool::drawVHDirected(const QPointF &p1, const QPointF &p2)
{
    makeAllVisible();
    m_preview[0]->model()->setLine(p1.x(),p1.y(),p1.x(),p2.y());
    m_preview[1]->model()->setLine(p1.x(),p2.y(),p1.y(),p2.y());
    m_scene->update();
}


QVector<int> ArrowTool::getClickableItems()
{
    QVector<int> result;
    result<<IsBox<<IsArrow;
    return result;
}

QVector<int> ArrowTool::getReleaseableItems()
{
    return QVector<int>();
}
QVector<int> ArrowTool::getKeyDownItems()
{
    QVector<int> v; v<<IsBox<<IsFreeComment<<IsCommentLine<<IsArrow;
    return v;
}


bool ArrowTool::onClick(const QPointF &p, QGraphicsItem * /* item */)
{

  return true;
}

bool ArrowTool::onRelease(const QPointF & /* p */, QGraphicsItem * /* item */)
{
  return true;
}

bool ArrowTool::onKeyDown(QKeyEvent * /* event */, QGraphicsItem * /* item */)
{
  return true;
}

void ArrowTool::onMove(const QPointF & /* lastpos */ , const QPointF &pos)
{
  //Update preview only if first point specified
  if (m_state==ATS_FIRSTPOINT)
  {

  }
}

