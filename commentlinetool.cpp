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
     delete m_line;
     m_scene->update();
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


bool CommentLineTool::onClick(const QPointF &p, QGraphicsItem *  item )
{
   if (m_state==CLTS_START)
   {
        if (item)
        {
          if (item->type()!=IsFreeComment)
          {
             QPointF place=p;
             if (item->type()==IsBox)
             {
                 m_box=static_cast<Box*>(item);
                 m_loc=ATS_BOX;
             }
             if (item->type()==IsArrow)
             {
                 m_arrow=static_cast<Arrow*>(item);
                 m_loc=ATS_ARROW;
                 qreal arrowpos=position(*(m_arrow->model()),p);
                 m_arrowposition=arrowpos;
                 place=position(*(m_arrow->model()),m_arrowposition);
             }
             m_line=new CommentLine(place,place+QPointF(0,1),NULL,NULL,m_diagram);
             m_scene->addItem(m_line);
             m_startpoint=place;
             m_state=CLTS_FIRSTPOINT;
             static_cast<MainWindow*>(m_scene->view()->window())->setActionText("Click on free comment to attach it...");
          }
        }
   }
   else
   {
       if (item)
       {
           if (item->type()==IsFreeComment)
           {
               FreeComment * fc=static_cast<FreeComment *>(item);
               if (fc->parentComment()->line()==NULL)
                  tryConnectWith(p,fc);
           }
       }

   }
   return true;
}


void CommentLineTool::tryConnectWith(const QPointF & pos, FreeComment * fc)
{
    QVector<CollisionObject *> exc; //!< Excluded objects
    QPointF startpoint=m_startpoint;
    ObjectConnector * toConnectWith=NULL;
    if (m_loc==ATS_BOX)
    {
        exc<<m_box;
        QRectF crect=m_box->collisionRect();
        QPointF * tmp=getCollisionPoint(QLineF(m_startpoint,pos),
                                     crect);
        if (tmp)
        {
            Direction dir=D_LEFT;
            if (fabs(tmp->x()-crect.left())<0.001) dir=D_LEFT;
            if (fabs(tmp->x()-crect.right())<0.001) dir=D_RIGHT;
            if (fabs(tmp->y()-crect.top())<0.001) dir=D_TOP;
            if (fabs(tmp->y()-crect.bottom())<0.001) dir=D_BOTTOM;
            toConnectWith=m_box->getBySide(dir);
            startpoint=*tmp;
            delete tmp;
        }
    }
    if (m_loc==ATS_ARROW)
    {
        exc<<m_arrow;
        toConnectWith=m_arrow->model();
    }
    QPointF endpoint=pos;
    if (fc)
    {
        exc<<fc;
        QPointF * tmp=getCollisionPoint(QLineF(m_startpoint,pos),
                                     fc->collisionRect());
        if (tmp)
        {
            endpoint=*tmp;
            delete tmp;
        }
    }
    m_line->setLine(startpoint.x(),startpoint.y(),
                    endpoint.x(),endpoint.y());
    QVector<int> excv; excv<<IsCommentLine<<IsArrow;
    if (m_diagram->canPlace(m_line,excv,exc))
    {
        fc->parentComment()->setLine(m_line);
        if (toConnectWith)
        {
            qreal cpos=position(*toConnectWith,startpoint);
            m_line->setInput(toConnectWith,cpos);
        }
        initState();
    }
    m_scene->update();
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
    if (m_state==CLTS_FIRSTPOINT)
    {
        QList<QGraphicsItem *> lst=m_scene->items(pos);
        FreeComment * fc=NULL;
        for (int i=0;i<lst.size();i++)
            if (lst[i]->type()==IsFreeComment)
                fc=static_cast<FreeComment*>(lst[i]);
        QPointF startpoint=m_startpoint;
        if (m_loc==ATS_BOX)
        {
            QPointF * tmp=getCollisionPoint(QLineF(m_startpoint,pos),
                                         m_box->collisionRect());
            if (tmp)
            {
                startpoint=*tmp;
                delete tmp;
            }
        }
        QPointF endpoint=pos;
        if (fc)
        {
            QPointF * tmp=getCollisionPoint(QLineF(m_startpoint,pos),
                                         fc->collisionRect());
            if (tmp)
            {
                endpoint=*tmp;
                delete tmp;
            }
        }
        m_line->setLine(startpoint.x(),startpoint.y(),
                        endpoint.x(),endpoint.y());
        m_scene->update();
    }
}
