#include "diagramscene.h"
#include "helpwindow.h"
#include "toolpanel.h"
#include "labeledit.h"
#include "alabelitem.h"
#include "arrowsegment.h"
#include "arrowpoint.h"
#include "alineitem.h"
#include <QTextEdit>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QGraphicsProxyWidget>
#include <QFontMetricsF>
#include <QApplication>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"
//Unit-tests for arrow joining
//#define MERGE_TEST_1
//#define MERGE_TEST_2
//#define MERGE_TEST_3
//#define MERGE_TEST_4

//Unit tests for annotation line item
//#define   TOP_ALINE_TEST
//#define BOTTOM_ALINE_TEST
//#define LEFT_ALINE_TEST
//#define RIGHT_ALINE_TEST

DiagramScene::DiagramScene(Diagram * d,QObject *parent) :
    QGraphicsScene(parent)
{
  m_diag=d;
  m_tooltype=TT_BLOCK;
  m_panel=NULL;
  m_panel_in_scene=NULL;
  //Compute size of default block
  //Get small size label
  QFont fnt=this->font();
  fnt.setPointSize(10);
  QFontMetrics mtr1(fnt);
  m_default_number_size=mtr1.boundingRect("9");
  QRectF & number_size=m_default_number_size;
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
  //Compute a size of default label
  QRect tmp_alabel=mtr2.boundingRect(DEFAULT_ALABEL_TEXT);
  m_alabel_block_size=QRectF(tmp_alabel.x(),tmp_alabel.y(),
                             tmp_alabel.width()+2, tmp_alabel.height()+2);
  //Sets a no edit state
  m_edit_state=TES_NONE;
  m_label_editor=NULL;
  m_label_editor_in_scene=NULL;
  this->clearElementStates();

  //Conditional test situation
#ifdef MERGE_TEST_1
    ArrowPoint * p0=new ArrowPoint(200,200);
    ArrowPoint * p1=new ArrowPoint(250,200);

    ArrowPoint * p2=new ArrowPoint(300,200);
    ArrowPoint * p3=new ArrowPoint(400,200);
#endif
#ifdef MERGE_TEST_2
    ArrowPoint * p0=new ArrowPoint(200,200);
    ArrowPoint * p1=new ArrowPoint(250,200);

    ArrowPoint * p2=new ArrowPoint(300,200);
    ArrowPoint * p3=new ArrowPoint(300,300);
#endif
#ifdef MERGE_TEST_3
    ArrowPoint * p0=new ArrowPoint(200,200);
    ArrowPoint * p1=new ArrowPoint(200,300);

    ArrowPoint * p2=new ArrowPoint(300,300);
    ArrowPoint * p3=new ArrowPoint(350,300);
#endif
#ifdef MERGE_TEST_4
    ArrowPoint * p0=new ArrowPoint(200,200);
    ArrowPoint * p1=new ArrowPoint(200,300);

    ArrowPoint * p2=new ArrowPoint(300,300);
    ArrowPoint * p3=new ArrowPoint(300,200);
#endif
#if  defined MERGE_TEST_1 || defined MERGE_TEST_2 || defined MERGE_TEST_3 || defined MERGE_TEST_4
    m_diag->addArrowPoint(p0);
    m_diag->addArrowPoint(p1);
    m_diag->addArrowPoint(p2);
    m_diag->addArrowPoint(p3);
    ArrowSegment * seg1=new ArrowSegment(p0,p1);
    ArrowSegment * seg2=new ArrowSegment(p2,p3);
    m_diag->addArrowSegment(seg1);
    m_diag->addArrowSegment(seg2);
    addItem(seg1); addItem(seg2);
    update();
#endif
#ifdef TOP_ALINE_TEST
    this->addItem( new ALineItem(QPointF(200,200),QPointF(200,190)) );
    this->addItem( new ALineItem(QPointF(250,200),QPointF(250,150)) );
    this->addItem( new ALineItem(QPointF(300,200),QPointF(300,100)) );
    this->addItem( new ALineItem(QPointF(350,200),QPointF(350,50)) );
    this->update();
#endif
#ifdef BOTTOM_ALINE_TEST
    this->addItem( new ALineItem(QPointF(200,190),QPointF(200,200)) );
    this->addItem( new ALineItem(QPointF(250,150),QPointF(250,200)) );
    this->addItem( new ALineItem(QPointF(300,100),QPointF(300,200)) );
    this->addItem( new ALineItem(QPointF(350,50),QPointF(350,200)) );
    this->update();
#endif
#ifdef LEFT_ALINE_TEST
    this->addItem( new ALineItem(QPointF(350,200),QPointF(340,200)) );
    this->addItem( new ALineItem(QPointF(350,150),QPointF(300,150)) );
    this->addItem( new ALineItem(QPointF(350,100),QPointF(250,100)) );
    this->addItem( new ALineItem(QPointF(350,50),QPointF(150,50)) );
    this->update();
#endif
#ifdef RIGHT_ALINE_TEST
    this->addItem( new ALineItem(QPointF(340,200),QPointF(350,200)) );
    this->addItem( new ALineItem(QPointF(300,150),QPointF(350,150)) );
    this->addItem( new ALineItem(QPointF(250,100),QPointF(350,100)) );
    this->addItem( new ALineItem(QPointF(150,50),QPointF(350,50)) );
    this->update();
#endif
}

void DiagramScene::hideUI()
{
 if (m_panel)
 {
     this->removeItem(m_panel_in_scene);
 }
 m_panel=NULL;
 m_panel_in_scene=NULL;
 if (m_edit_state==TES_EDIT)
 {
     this->removeItem(m_label_editor_in_scene);
 }
 m_edit_state=TES_NONE;
 m_label_editor=NULL;
 m_label_editor_in_scene=NULL;
}

const QRectF & DiagramScene::getDefaultBlockNumberSize() const
{
    return this->m_default_number_size;
}
void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos=event->buttonDownScenePos(event->button());
    //Do in case of nothing found
    if (this->items(pos).size()==0)
    {
      if (m_tooltype==TT_BLOCK && m_edit_state==TES_NONE)
          addBlock(event);
      if (m_tooltype==TT_ANNOTATION_LABEL && m_edit_state==TES_NONE)
          addAnnotationLabel(event);
      if (m_tooltype==TT_ARROW)
          processArrowClickOnBlankSpace(event);
      if (m_tooltype==TT_ANNOTATION_LINE)
      {
          if (m_alds==ALDS_SPECIFIEDNONE)
            processAnnotationLinePointOnBlank(pos);
          else
            processAnnotationLineSecondPointOnBlank(pos);
      }

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
         if (m_tooltype==TT_ARROW && lst[i]->type()==BoxItem::USERTYPE)
         {
             this->processArrowClickOnBox(event,static_cast<BoxItem *>(lst[i]));
             return;
         }
         if (m_tooltype==TT_ARROW && lst[i]->type()==ArrowSegment::USERTYPE)
         {
             this->processArrowClickOnLine(event,static_cast<ArrowSegment *>(lst[i]));
             return;
         }
         if (m_tooltype==TT_ANNOTATION_LINE && lst[i]->type()==BoxItem::USERTYPE
                                            && m_alds==ALDS_SPECIFIEDNONE)
         {
             this->processAnnotationLineToBox(event->scenePos(),static_cast<BoxItem *>(lst[i]));
             return;
         }
         if (m_tooltype==TT_ANNOTATION_LINE && lst[i]->type()==ArrowSegment::USERTYPE
                                            && m_alds==ALDS_SPECIFIEDNONE)
         {
             this->processAnnotationLineToSegment(event->scenePos(),static_cast<ArrowSegment *>(lst[i]));
             return;
         }
         if (m_tooltype==TT_SELECT && lst[i]->type()==ArrowSegment::USERTYPE)
         {
             this->enterArrowMove(event->scenePos(),static_cast<ArrowSegment *>(lst[i]));
             return;
         }
         if (m_tooltype==TT_SELECT && lst[i]->type()==ALineItem::USERTYPE)
         {
             this->enterAnnotationLineResize(event->scenePos(),static_cast<ALineItem *>(lst[i]));
             return;
         }
     }
     if (isWidgetClicked)
        this->QGraphicsScene::mousePressEvent(event);
     else
     {
       if (m_tooltype==TT_ERASER && m_edit_state==TES_NONE)
             processRemoving(lst);
       else if (m_tooltype==TT_SELECT) blockResizeMoveEnter(event);
       else this->QGraphicsScene::mousePressEvent(event);
     }
    }
}
void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

QPointF pointTmp=event->scenePos();
double x1 = pointTmp.rx();
double y1 = pointTmp.ry();
y1=y1*-1;
QString   textX;
textX.setNum(x1);
QString   textY;
textY.setNum(y1);
QString text;
text.append(textX);
text.append(" ");
text.append(textY);
MainWindow *parentwin = static_cast <MainWindow*>(this->views()[0]->window());
parentwin->ui->cursorCoordinates->clear();
parentwin->ui->cursorCoordinates->setText(text);


}
void DiagramScene::keyPressEvent(QKeyEvent * event)
{
  bool handled=false;
  //In case when F1 pressed show help
  if (m_panel)
     handled=processKeyToolSelect(event);
  if (event->key()==Qt::Key_Escape && m_tooltype==TT_ARROW && m_arrow_state==AES_EDIT)
  {
      //Compute position
      QPointF  pos;
      QPoint local_pos=m_view->mapFromGlobal(QCursor::pos());
      pos=m_view->mapToScene(local_pos);
      this->processArrowEscapePress(pos);
  }
  if (event->key()==Qt::Key_Escape && m_tooltype==TT_ANNOTATION_LINE && m_alds==ALDS_SPECIFIEDFIRSTPOINT)
  {
      //Compute position
      QPointF  pos;
      QPoint local_pos=m_view->mapFromGlobal(QCursor::pos());
      pos=m_view->mapToScene(local_pos);
      this->processAnnotationLineEscapePress(pos);
  }
  //TODO: Remove this part
  if (event->key()==Qt::Key_G)
      this->exportTo("test.png");
  if (event->key()==Qt::Key_F1)
  {
      HelpWindow d;
      d.exec();
      handled=true;
  }
  if (event->key()==Qt::Key_F5)
  {
      if (! (this->save("tmp.xml")))
      { QMessageBox::critical(NULL,"Can't save file","Can't save to \"tmp.xml\"");}
  }
  if (event->key()==Qt::Key_F6)
  {
      if (! (this->load("tmp.xml")))
      { QMessageBox::critical(NULL,"Can't load file","Can't load from \"tmp.xml\"");}
  }
  if (event->key()==Qt::Key_F3)
  {
      this->clear();
  }
  //In case when Shift presset add panel if not present
  if (event->key()==Qt::Key_Shift
      && m_arrow_state!=AES_EDIT
      && m_alds!=ALDS_SPECIFIEDFIRSTPOINT
      && m_dragstate==DS_NONE)
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
         if (items[i]->type()==ALabelItem::USERTYPE)
         {
             static_cast<ALabelItem *>(items[i])->keyPressEvent(event);
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
    bool handled=false;
    for (int i=0;i<6;i++)
    {
        if (event->key()==keys[i])
        {
            this->setTool(types[i]);
            handled=true;
        }
    }
    return handled;
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

QRectF DiagramScene::getDefaultAnnotationLabelSize(const QPointF & pos)
{
  QRectF result;
  result.setX(pos.x()-m_alabel_block_size.width()/2);
  result.setY(pos.y()-m_alabel_block_size.height()/2);
  result.setWidth(m_alabel_block_size.width());
  result.setHeight(m_alabel_block_size.height());
  return result;
}

void DiagramScene::addBlock(QGraphicsSceneMouseEvent *event)
{
 if (m_diag->canAddBoxes())
 {
  QRectF size=getDefaultBlockSize(event->scenePos());
  if (m_diag->canBePlaced(size,(BoxItem*)NULL))
  {
   QVector<ArrowPoint *> list=m_diag->getNearArrowPoints(size);
   if (m_diag->canBePlacedAroundPoints(size,list))
   {
    BoxItem * b=new BoxItem(event->scenePos(),this);
    this->addItem(b);
    m_diag->addBox(b);
    b->attachAllPoints(list);
   }
  }
 }
}

void DiagramScene::addAnnotationLabel(QGraphicsSceneMouseEvent *event)
{
 QRectF size=getDefaultAnnotationLabelSize(event->scenePos());
 if (m_diag->canBePlaced(size,(ALabelItem*)NULL))
 {
     ALabelItem * a=new ALabelItem(size);
     this->addItem(a);
     m_diag->addAnnotationLabel(a);
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

void DiagramScene::processChangeBlockID(BoxItem * block, char /* previd */, char  newid)
{
 //m_diag->undoIfSwapped(block,previd);
 if (m_diag->getBlockByID(newid))
 {
        //BoxItem * oldblock=m_diag->getBlockByID(newid);
        //m_diag->addNewSwap(oldblock,previd,block,newid);
 }
 m_diag->setBlockID(block,newid);
}

void DiagramScene::processRemoving(const QList<QGraphicsItem *> & items)
 {
    for (int i=0;i<items.size();i++)
    {
        if (items[i]->type()==BoxItem::USERTYPE)
        {
            char id=static_cast<BoxItem *>(items[i])->id();
            static_cast<BoxItem *>(items[i])->clearPointReferences();
            m_diag->removeBlock(id);
            this->removeItem(items[i]);
        }
        if (items[i]->type()==ALabelItem::USERTYPE)
        {
            m_diag->removeAnnotationLabel(static_cast<ALabelItem *>(items[i]));
            this->removeItem(items[i]);
        }
        if (items[i]->type()==ArrowSegment::USERTYPE)
        {
          removeArrowSegment(static_cast<ArrowSegment *>(items[i]));
        }
        if (items[i]->type()==ALineItem::USERTYPE)
        {
            removeAnnotationLine(static_cast<ALineItem *>(items[i]));
        }
    }
 }

void DiagramScene::toggleEditStateOff()
{
    this->m_edit_state=TES_NONE;
    this->m_label_editor->releaseKeyboard();
    this->removeItem(m_label_editor_in_scene);
    m_label_editor=NULL;
    m_label_editor_in_scene=NULL;
}


void  DiagramScene::blockResizeMoveEnter ( QGraphicsSceneMouseEvent * event )
{
   QList<QGraphicsItem *> lst=items(event->scenePos());
   bool isWidgetClicked=false;
   for (int i=0;i<lst.size();i++)
   {
       if (lst[i]->type()==QGraphicsProxyWidget::Type)
           isWidgetClicked=true;
   }
   if (isWidgetClicked || lst.size()==0)
      this->QGraphicsScene::mousePressEvent(event);
   else
   {
        for(int i=0;i<lst.size();i++)
        {
          if (lst[i]->type()==BoxItem::USERTYPE)
          {
                determineDraggingBoxAction(static_cast<BoxItem *>(lst[i]),
                                           event->scenePos()
                                           );
          }
          if (lst[i]->type()==ALabelItem::USERTYPE)
          {
              m_dragstate=DS_ALABEL_MOVE;
              QPointF pos=event->scenePos();
              ALabelItem * item=static_cast<ALabelItem *>(lst[i]);
              m_moving_label=item;
              QRectF       rct=item->boundingRect();
              m_blockmovingparams[0]=(pos.x()-rct.left())/rct.width();
              m_blockmovingparams[1]=(pos.y()-rct.top())/rct.height();
          }
        }
   }
}

#define CORNER_PRECISE 7
void DiagramScene::determineDraggingBoxAction(BoxItem * item,const QPointF & pos)
{
  bool handled=false;
  if (   pos.x()-item->boundingRect().x() < CORNER_PRECISE
         &&  pos.y()-item->boundingRect().y() < CORNER_PRECISE
         &&  !handled
     )
  {
     handled=true;
     m_dragstate=DS_BLOCK_RESIZE;
     m_draggingblock=item;
     m_resizingblockcorner=BC_UPPERLEFT;
  }
  //printf("Check for upper right: %d %d %d %d\n",(int)(item->boundingRect().right())
  //       ,pos.x(),pos.y(),item->boundingRect().y()
  //        );
  if (   item->boundingRect().right()-pos.x() < CORNER_PRECISE
         &&  pos.y()-item->boundingRect().y() < CORNER_PRECISE
         &&  !handled
     )
  {
     handled=true;
     m_dragstate=DS_BLOCK_RESIZE;
     m_draggingblock=item;
     m_resizingblockcorner=BC_UPPERRIGHT;
  }
  if (   pos.x()-item->boundingRect().x() < CORNER_PRECISE
         &&  item->boundingRect().bottom()-pos.y() < CORNER_PRECISE
         &&  !handled
     )
  {
     handled=true;
     m_dragstate=DS_BLOCK_RESIZE;
     m_draggingblock=item;
     m_resizingblockcorner=BC_LOWERLEFT;
  }
  //printf("Check for upper right: %d %d %d %d\n",(int)(item->boundingRect().right())
  //       ,pos.x(),item->boundingRect().bottom(),pos.y()
  //       );
  if (   item->boundingRect().right()-pos.x() < CORNER_PRECISE
         &&  item->boundingRect().bottom()-pos.y() < CORNER_PRECISE
         &&  !handled
     )
  {
     handled=true;
     m_dragstate=DS_BLOCK_RESIZE;
     m_draggingblock=item;
     m_resizingblockcorner=BC_LOWERRIGHT;
  }
  if (!handled)
  {
      m_dragstate=DS_BLOCK_MOVE;
      m_draggingblock=item;
      QRectF rct=item->boundingRect();
      m_blockmovingparams[0]=((qreal)pos.x()-rct.left())/rct.width();
      m_blockmovingparams[1]=((qreal)pos.y()-rct.top())/rct.height();
  }
}

void resizeLeft(QRectF & oldrect, QPointF & pos)
{
    if (pos.x()>=oldrect.right())
    {
        qreal rt=oldrect.right();
        oldrect.setWidth(pos.x()-oldrect.left());
        oldrect.setLeft(rt);
    } else oldrect.setLeft(pos.x());
}

void resizeRight(QRectF & oldrect, QPointF & pos)
{
    if (pos.x()<=oldrect.left())
    {
        qreal oldleft=oldrect.left();
        oldrect.setX(pos.x());
        oldrect.setWidth(oldleft-pos.x());
    } else oldrect.setWidth(pos.x()-oldrect.left());
}
void resizeLower(QRectF & oldrect, QPointF & pos)
{
    if (pos.y()<=oldrect.top())
    {
        qreal oldtop=oldrect.top();
        oldrect.setY(pos.y());
        oldrect.setHeight(oldtop-pos.y());
    } else oldrect.setHeight(pos.y()-oldrect.top());
}

void resizeUpper(QRectF & oldrect, QPointF & pos)
{
   if (pos.y()>=oldrect.bottom())
   {
      qreal oldbottom=oldrect.bottom();
      oldrect.setHeight(pos.y()-oldrect.top());
      oldrect.setY(oldbottom);
   } else oldrect.setY(pos.y());
}


void DiagramScene::annnotationLabelMoveLeave(const QPointF & pos)
{
 QRectF oldrect=m_moving_label->boundingRect();
 qreal x=pos.x()-m_blockmovingparams[0]*oldrect.width();
 qreal y=pos.y()-m_blockmovingparams[1]*oldrect.height();
 if (x<0) x=0;
 if (y<0) y=0;
 if (x+oldrect.width()>this->width()) x=this->width()-oldrect.width();
 if (y+oldrect.height()>this->height()) y=this->height()-oldrect.height();
 QRectF newrect(x,y,oldrect.width(),oldrect.height());
 bool can_placed=m_diag->canBePlaced(newrect,m_moving_label);
 if(can_placed)
 {
  m_moving_label->setRect(newrect);
  this->update();
 }
 m_dragstate=DS_NONE;
 m_moving_label=NULL;
}

void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_tooltype==TT_SELECT && (m_dragstate==DS_BLOCK_MOVE || m_dragstate==DS_BLOCK_RESIZE))
        blockResizeMoveLeave(event);
    if (m_tooltype==TT_SELECT && m_dragstate==DS_ALABEL_MOVE)
        annnotationLabelMoveLeave(event->scenePos());
    if (m_tooltype==TT_SELECT  && m_dragstate==DS_ARROW_MOVE)
        arrowMoveLeave(event->scenePos());
    if (m_tooltype==TT_SELECT && m_dragstate==DS_ALINE_RESIZE)
        leaveAnnotationLineResize(event->scenePos());
}

void  DiagramScene::blockResizeMoveLeave ( QGraphicsSceneMouseEvent * event )
{
    this->QGraphicsScene::mouseReleaseEvent(event);
    QPointF pos=event->scenePos();
    if (m_dragstate==DS_BLOCK_RESIZE)
    {
         QRectF oldrect=m_draggingblock->boundingRect();
        //Compute new rectangle
#define WHATDO(X,Y,Z)   if (m_resizingblockcorner == X) \
                        {   Y (oldrect,pos); Z (oldrect,pos);}
        WHATDO(BC_LOWERLEFT,resizeLower,resizeLeft);
        WHATDO(BC_LOWERRIGHT,resizeLower,resizeRight);
        WHATDO(BC_UPPERLEFT,resizeUpper,resizeLeft);
        WHATDO(BC_UPPERRIGHT,resizeUpper,resizeRight);
#undef  WHATDO
        QRectF labelsize=this->getDefaultBlockNumberSize();
        bool too_small=oldrect.height()<=labelsize.height() || oldrect.width()<=labelsize.width();
        bool can_placed=m_diag->canBePlaced(oldrect,m_draggingblock);
        if( !too_small && can_placed)
        {
         QVector<ArrowPoint *> lst=m_diag->getNearArrowPoints(oldrect);
         if (m_diag->canBePlacedAroundPoints(oldrect,lst))
         {
           m_draggingblock->clearPointReferences();
           m_draggingblock->setRect(oldrect);
           m_draggingblock->attachAllPoints(lst);
           this->update();
         }
        }
        m_dragstate=DS_NONE;
        m_draggingblock=NULL;
        m_resizingblockcorner=BC_LOWERLEFT;
    }
    if (m_dragstate==DS_BLOCK_MOVE)
    {
       QRectF oldrect=m_draggingblock->boundingRect();
       qreal x=pos.x()-m_blockmovingparams[0]*oldrect.width();
       qreal y=pos.y()-m_blockmovingparams[1]*oldrect.height();
       if (x<0) x=0;
       if (y<0) y=0;
       if (x+oldrect.width()>this->width()) x=this->width()-oldrect.width();
       if (y+oldrect.height()>this->height()) y=this->height()-oldrect.height();
       QRectF newrect(x,y,oldrect.width(),oldrect.height());
       bool can_placed=m_diag->canBePlaced(newrect,m_draggingblock);
       if(can_placed)
       {
          m_draggingblock->setRect(newrect);
          m_draggingblock->clearPointReferences();
          this->update();
       }
       m_dragstate=DS_NONE;
       m_draggingblock=NULL;
       m_resizingblockcorner=BC_LOWERLEFT;
    }
}


void DiagramScene::clear()
{
 this->hideUI();
 this->QGraphicsScene::clear();
 this->m_diag->clear();
 this->clearElementStates();
 m_tooltype=TT_BLOCK;
}

void DiagramScene::clearElementStates()
{
 //Sets a no dragging state
 m_dragstate=DS_NONE;
 m_draggingblock=NULL;
 m_resizingblockcorner=BC_LOWERLEFT;
 m_moving_label=NULL;
 //Sets a no arrow editing state
 m_arrow_state=AES_NONE;
 m_last_arrow_point=NULL;
 //Sets a no annotation line editing state
 m_aline_segment=NULL;
 m_alds=ALDS_SPECIFIEDNONE;
}
