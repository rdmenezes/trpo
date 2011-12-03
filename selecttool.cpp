#include "selecttool.h"
#include "diagramscene.h"
#include "mainwindow.h"
#include "keytest.h"

SelectTool::SelectTool()
{
}


SelectTool::~SelectTool()
{

}

void SelectTool::clearState()
{
  static_cast<MainWindow*>(m_scene->view()->window())->setActionText("");
}

void SelectTool::initState()
{
  static_cast<MainWindow*>(m_scene->view()->window())->setActionText("Using select instrument...");
}


QVector<int> SelectTool::getClickableItems()
{
    return QVector<int>();
}

QVector<int> SelectTool::getReleaseableItems()
{
    return QVector<int>();
}
QVector<int> SelectTool::getKeyDownItems()
{
    QVector<int> v; v<<IsBox<<IsFreeComment;
    return v;
}


bool SelectTool::onClick(const QPointF &p, QGraphicsItem * /* item */)
{
 return true;
}

bool SelectTool::onRelease(const QPointF & /* p */, QGraphicsItem * /* item */ )
{
  return true;
}

bool SelectTool::onKeyDown(QKeyEvent *  event , QGraphicsItem *  item )
{
  if (item)
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
  }
  return true;
}

void SelectTool::onMove(const QPointF & /* lastpos */ , const QPointF &pos)
{

}

