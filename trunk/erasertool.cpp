#include "erasertool.h"
#include "diagramobject.h"
#include "itemtypes.h"
#include "diagramscene.h"
#include "mainwindow.h"

void   EraserTool::clearState()
{
  static_cast<MainWindow*>(m_scene->view()->window())->setActionText("");
}


void EraserTool::initState()
{
 static_cast<MainWindow*>(m_scene->view()->window())->setActionText("Click on item, which you wish to disappear...");
}



QVector<int> EraserTool::getClickableItems()
{
    QVector<int> v;
    v<<IsDefaultItem<<IsBox<<IsArrow<<IsCommentLine<<IsFreeComment;
    return v;
}

QVector<int> EraserTool::getReleaseableItems()
{
    return QVector<int>();
}
QVector<int> EraserTool::getKeyDownItems()
{
    return QVector<int>();
}


bool EraserTool::onClick(const QPointF & /* p */, QGraphicsItem *  item )
{
  if (item)
  {
        if (item->type()==IsDefaultItem)
            delete item;
        else
        {
            m_diagram->remove(static_cast<DiagramObject*>(item));
            m_diagram->commit();
        }
  }
  return true;
}

bool EraserTool::onRelease(const QPointF & /* p */, QGraphicsItem * /* item */)
{
  return true;
}

bool EraserTool::onKeyDown(QKeyEvent *  /*event*/ , QGraphicsItem *  /*item*/ )
{

  return true;
}

void EraserTool::onMove(const QPointF & /* lastpos */ , const QPointF & /*pos*/)
{

}
