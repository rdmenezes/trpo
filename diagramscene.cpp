#include "diagramscene.h"
#include "helpwindow.h"
#include "toolpanel.h"
#include <QTextEdit>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QGraphicsProxyWidget>
#include <QFontMetricsF>
#include <QApplication>

DiagramScene::DiagramScene(Diagram * d,QObject *parent) :
    QGraphicsScene(parent)
{
  m_diag=d;
  m_tooltype=TT_BLOCK;
  m_panel=NULL;
  //Compute size of default block
  //Get small size label
  QFont fnt=this->font();
  fnt.setPointSize(10);
  QFontMetrics mtr1(fnt);
  QRect number_size=mtr1.boundingRect("9");
  //Get size of default text
  QFontMetrics mtr2(this->font());
  QRect label_size=mtr2.boundingRect(DEFAULT_BLOCK_TEXT);
  m_default_block_size.setX(0);
  m_default_block_size.setY(0);
  m_default_block_size.setWidth(
                                ((label_size.width()>number_size.width())?
                                label_size.width():number_size.width())
                                +BLOCK_SPACE_X*2
                               );
  m_default_block_size.setHeight(
                                 label_size.height()+
                                 (number_size.height()+BLOCK_SPACE_Y)*2
                                );
}

void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos=event->buttonDownScenePos(event->button());
    //Do in case of nothing found
    if (this->items(pos).size()==0)
    {
      if (m_tooltype==TT_BLOCK)
          addBlock(event);
    }
    //Propagate key pressing event
    else
    {
     bool isWidgetClicked=false;
     QList<QGraphicsItem *> lst=this->items(pos);
     for (int i=0;i<lst.size();i++)
     {
         if (lst[i]->type()==QGraphicsProxyWidget::Type)
             isWidgetClicked=true;
     }
     if (isWidgetClicked)
        this->QGraphicsScene::mousePressEvent(event);
     else
        this->QGraphicsScene::mousePressEvent(event);
    }
}
void DiagramScene::keyPressEvent(QKeyEvent * event)
{
  bool handled=false;
  //In case when F1 pressed show help
  if (m_panel)
     handled=processKeyToolSelect(event);
  if (event->key()==Qt::Key_F1)
  {
      HelpWindow d;
      d.exec();
      handled=true;
  }
  //In case when Shift presset add panel if not present
  if (event->key()==Qt::Key_Shift)
  {
      //Compute panel position
      QPointF  panel_pos;
      QPoint local_pos=m_view->mapFromGlobal(QCursor::pos());
      panel_pos=m_view->mapToScene(local_pos);
      panel_pos.setX(panel_pos.x()-PANEL_WIDTH/2);
      panel_pos.setY(panel_pos.y()-PANEL_HEIGHT/2);

      //Check under- and over-flow
      if (panel_pos.x()<0)  panel_pos.setX(0);
      if (panel_pos.y()<0)  panel_pos.setY(0);
      if (panel_pos.x()+PANEL_WIDTH>this->width())
           panel_pos.setX(this->width()-PANEL_WIDTH);
      if (panel_pos.y()+PANEL_HEIGHT>this->height())
          panel_pos.setY(this->height()-PANEL_HEIGHT);

      if (m_panel==NULL)
      {
       //Add tool panel
       m_panel=new ToolPanel(m_view);
       m_panel_in_scene=this->addWidget(m_panel);
       //Set parameters and update
       m_panel->setGeometry(panel_pos.x(),panel_pos.y(),PANEL_WIDTH,PANEL_HEIGHT);
       m_panel->update();
      }
      else
      {
          //Move panel
          m_panel->move(panel_pos.x(),panel_pos.y());
          m_panel->update();
      }
      handled=true;
  }
  if (!handled)
  {
     QPoint viewpos=m_view->mapFromGlobal(QCursor::pos());
     QPointF scenepos=m_view->mapToScene(viewpos);
     QList<QGraphicsItem *> items=this->items(scenepos);
     for (int i=0;(i<items.size()) && !handled;i++)
     {
         if (items[i]->type()==BoxItem::USERTYPE)
         {
             static_cast<BoxItem *>(items[i])->keyPressEvent(event);
             handled=true;
         }
     }
  }
  if (!handled)
      this->QGraphicsScene::keyPressEvent(event);
}


void DiagramScene::setTool(ToolType t)
{
    Q_ASSERT( m_panel );
    m_tooltype=t;

    this->removeItem(m_panel_in_scene);

    m_panel=NULL;
    m_panel_in_scene=NULL;
}

bool DiagramScene::processKeyToolSelect(QKeyEvent * event)
{
    int       keys[6]={Qt::Key_1,Qt::Key_2,Qt::Key_3,Qt::Key_4,Qt::Key_5,Qt::Key_6};
    ToolType types[6]={TT_SELECT,TT_ERASER,TT_BLOCK,TT_ARROW,TT_ANNOTATION_LINE,
                       TT_ANNOTATION_LABEL};
    for (int i=0;i<6;i++)
    {
        if (event->key()==keys[i])
            this->setTool(types[i]);
    }
    return false;
}

QRectF DiagramScene::getDefaultBlockSize(const QPointF & pos)
{
  QRectF result;
  result.setX(pos.x()-m_default_block_size.width()/2);
  result.setY(pos.y()-m_default_block_size.height()/2);
  result.setWidth(m_default_block_size.width());
  result.setHeight(m_default_block_size.height());
  return result;
}

void DiagramScene::addBlock(QGraphicsSceneMouseEvent *event)
{
 if (m_diag->canAddBoxes())
 {
  if (m_diag->canBePlaced(getDefaultBlockSize(event->scenePos())))
  {
   BoxItem * b=new BoxItem(event->scenePos(),this);
   this->addItem(b);
   m_diag->addBox(b);
  }
 }
}

void DiagramScene::decrementBlockID(BoxItem * block)
{
 int previd=block->id();
 int newid=((block->id()==0)?DIAGRAM_MAX_BLOCKS:previd)-1;
 processChangeBlockID(block,previd,newid);
}

void DiagramScene::incrementBlockID(BoxItem * block)
{
 int previd=block->id();
 int newid=(block->id()==DIAGRAM_MAX_BLOCKS-1)?0:(previd+1);
 processChangeBlockID(block,previd,newid);
}

void DiagramScene::processChangeBlockID(BoxItem * block, char previd, char newid)
{
 m_diag->undoIfSwapped(block,previd);
 if (m_diag->getBlockByID(newid))
 {
        BoxItem * oldblock=m_diag->getBlockByID(newid);
        m_diag->addNewSwap(oldblock,previd,block,newid);
 }
 m_diag->setBlockID(block,newid);
}
