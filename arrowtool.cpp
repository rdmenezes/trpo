#include "arrowtool.h"

ArrowTool::ArrowTool()
{
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

}

