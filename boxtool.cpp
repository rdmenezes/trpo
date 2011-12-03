#include "boxtool.h"
#include "mainwindow.h"
#include "diagramscene.h"

BoxTool::BoxTool()
{
}

BoxTool::~BoxTool()
{
}


void BoxTool::clearState()
{
  static_cast<MainWindow*>(m_scene->view()->window())->setActionText("");
  if (m_obj)
    this->m_scene->removeItem(m_obj);
}

void BoxTool::initState()
{
  m_obj=new Box(QPointF(0,0),m_diagram);
  this->m_scene->addItem(m_obj);
  static_cast<MainWindow*>(m_scene->view()->window())->setActionText("Click, where you want to place block...");
}


QVector<int> BoxTool::getClickableItems()
{
    return QVector<int>();
}

QVector<int> BoxTool::getReleaseableItems()
{
    return QVector<int>();
}
QVector<int> BoxTool::getKeyDownItems()
{
    QVector<int> v; v<<IsBox;
    return v;
}


bool BoxTool::onClick(const QPointF &p, QGraphicsItem * /* item */)
{
  QPointF pp=p;
  if (m_obj)
  {
      QRectF rct=m_obj->boundingRect();
      pp-=QPointF(rct.width()/2,rct.height()/2);
      m_obj->setPos(pp);
      if (m_diagram->canPlace(m_obj))
      {
          m_diagram->add(m_obj);
          m_obj=new Box(p,m_diagram);
          m_scene->addItem(m_obj);
      }
      m_scene->update();
  }
  return true;
}

bool BoxTool::onRelease(const QPointF & /* p */, QGraphicsItem * /* item */)
{
  return true;
}

bool BoxTool::onKeyDown(QKeyEvent * /* event */, QGraphicsItem * /* item */)
{
  return true;
}

void BoxTool::onMove(const QPointF & /* lastpos */ , const QPointF &pos)
{
    if (m_obj)
    {
        QRectF rct=m_obj->boundingRect();
        m_obj->setPos(pos-QPointF(rct.width()/2,rct.height()/2));
        m_scene->update();
    }
}
