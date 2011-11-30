#include "tooldelegate.h"
#include <QPixmap>
#include <QGraphicsView>
#include <stdio.h>

#define FRAME_OFFSET_X 0
#define FRAME_OFFSET_Y 0
#define ICON_OFFSET_X 25
#define ICON_OFFSET_Y 25

void ToolSceneData::deselect()
{
    this->m_scene->deselect();
}

void ToolSceneData::select()
{
    this->m_window->selectTool(this);
}

void ToolScene::setData(ToolSceneData * data)
 {
  m_data=data;
  data->setScene(this);
  /*
  QPixmap px=QPixmap::fromImage(*( data->icon() ));
  QGraphicsPixmapItem * item=addPixmap(px);
  item->moveBy((this->sceneRect().width()-px.width())/2
               ,
               (this->sceneRect().height()-px.height())/2);
  */
 }

void ToolScene::drawBackground ( QPainter * painter,
                                 const QRectF & rect )
{
    if (!m_init) {
        QPixmap px=QPixmap::fromImage(*( m_data->icon() ));
        QGraphicsPixmapItem * item=addPixmap(px);
        item->moveBy((this->sceneRect().x()+this->sceneRect().width()-px.width())/2
                     ,
                     (this->sceneRect().y()+this->sceneRect().height()-px.height())/2);
        m_init=true;
    }
    QGraphicsScene::drawBackground(painter,rect);
}

void ToolScene::drawSelectionFrame()
{
  QPen  pen(QBrush(QColor(255,0,0)),1);
  qreal firstx=this->sceneRect().x()+FRAME_OFFSET_X;
  qreal firsty=this->sceneRect().y()+FRAME_OFFSET_Y;
  qreal lastx=this->sceneRect().x()+this->sceneRect().width()-FRAME_OFFSET_X-1;
  qreal lasty=this->sceneRect().y()+this->sceneRect().height()-FRAME_OFFSET_Y-1;
  m_frame<<addLine(firstx,firsty,lastx,firsty,pen);
  m_frame<<addLine(lastx,firsty,lastx,lasty,pen);
  m_frame<<addLine(lastx,lasty,firstx,lasty,pen);
  m_frame<<addLine(firstx,lasty,firstx,firsty,pen);
  update();
}


void ToolScene::deselect()
{
    for (int i=0;i<m_frame.size();i++)
        removeItem(m_frame[i]);
    m_frame.clear();
}

void ToolScene::select()
{
    m_data->select();
    drawSelectionFrame();
}

#include <QGraphicsSceneMouseEvent>
void 	ToolScene::mousePressEvent ( QGraphicsSceneMouseEvent *  mouseEvent  )
{
    printf("Clicked at: %f %f\n",mouseEvent->scenePos().x(),mouseEvent->scenePos().y());
    printf("Press ev: %d %d\n",this->views()[0]->viewport()->width(),
                               this->views()[0]->viewport()->height());

    printf("Scene rectangle: %f %f %f %f\n",
                               this->sceneRect().x(),
                               this->sceneRect().y(),
                               this->sceneRect().width(),
                               this->sceneRect().height());
    select();
}

