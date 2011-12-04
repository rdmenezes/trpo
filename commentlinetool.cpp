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
  if (m_state==CLTS_FIRSTPOINT)
  {
     m_scene->removeItem(m_line);
  }
  static_cast<MainWindow*>(m_scene->view()->window())->setActionText("");
}

void CommentLineTool::initState()
{
  static_cast<MainWindow*>(m_scene->view()->window())->setActionText("Specify first point of comment line...");
  m_state=CLTS_START;
}


QVector<int> CommentLineTool::getClickableItems()
{
    QVector<int>   v;
    v<<IsBox<<IsArrow<<IsFreeComment;
    return v;
}

QVector<int> CommentLineTool::getReleaseableItems()
{
    return QVector<int>();
}
QVector<int> CommentLineTool::getKeyDownItems()
{
    QVector<int> v;
    v<<IsBox        <<IsArrow
     <<IsFreeComment<<IsCommentLine;
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

bool CommentLineTool::onKeyDown(QKeyEvent *  event , QGraphicsItem * /* item */)
{
  if (m_state==CLTS_FIRSTPOINT && event->key()==Qt::Key_Escape)
  {
        clearState();
        initState();
  }
  return true;
}

void CommentLineTool::onMove(const QPointF & /* lastpos */ , const QPointF &pos)
{

}
