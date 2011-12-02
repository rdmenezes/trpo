#include "diagramtesttool.h"
#include "diagramscene.h"
#include "mainwindow.h"

void   DiagramTestTool::clearState()
{
  if (m_test)
    this->m_scene->removeItem(m_test);
  delete this;
}

void DiagramTestTool::initState()
{
    m_test=new DiagramObjectTest(QPointF(0,0),m_diagram);
    this->m_scene->addItem(m_test);
    static_cast<MainWindow*>(m_scene->view()->window())->setActionText("Performing tool test...");
}

DiagramTestTool::~DiagramTestTool()
{

}

QVector<int> DiagramTestTool::getClickableItems()
{
    return QVector<int>();
}

QVector<int> DiagramTestTool::getReleaseableItems()
{
    return QVector<int>();
}
QVector<int> DiagramTestTool::getKeyDownItems()
{
    return QVector<int>();
}


void DiagramTestTool::onClick(const QPointF &p, QGraphicsItem * /* item */)
{
  if (m_test)
  {
      QRectF rct=m_test->boundingRect();
      m_test->setPos(p-QPointF(rct.width()/2,rct.height()/2));
      m_scene->update();
  }
  m_test=new DiagramObjectTest(p,m_diagram);
  this->m_scene->addItem(m_test);
}

void DiagramTestTool::onRelease(const QPointF & /* p */, QGraphicsItem * /* item */)
{

}

void DiagramTestTool::onKeyDown(QKeyEvent * /* event */, QGraphicsItem * /* item */)
{

}

void DiagramTestTool::onMove(const QPointF & /* lastpos */ , const QPointF &pos)
{
    if (m_test)
    {
        QRectF rct=m_test->boundingRect();
        m_test->setPos(pos-QPointF(rct.width()/2,rct.height()/2));
        m_scene->update();
    }
}



