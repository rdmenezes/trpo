#include "selecttool.h"
#include "diagramscene.h"
#include "mainwindow.h"
#include "keytest.h"
#include "arrow.h"

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
    QVector<int> v; v<<IsBox<<IsFreeComment<<IsArrow;
    return v;
}


bool SelectTool::onClick(const QPointF & /* p */, QGraphicsItem * /* item */)
{
 return true;
}

bool SelectTool::onRelease(const QPointF & /* p */, QGraphicsItem * /* item */ )
{
  return true;
}

#define TEXT_EDIT_BORDER 5

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

void SelectTool::onMove(const QPointF & /* lastpos */ , const QPointF & /* pos */)
{

}

