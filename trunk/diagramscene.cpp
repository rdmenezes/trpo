#include "diagramscene.h"
#include "helpwindow.h"
#include "objecttexteditor.h"
#include "commentline.h"
#include "arrowsegment.h"
#include "arrowpoint.h"
#include "freecomment.h"
#include <QTextEdit>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QGraphicsProxyWidget>
#include <QFontMetricsF>
#include <QApplication>
#include <QMessageBox>
#include <QVector>
#include <QtAlgorithms>
#include "mainwindow.h"
#include "ui_mainwindow.h"
//using namespace std;

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


void DiagramScene::toggleEditMode(bool on, const QRectF & r,
                                           QKeyEvent * e,
                                           DiagramObject * d)
{
  if (on)
  {
     if (m_editor) return;
     ObjectTextEditor * editor=new ObjectTextEditor(this,d);
     editor->setGeometry(r.x(),r.y(),r.width(),r.height());
     editor->setFont(this->font());
     editor->setPlainText(d->getEditableText());
     QTextCursor c=editor->textCursor();
     c.movePosition(QTextCursor::End);
     editor->setTextCursor(c);
     m_editor=this->addWidget(editor);
     editor->grabKeyboard();
     editor->keyPressEvent(e);
  }
  else
  {
      if (!m_editor) return;
      m_editor->widget()->releaseKeyboard();
      this->removeItem(m_editor);
      m_editor=NULL;
  }
}

DiagramScene::DiagramScene(Diagram * d,QObject *parent) :
    QGraphicsScene(parent)
{
  m_diag=d;
  m_editor=NULL;
  m_tool=NULL;
}

void DiagramScene::hideUI()
{
    if (m_editor)
        this->removeItem(m_editor);
}

const QRectF & DiagramScene::getDefaultBlockNumberSize() const
{
    return this->m_default_number_size;
}
/*! Cравнивает два объекта и возвращает объект с меньшим типом
    \param[in] i1 элемент1
    \param[in] i2 элемент2
 */
bool lessItem(QGraphicsItem * i1, QGraphicsItem * i2)
{
   return i1->type()<i2->type();
}

void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  QPointF pos=event->scenePos();
  // Затем сцена получает список объектов под курсором (метод QGraphicsScene::items)
  QList<QGraphicsItem *> lst=this->items(pos);
  this->diagram()->items(pos,lst);
  if (m_editor!=NULL || m_tool==NULL)
  {
     this->QGraphicsScene::mousePressEvent(event);
     return;
  }
  // Сцена сортирует этот список по убыванию типа
  qSort(lst.begin(),lst.end(),lessItem);  // Не совсем правильно, вначале, так будет вернее.
  QVector<int> vec=m_tool->getClickableItems();// через m_tool->getClickableItems() получается список типов объектов, которые инструмент обрабатывает
  bool objectClick=false;
  for (int i=0;(i<lst.size()) && !objectClick;i++)
  {
    if (vec.contains(lst[i]->type()))
        objectClick=m_tool->onClick(pos,lst[i]);//.Вызываем метод onClick() передавая туда объект, и точку
  }
  if (objectClick==false)
  {
    if (! ( m_tool->onClick(pos,NULL )))
         QGraphicsScene::mousePressEvent(event);
  }
}

void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos=event->scenePos();
    // Затем сцена получает список объектов под курсором (метод QGraphicsScene::items)
    QList<QGraphicsItem *> lst=this->items(pos);
    this->diagram()->items(pos,lst);
    if (m_editor!=NULL || m_tool==NULL)
    {
       this->QGraphicsScene::mouseReleaseEvent(event);
       return;
    }
    // Сцена сортирует этот список по убыванию типа
    qSort(lst.begin(),lst.end(),lessItem);
    QVector<int> vec=m_tool->getReleaseableItems();// через m_tool->getClickableItems() получается список типов объектов, которые инструмент обрабатывает
    bool objectClick=false;
    for (int i=0;(i<lst.size()) && !objectClick;i++)
    {
      if (vec.contains(lst[i]->type()))
      {
         objectClick=m_tool->onRelease(pos,lst[i]);//.Вызываем метод onClick() передавая туда объект, и точку
      }
    }
    if (objectClick==false)
    {
       if (! ( m_tool->onRelease(pos,NULL )))
           QGraphicsScene::mousePressEvent(event);
    }
}

void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
 {
  QPointF pointTmp=event->scenePos();
  double x1 = pointTmp.x();
  double y1 = pointTmp.y();
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
 if (m_tool)
        m_tool->onMove(event->lastScenePos(),event->scenePos());

}
void DiagramScene::keyPressEvent(QKeyEvent * event)
{
  QPoint local_pos=this->view()->mapFromGlobal(QCursor::pos());
  QPointF pos=this->view()->mapToScene(local_pos);
  // Затем сцена получает список объектов под курсором (метод QGraphicsScene::items)
  QList<QGraphicsItem *> lst=this->items(pos);
  this->diagram()->items(pos,lst);
  if (m_editor!=NULL || m_tool==NULL)
  {
     this->QGraphicsScene::keyPressEvent(event);
     return;
  }
  // Сцена сортирует этот список по убыванию типа
  qSort(lst.begin(),lst.end(),lessItem);
  QVector<int> vec=m_tool->getKeyDownItems();// через m_tool->getClickableItems() получается список типов объектов, которые инструмент обрабатывает
  bool objectClick=false;
  for (int i=0;(i<lst.size()) && !objectClick;i++)
  {
    if (vec.contains(lst[i]->type()))
    {
       objectClick=m_tool->onKeyDown(event,lst[i]);//.Вызываем метод onClick() передавая туда объект, и точку
    }
  }
  if (objectClick==false)
  {
     static_cast<MainWindow*>(this->view()->window())->keyPressEvent(event);
  }
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
          //if (lst[i]->type()==Box::USERTYPE)
          {
                determineDraggingBoxAction(static_cast<Box *>(lst[i]),
                                           event->scenePos()
                                           );
          }
          //if (lst[i]->type()==FreeComment::USERTYPE)
          {
              m_dragstate=DS_ALABEL_MOVE;
              QPointF pos=event->scenePos();
              FreeComment * item=static_cast<FreeComment *>(lst[i]);
              m_moving_label=item;
              QRectF       rct=item->boundingRect();
              m_blockmovingparams[0]=(pos.x()-rct.left())/rct.width();
              m_blockmovingparams[1]=(pos.y()-rct.top())/rct.height();
          }
        }
   }
}

#define CORNER_PRECISE 7
void DiagramScene::determineDraggingBoxAction(Box * item,const QPointF & pos)
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





void  DiagramScene::blockResizeMoveLeave ( QGraphicsSceneMouseEvent * event )
{
  /*
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
           //m_draggingblock->clearPointReferences();
           //m_draggingblock->setRect(oldrect);
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
          //m_draggingblock->setRect(newrect);
          //m_draggingblock->clearPointReferences();
          this->update();
       }
       m_dragstate=DS_NONE;
       m_draggingblock=NULL;
       m_resizingblockcorner=BC_LOWERLEFT;
    }
    */
}


void DiagramScene::clear()
{
 this->hideUI();
 this->m_diag->clear();
 this->QGraphicsScene::clear();
}

