#include "freecommenttool.h"
#include "mainwindow.h"
#include "diagramscene.h"
#include "attachedcomment.h"

FreeCommentTool::FreeCommentTool()
{
}


FreeCommentTool::~FreeCommentTool()
{

}



void FreeCommentTool::clearState()
{
  static_cast<MainWindow*>(m_scene->view()->window())->setActionText("");
  if (m_obj)
  {
    delete m_obj->parentComment();
    this->m_scene->removeItem(m_obj);
  }
}

void FreeCommentTool::initState()
{
  m_obj=new FreeComment(QPointF(0,0),m_diagram);
  AttachedComment * parent=new AttachedComment(m_obj,NULL);
  m_obj->setParentComment(parent);
  this->m_scene->addItem(m_obj);
  static_cast<MainWindow*>(m_scene->view()->window())->setActionText("Click, where you want to place comment...");
}


QVector<int> FreeCommentTool::getClickableItems()
{
    return QVector<int>();
}

QVector<int> FreeCommentTool::getReleaseableItems()
{
    return QVector<int>();
}
QVector<int> FreeCommentTool::getKeyDownItems()
{
    QVector<int> v; v<<IsFreeComment;
    return v;
}


bool FreeCommentTool::onClick(const QPointF &p, QGraphicsItem * /* item */)
{
  QPointF pp=p;
  if (m_obj)
  {
      QRectF rct=m_obj->boundingRect();
      pp-=QPointF(rct.width()/2,rct.height()/2);
      m_obj->setPos(pp);
      if (m_diagram->canPlace(m_obj))
      {
          m_diagram->add(m_obj->parentComment());
          m_obj=new FreeComment(p,m_diagram);
          AttachedComment * parent=new AttachedComment(m_obj,NULL);
          m_obj->setParentComment(parent);
          m_scene->addItem(m_obj);
      }
      m_scene->update();
  }
  return true;
}

bool FreeCommentTool::onRelease(const QPointF & /* p */, QGraphicsItem * /* item */)
{
  return true;
}

bool FreeCommentTool::onKeyDown(QKeyEvent * /* event */, QGraphicsItem * /* item */)
{
  return true;
}

void FreeCommentTool::onMove(const QPointF & /* lastpos */ , const QPointF &pos)
{
    if (m_obj)
    {
        QRectF rct=m_obj->boundingRect();
        m_obj->setPos(pos-QPointF(rct.width()/2,rct.height()/2));
        m_scene->update();
    }
}
