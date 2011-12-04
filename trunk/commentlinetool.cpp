#include "commentlinetool.h"
#include "diagramscene.h"
#include "mainwindow.h"

CommentLineTool::CommentLineTool()
{
}

CommentLineTool::~CommentLineTool()
{

}


void CommentLineTool::clearState()
{
  static_cast<MainWindow*>(m_scene->view()->window())->setActionText("");
}

void CommentLineTool::initState()
{
  static_cast<MainWindow*>(m_scene->view()->window())->setActionText("Specify first point of comment line...");
}


QVector<int> CommentLineTool::getClickableItems()
{
    return QVector<int>();
}

QVector<int> CommentLineTool::getReleaseableItems()
{
    return QVector<int>();
}
QVector<int> CommentLineTool::getKeyDownItems()
{
    QVector<int> v;
    return v;
}


bool CommentLineTool::onClick(const QPointF &p, QGraphicsItem * /* item */)
{
   return true;
}

bool CommentLineTool::onRelease(const QPointF & /* p */, QGraphicsItem * /* item */)
{
  return true;
}

bool CommentLineTool::onKeyDown(QKeyEvent * /* event */, QGraphicsItem * /* item */)
{
  return true;
}

void CommentLineTool::onMove(const QPointF & /* lastpos */ , const QPointF &pos)
{

}
