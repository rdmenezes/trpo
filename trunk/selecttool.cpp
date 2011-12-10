#include "selecttool.h"
#include "diagramscene.h"
#include "mainwindow.h"
#include "keytest.h"
#include "arrow.h"
#include "attachedcomment.h"


SelectTool::SelectTool()
{
    m_state=NULL;
}


SelectTool::~SelectTool()
{

}




void SelectTool::clearState()
{
  static_cast<MainWindow*>(m_scene->view()->window())->setActionText("");
  if (m_state)
  {
      m_state->clearState();
      delete m_state;
      m_state=NULL;
  }
}

void SelectTool::initState()
{
  static_cast<MainWindow*>(m_scene->view()->window())->setActionText("Using select instrument...");
  m_state=NULL;
}


QVector<int> SelectTool::getClickableItems()
{
    QVector<int> v; v<<IsFreeComment<<IsArrow<<IsBox;
    return v;
}

QVector<int> SelectTool::getReleaseableItems()
{
    return QVector<int>();
}
QVector<int> SelectTool::getKeyDownItems()
{
    QVector<int> v; v<<IsBox<<IsFreeComment<<IsArrow;
    return v;
}


bool SelectTool::onClick(const QPointF &  p , QGraphicsItem *  item )
{
 if (item && !m_state)
 {
     if (item->type()==IsFreeComment)
     {
        m_state=new FreeCommentMoving(m_diagram,p,
                                      static_cast<FreeComment*>(item));
     }
     if (item->type()==IsArrow)
     {
         m_state=new ArrowSegmentMoving(m_diagram,p,static_cast<Arrow*>(item));
     }
     if (item->type()==IsBox)
     {
         BlockCorner corner=determineCorner(p,static_cast<Box*>(item)->collisionRect());
         if (corner==BC_CENTER)
             m_state=new BoxMoving(m_diagram,p,static_cast<Box*>(item));
         else
             m_state=new BoxResize(m_diagram,p,static_cast<Box*>(item),corner);
     }
 }
 return true;
}


#define FCM_MOVE_SENSIVITY 2

bool SelectTool::onRelease(const QPointF &  p , QGraphicsItem * /* item */ )
{
  if (m_state)
  {
     m_state->onRelease(p);
     delete m_state;
     m_state=NULL;
     static_cast<MainWindow*>(m_scene->view()->window())->setActionText("Using select instrument...");
  }
  return true;
}

#define TEXT_EDIT_BORDER 5

bool SelectTool::onKeyDown(QKeyEvent *  event , QGraphicsItem *  item )
{
  if (item && !m_state)
  {
    //If we are pressed it on box
    if (item->type()==IsBox)
    {
        //Decrement number
        if (event->key()==Qt::Key_Left)
        {
            m_diagram->changeBlockNumber(-1,static_cast<Box*>(item));
            m_diagram->commit();
        }
        //Increment number
        else if (event->key()==Qt::Key_Right)
        {
            m_diagram->changeBlockNumber(1,static_cast<Box*>(item));
            m_diagram->commit();
        }
        //Change text
        else if (isTextEditKey(event))
        {
            m_scene->toggleEditMode(true,static_cast<Box*>(item)->collisionRect(),
                                         event,
                                         static_cast<Box*>(item));
        }
    }
    if (item->type()==IsFreeComment && isTextEditKey(event))
    {
        QRectF rect=static_cast<FreeComment*>(item)->collisionRect();
        rect.moveLeft(rect.left()-TEXT_EDIT_BORDER);
        rect.moveTop(rect.top()-TEXT_EDIT_BORDER);
        rect.setWidth(rect.width()+2*TEXT_EDIT_BORDER);
        rect.setHeight(rect.height()+2*TEXT_EDIT_BORDER);
        m_scene->toggleEditMode(true,rect,
                                     event,
                                     static_cast<FreeComment*>(item));
    }
    if (item->type()==IsArrow && event->key()==Qt::Key_B)
    {
        static_cast<Arrow*>(item)->toggleTunneledBegin();
        m_diagram->commit();
        this->m_scene->update();
    }
    if (item->type()==IsArrow && event->key()==Qt::Key_E)
    {
        static_cast<Arrow*>(item)->toggleTunneledEnd();
        m_diagram->commit();
        this->m_scene->update();
    }
  }
  return true;
}

void SelectTool::onMove(const QPointF & /* lastpos */ , const QPointF &  pos )
{
  if (m_state)
  {
      m_state->onMove(pos);
  }
}


FreeCommentMoving::FreeCommentMoving(Diagram *diagram, const QPointF &clickpos, FreeComment *obj)
                  : DynamicEditState(diagram,clickpos)
{
    m_obj=obj;
    m_line=m_obj->parentComment()->line();
    if (m_line)
    {
        m_startlinepos=QLineF(m_line->in(),m_line->out());
    }
    m_startcommentpos=m_obj->pos();
    static_cast<MainWindow*>(m_diagram->scene()->view()->window())->setActionText("Release comment where you would place it...");
}

void FreeCommentMoving::restorePosition()
{
  if (m_line)
  {
     m_line->setLine(m_startlinepos.x1(),m_startlinepos.y1(),m_startlinepos.x2(),m_startlinepos.y2());
  }
  m_obj->setPos(m_startcommentpos.x(),m_startcommentpos.y());
  m_obj->setPosWithoutCheck(m_startcommentpos);
}

void  FreeCommentMoving::clearState()
{
  this->restorePosition();
  this->m_diagram->scene()->update();
}

void  FreeCommentMoving::onMove(const QPointF & pos)
{
    if (fabs(pos.x()-m_startcommentpos.x())<=FCM_MOVE_SENSIVITY
        && fabs(pos.y()-m_startcommentpos.y())<FCM_MOVE_SENSIVITY)
    {
        restorePosition();
    }
    else
    {
       QPointF newpos=m_startcommentpos+pos-m_clickpos;
       m_obj->setPosWithoutCheck(newpos);
    }
}

void FreeCommentMoving::onRelease(const QPointF &p)
{
    if (fabs(p.x()-m_startcommentpos.x())<=FCM_MOVE_SENSIVITY
        && fabs(p.y()-m_startcommentpos.y())<FCM_MOVE_SENSIVITY)
    {
        restorePosition();
    }
    else
    {
       QPointF newpos=m_startcommentpos+p-m_clickpos;
       if (m_obj->checkPos(newpos))
       {
           m_obj->setPosWithoutCheck(newpos);
           m_diagram->commit();
       }
       else restorePosition();
    }
    if (m_line)
           Q_ASSERT( m_line->model()->getConnected(C_INPUT).size() );
    this->m_diagram->scene()->update();
}
