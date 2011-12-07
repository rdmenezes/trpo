#include "selecttool.h"
#include "diagramscene.h"
#include "mainwindow.h"
#include "keytest.h"
#include "arrow.h"
#include "attachedcomment.h"

void FreeCommentMove::restorePosition()
{
  if (m_line)
  {
     m_line->setLine(m_startlinepos.x1(),m_startlinepos.y1(),m_startlinepos.x2(),m_startlinepos.y2());
  }
  m_obj->setPos(m_startcommentpos.x(),m_startcommentpos.y());
  m_obj->setPosWithoutCheck(m_startcommentpos);
}

SelectTool::SelectTool()
{
}


SelectTool::~SelectTool()
{

}

void SelectTool::clearState()
{
  static_cast<MainWindow*>(m_scene->view()->window())->setActionText("");
  if (m_state==STS_MOVEFREECOMMENT)
  {
     m_fcm.restorePosition();
     m_scene->update();
     m_state=STS_NONE;
  }
}

void SelectTool::initState()
{
  static_cast<MainWindow*>(m_scene->view()->window())->setActionText("Using select instrument...");
  m_state=STS_NONE;
}


QVector<int> SelectTool::getClickableItems()
{
    QVector<int> v; v<<IsFreeComment;
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
 if (item && m_state==STS_NONE)
 {
     if (item->type()==IsFreeComment)
     {
         m_fcm.m_clickpos=p;
         m_fcm.m_obj=static_cast<FreeComment*>(item);
         m_fcm.m_line=m_fcm.m_obj->parentComment()->line();
         if (m_fcm.m_line)
         {
             m_fcm.m_startlinepos=QLineF(m_fcm.m_line->in(),m_fcm.m_line->out());
         }
         m_fcm.m_startcommentpos=m_fcm.m_obj->pos();
         m_state=STS_MOVEFREECOMMENT;
         static_cast<MainWindow*>(m_scene->view()->window())->setActionText("Release comment where you would place it...");
     }
 }
 return true;
}

#define FCM_MOVE_SENSIVITY 2

bool SelectTool::onRelease(const QPointF &  p , QGraphicsItem * /* item */ )
{
  if (m_state==STS_MOVEFREECOMMENT)
  {
     if (fabs(p.x()-m_fcm.m_startcommentpos.x())<=FCM_MOVE_SENSIVITY && fabs(p.y()-m_fcm.m_startcommentpos.y())<FCM_MOVE_SENSIVITY)
     {
         m_fcm.restorePosition();
     }
     else
     {
        QPointF newpos=m_fcm.m_startcommentpos+p-m_fcm.m_clickpos;
        if (m_fcm.m_obj->checkPos(newpos))
        {
            m_fcm.m_obj->setPosWithoutCheck(newpos);
        }
        else m_fcm.restorePosition();
     }
     if (m_fcm.m_line)
            Q_ASSERT( m_fcm.m_line->model()->getConnected(C_INPUT).size() );
     static_cast<MainWindow*>(m_scene->view()->window())->setActionText("Using select instrument...");
     this->m_scene->update();
     this->m_state=STS_NONE;
  }
  return true;
}

#define TEXT_EDIT_BORDER 5

bool SelectTool::onKeyDown(QKeyEvent *  event , QGraphicsItem *  item )
{
  if (item && m_state==STS_NONE)
  {
    //If we are pressed it on box
    if (item->type()==IsBox)
    {
        //Decrement number
        if (event->key()==Qt::Key_Left)
        {
            m_diagram->changeBlockNumber(-1,static_cast<Box*>(item));
        }
        //Increment number
        else if (event->key()==Qt::Key_Right)
        {
            m_diagram->changeBlockNumber(1,static_cast<Box*>(item));
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
        this->m_scene->update();
    }
    if (item->type()==IsArrow && event->key()==Qt::Key_E)
    {
        static_cast<Arrow*>(item)->toggleTunneledEnd();
        this->m_scene->update();
    }
  }
  return true;
}

void SelectTool::onMove(const QPointF & /* lastpos */ , const QPointF &  pos )
{
  if (m_state==STS_MOVEFREECOMMENT)
  {
      if (fabs(pos.x()-m_fcm.m_startcommentpos.x())<=FCM_MOVE_SENSIVITY && fabs(pos.y()-m_fcm.m_startcommentpos.y())<FCM_MOVE_SENSIVITY)
      {
          m_fcm.restorePosition();
      }
      else
      {
         QPointF newpos=m_fcm.m_startcommentpos+pos-m_fcm.m_clickpos;
         m_fcm.m_obj->setPosWithoutCheck(newpos);
      }
  }
}

