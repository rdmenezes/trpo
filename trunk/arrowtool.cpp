#include "arrowtool.h"
#include "diagramscene.h"
#include "mainwindow.h"
#include <math.h>
#include <stdio.h>
#include <QMessageBox>
#include <assert.h>
ArrowTool::ArrowTool()
{
    //Init drawing array
    m_drawarr[0]=&ArrowTool::drawZeroDirected;
    m_drawarr[1]=&ArrowTool::drawRightLeftDirected;
    m_drawarr[2]=&ArrowTool::drawHVDirected;
    m_drawarr[3]=&ArrowTool::drawVHDirected;
    m_drawarr[4]=&ArrowTool::drawTopBottomDirected;
    m_drawarr[5]=&ArrowTool::drawVHDirected;
    m_drawarr[6]=&ArrowTool::drawHVDirected;
    m_drawarr[7]=&ArrowTool::drawRightLeftDirected;
    m_drawarr[8]=&ArrowTool::drawHVDirected;
    m_drawarr[9]=&ArrowTool::drawVHDirected;
    m_drawarr[10]=&ArrowTool::drawTopBottomDirected;
    m_drawarr[11]=&ArrowTool::drawVHDirected;
    m_drawarr[12]=&ArrowTool::drawHVDirected;

    m_drawllr[0]=&ArrowTool::drawZeroDirected;
    m_drawllr[1]=&ArrowTool::drawRightLeftDirected;
    m_drawllr[2]=&ArrowTool::drawHorizontalDirected3;
    m_drawllr[3]=&ArrowTool::drawVerticalDirected3;
    m_drawllr[4]=&ArrowTool::drawTopBottomDirected;
    m_drawllr[5]=&ArrowTool::drawVerticalDirected3;
    m_drawllr[6]=&ArrowTool::drawHorizontalDirected3;
    m_drawllr[7]=&ArrowTool::drawRightLeftDirected;
    m_drawllr[8]=&ArrowTool::drawHorizontalDirected3;
    m_drawllr[9]=&ArrowTool::drawVerticalDirected3;
    m_drawllr[10]=&ArrowTool::drawTopBottomDirected;
    m_drawllr[11]=&ArrowTool::drawVerticalDirected3;
    m_drawllr[12]=&ArrowTool::drawHorizontalDirected3;

    m_connection_cbs<<QVector<ArrowTool::Callback>()
                    <<QVector<ArrowTool::Callback>()
                    <<QVector<ArrowTool::Callback>();

    m_connection_cbs[ATS_NONE ]<<&ArrowTool::connectNoneToNone
                               <<&ArrowTool::connectNoneToLine
                               <<&ArrowTool::connectNoneToBox;
    m_connection_cbs[ATS_ARROW]<<&ArrowTool::connectLineToNone
                               <<&ArrowTool::connectLineToLine
                               <<&ArrowTool::connectLineToBox;
    m_connection_cbs[ATS_BOX  ]<<&ArrowTool::connectBoxToNone
                               <<&ArrowTool::connectBoxToLine
                               <<&ArrowTool::connectBoxToBox;
}


ArrowTool::~ArrowTool()
{
}

void   ArrowTool::initState()
{
    static_cast<MainWindow*>(m_scene->view()->window())->setActionText("Click to place start point of arrow");
    m_state=ATS_START;
}



void ArrowTool::clearState()
{
 if (m_state==ATS_FIRSTPOINT)
 {
   for (int i=0;i<3;i++)
       delete m_preview[0];
   m_state=ATS_START;
   m_scene->update();
 }
  static_cast<MainWindow*>(m_scene->view()->window())->setActionText("");
}


int ArrowTool::clockwiseDirection(const QPointF & pos,bool canReturnZero)
{
   QPointF pivot=m_points[0];
   //! If it's the same point return zero
   if (canReturnZero
       && fabs(pos.x()-pivot.x())<ARROW_SENSIVITY_X
       && fabs(pos.y()-pivot.y())<ARROW_SENSIVITY_Y)
   {
       return 0;
   }
   //! If this is vertical line with top direction return 4
   if (pos.y()<pivot.y() && fabs(pos.x()-pivot.x())<ARROW_SENSIVITY_X)
   {
       return 4;
   }
   //! If this is vertical line with bottom direction return 10
   if (pos.y()>=pivot.y() && fabs(pos.x()-pivot.x())<ARROW_SENSIVITY_X)
   {
       return 10;
   }
   //! If this is horizontal line with right direction return 1
   if (pos.x()>pivot.x() && fabs(pos.y()-pivot.y())<ARROW_SENSIVITY_Y)
   {
       return 1;
   }
   //! If this is horizontal line with left direction return 7
   if (pos.x()<=pivot.x() && fabs(pos.y()-pivot.y())<ARROW_SENSIVITY_Y)
   {
       return 7;
   }
   //! Right-top direction
   if (pos.x()>pivot.x() && pos.y()<pivot.y() &&  pos.x()-pivot.x()>pivot.y()-pos.y() )
   {
       return 2;
   }
   //! Top-right direction
   if (pos.x()>pivot.x() && pos.y()<pivot.y() &&  pos.x()-pivot.x()<=pivot.y()-pos.y() )
   {
       return 3;
   }
   //! Top-left direction
   if (pos.x()<=pivot.x() && pos.y()<pivot.y() &&  pivot.x()-pos.x()<=pivot.y()-pos.y() )
   {
       return 5;
   }
   //! Left-top direction
   if (pos.x()<=pivot.x() && pos.y()<pivot.y() &&  pivot.x()-pos.x()>pivot.y()-pos.y() )
   {
       return 6;
   }
   //! Left-bottom direction
   if (pos.x()<=pivot.x() && pos.y()>pivot.y() &&  pivot.x()-pos.x()>pos.y()-pivot.y() )
   {
       return 8;
   }
   //! Bottom-left direction
   if (pos.x()<=pivot.x() && pos.y()>pivot.y() &&  pivot.x()-pos.x()<=pos.y()-pivot.y() )
   {
       return 9;
   }
   //! Bottom-right direction
   if (pos.x()>pivot.x() && pos.y()>pivot.y() &&  pos.x()-pivot.x()<=pos.y()-pivot.y() )
   {
       return 11;
   }
   //! Right-bottom direction
   if (pos.x()>pivot.x() && pos.y()>pivot.y() &&  pos.x()-pivot.x()>pos.y()-pivot.y() )
   {
       return 12;
   }
   //! If control reached this point, it's some kind of bug
   return 13;
}


void ArrowTool::makeOneLineVisible()
{
  m_preview[0]->setVisible(true);
  m_preview[1]->setVisible(false);
  m_preview[2]->setVisible(false);
  m_preview[0]->model()->remove(m_preview[1]->model());
  m_preview[1]->model()->remove(m_preview[0]->model());

  m_preview[2]->model()->remove(m_preview[1]->model());
  m_preview[1]->model()->remove(m_preview[2]->model());
  m_preview_amount=1;
}


void ArrowTool::makeTwoLinesVisible()
{
    m_preview[0]->setVisible(true);
    m_preview[1]->setVisible(true);
    m_preview[2]->setVisible(false);
    if (!(m_preview[0]->model()->hasConnected(m_preview[1]->model())))
    {
        m_preview[0]->model()->addConnector(m_preview[1]->model(),1,C_OUTPUT);
        m_preview[1]->model()->addConnector(m_preview[0]->model(),0,C_INPUT);
    }
    m_preview[2]->model()->remove(m_preview[1]->model());
    m_preview[1]->model()->remove(m_preview[2]->model());
    m_preview_amount=2;
}

void ArrowTool::makeAllLinesVisible()
{
    m_preview[0]->setVisible(true);
    m_preview[1]->setVisible(true);
    m_preview[2]->setVisible(true);
    m_preview_amount=3;
    disconnectAllPreviews();
    for (int i=1;i<3;i++)
    {
        m_preview[i-1]->model()->addConnector(m_preview[i]->model(),1.0,C_OUTPUT);
        m_preview[i]->model()->addConnector(m_preview[i-1]->model(),0.0,C_INPUT);
    }
}


void    ArrowTool::drawZeroDirected(const QPointF & /* p1 */, const QPointF & /* p2 */)
{
   m_preview[0]->setVisible(false);
   m_preview[1]->setVisible(false);
   m_preview[2]->setVisible(false);
   m_preview_amount=0;
   m_scene->update();
}



void  ArrowTool::drawRightLeftDirected(const QPointF &p1, const QPointF &p2)
{
    makeOneLineVisible();
    m_preview[0]->setLine(p1.x(),p1.y(),p2.x(),p1.y());
    m_scene->update();
}

void ArrowTool::drawTopBottomDirected(const QPointF &p1, const QPointF &p2)
{
    makeOneLineVisible();
    m_preview[0]->setLine(p1.x(),p1.y(),p1.x(),p2.y());
    m_scene->update();
}


void ArrowTool::drawHVDirected(const QPointF &p1, const QPointF &p2)
{
    makeTwoLinesVisible();
    m_preview[0]->setLine(p1.x(),p1.y(),p2.x(),p1.y());
    m_preview[1]->setLine(p2.x(),p1.y(),p2.x(),p2.y());
    m_scene->update();
}

void ArrowTool::drawVHDirected(const QPointF &p1, const QPointF &p2)
{
    makeTwoLinesVisible();
    m_preview[0]->setLine(p1.x(),p1.y(),p1.x(),p2.y());
    m_preview[1]->setLine(p1.x(),p2.y(),p2.x(),p2.y());
    m_scene->update();
}

void ArrowTool::drawVerticalDirected3(const QPointF & p1, const QPointF & p2)
{
    makeAllLinesVisible();
    QPointF mid1=QPointF(p1.x(),(p1.y()+p2.y())/2);
    QPointF mid2=QPointF(p2.x(),(p1.y()+p2.y())/2);
    m_preview[0]->setLine(p1.x(),p1.y(),mid1.x(),mid1.y());
    m_preview[1]->setLine(mid1.x(),mid1.y(),mid2.x(),mid2.y());
    m_preview[2]->setLine(mid2.x(),mid2.y(),p2.x(),p2.y());
}

void  ArrowTool::drawHorizontalDirected3(const QPointF & p1, const QPointF & p2)
 {
     makeAllLinesVisible();
     QPointF mid1=QPointF((p1.x()+p2.x())/2,p1.y());
     QPointF mid2=QPointF((p1.x()+p2.x())/2,p2.y());
     m_preview[0]->setLine(p1.x(),p1.y(),mid1.x(),mid1.y());
     m_preview[1]->setLine(mid1.x(),mid1.y(),mid2.x(),mid2.y());
     m_preview[2]->setLine(mid2.x(),mid2.y(),p2.x(),p2.y());
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


void ArrowTool::removeOddSegments()
{
  if (m_preview_amount!=0)
      m_preview[m_preview_amount-1]->model()->remove(m_preview[m_preview_amount]->model());

  for (int i=m_preview_amount;i<MAX_PREVIEW_SEGMENTS;i++)
      m_scene->removeItem(m_preview[i]);
}

void ArrowTool::generatePreview(const QPointF & p)
{
    //Add new items
    ObjectConnector * i1=new ObjectConnector(QLineF(p,p+QPointF(1,0)));
    ObjectConnector * i2=new ObjectConnector(QLineF(p+QPointF(1,0),p+QPointF(1,1)));
    ObjectConnector * i3=new ObjectConnector(QLineF(p+QPointF(1,0),p+QPointF(1,1)));
    m_preview_amount=0;
    m_preview[0]=new Arrow(i1,m_diagram);
    m_preview[1]=new Arrow(i2,m_diagram);
    m_preview[2]=new Arrow(i3,m_diagram);
    m_preview[0]->setVisible(false);
    m_preview[1]->setVisible(false);
    m_preview[2]->setVisible(false);
    i1->addConnector(i2,1,C_OUTPUT);
    i2->addConnector(i1,0,C_INPUT);
    i2->addConnector(i3,1,C_OUTPUT);
    i3->addConnector(i2,0,C_INPUT);
    m_scene->addItem(m_preview[0]);
    m_scene->addItem(m_preview[1]);
    m_scene->addItem(m_preview[2]);
    m_scene->update();
}

bool ArrowTool::onClick(const QPointF &p, QGraphicsItem *  item )
{
  if (m_state==ATS_START)
  {
        m_points[0]=p;
        m_loc[0]=ATS_NONE;
        //Setup location
        if (item)
        {
            if (item->type()==IsBox)
            {
                m_boxes[0]=static_cast<Box*>(item);
                m_loc[0]=ATS_BOX;
            }
            if (item->type()==IsArrow)
            {
                m_arrows[0]=static_cast<Arrow*>(item);
                m_poses[0]=position(*(m_arrows[0]->model()),m_points[0]);
                m_points[0]=position(*(m_arrows[0]->model()),m_poses[0]);
                m_loc[0]=ATS_ARROW;
            }
        }
        generatePreview(p);
        //Set a data
        static_cast<MainWindow*>(m_scene->view()->window())->setActionText("Select a direction for arrow");
        m_scene->update();
        m_state=ATS_FIRSTPOINT;
  }
  else
  {
      redrawLines(p,false);
      m_points[1]=m_preview[m_preview_amount-1]->model()->p2();
      if (item)
      {
          if (item->type()==IsArrow)
          {
            m_arrows[1]=static_cast<Arrow*>(item);
            if (m_arrows[1]==m_preview[0] || m_arrows[1]==m_preview[1] || m_arrows[1]==m_preview[2])
                return false;
            QPointF  ppos=m_preview[m_preview_amount-1]->model()->p2();
            m_poses[1]=position(*(m_arrows[1]->model()),ppos);
            m_loc[1]=ATS_ARROW;
          }
          else if (item->type()==IsBox)
          {
              m_boxes[1]=static_cast<Box*>(item);
              m_loc[1]=ATS_BOX;
          }
          else return true;
      } else m_loc[1]=ATS_NONE;
      ArrowTool::Callback cb=m_connection_cbs[m_loc[0]][m_loc[1]];
      (this->*cb)();
  }
  return true;
}

bool ArrowTool::onRelease(const QPointF & /* p */, QGraphicsItem * /* item */)
{
  return true;
}

bool ArrowTool::onKeyDown(QKeyEvent *  event, QGraphicsItem * /* item */)
{
  if (m_state==ATS_FIRSTPOINT && event->key()==Qt::Key_Escape)
  {
     for (int i=0;i<3;i++)
        delete m_preview[i];
     m_scene->update();
     static_cast<MainWindow*>(m_scene->view()->window())->setActionText("Click to place start point of arrow");
     m_state=ATS_START;
  }
  return true;
}


void ArrowTool::redrawLines(const QPointF & pos,bool canBeZero)
{
    int direction=clockwiseDirection(pos,canBeZero);
    Box * box=NULL;
    Arrow * arrow=NULL;
    QPointF p2=pos;
    {
    QList<QGraphicsItem *> lst=m_scene->items(pos);
    m_diagram->items(pos,lst);
    QList<QGraphicsItem *> all=m_scene->items();
    //static_cast<MainWindow*>(m_scene->view()->window())->setActionText(QString::number(lst.size())
    //                                                                  +QString("  ")
    //                                                                  +QString::number(all.size()));
    for (int i=0;i<lst.size() && !box && !arrow;i++)
    {
        if (lst[i]->type()==IsBox)
        {
            box=static_cast<Box*>(lst[i]);
        }
        else
        {
             if (lst[i]->type()==IsArrow && lst[i]!=m_preview[0]
                                         && lst[i]!=m_preview[1]
                                         && lst[i]!=m_preview[2])
                 arrow=static_cast<Arrow*>(lst[i]);
        }
    }
    if (arrow)
    {
        qreal apos=position(*(arrow->model()),p2);
        p2=position(*(arrow->model()),apos);
        //QString id=QString::number(p2.x())+QString(" ")+QString::number(p2.y())+QString(" ")+QString::number(apos);
        //static_cast<MainWindow*>(m_scene->view()->window())->setActionText(id);
    }
    if (box)
    {
        if (direction==1
            || direction==3
            || direction==11) p2.setX(box->collisionRect().left());
        if (direction==5
            || direction==7
            || direction==9) p2.setX(box->collisionRect().right());
        if (direction==2
            || direction==4
            || direction==6) p2.setY(box->collisionRect().bottom());
        if (direction==8
            || direction==10
            || direction==12) p2.setY(box->collisionRect().top());
    }
    }

    QPointF p1=m_points[0];

    if (m_loc[0]==ATS_BOX)
    {
          if (box==m_boxes[0])
              direction=0;
          if (direction==1
              || direction==2
              || direction==12)  p1.setX(m_boxes[0]->collisionRect().right());
          if  (direction==3
               || direction==4
               || direction==5)  p1.setY(m_boxes[0]->collisionRect().top());
          if (direction==6
              || direction==7
              || direction==8)   p1.setX(m_boxes[0]->collisionRect().left());
          if (direction==9
              || direction==10
              || direction==11)  p1.setY(m_boxes[0]->collisionRect().bottom());
    }
    direction=clockwiseDirection(p2,canBeZero);
    void (ArrowTool::*ptr)(const QPointF&, const QPointF&);
    if (m_loc[0]==ATS_ARROW && arrow)  ptr=m_drawllr[direction];  else  ptr=m_drawarr[direction];
    (this->*ptr)(p1,p2);
}

void ArrowTool::onMove(const QPointF & /* lastpos */ , const QPointF &pos)
{
  //Update preview only if first point specified
  if (m_state==ATS_FIRSTPOINT)
  {
     redrawLines(pos,true);
  }
}


bool ArrowTool::canPlacePreviews()
{
    QVector<int> exctypes; exctypes<<IsArrow<<IsCommentLine;
    QVector<CollisionObject *> excobjs;
    if (m_loc[0]==ATS_ARROW) excobjs<<m_arrows[0];
    if (m_loc[0]==ATS_BOX) excobjs<<m_boxes[0];
    if (m_loc[1]==ATS_ARROW) excobjs<<m_arrows[1];
    if (m_loc[1]==ATS_BOX) excobjs<<m_boxes[1];
    bool canplace=true;
    for (int i=0;i<m_preview_amount;i++)
    {
        canplace=canplace && m_diagram->canPlace(m_preview[i],exctypes,excobjs);
    }
    return canplace;
}

void ArrowTool::addPreviewsToDiagram()
{
   for (int i=0;i<m_preview_amount;i++)
       m_diagram->add(m_preview[i]);
}

void ArrowTool::disconnectAllPreviews()
{
  for (int i=0;i<m_preview_amount;i++)
      m_preview[i]->model()->clear();
}

void ArrowTool::connectNoneToNone()
{
  if (canPlacePreviews())
  {
    addPreviewsToDiagram();
    m_loc[0]=ATS_ARROW;
    m_points[0]=m_preview[m_preview_amount-1]->model()->p2();
    m_poses[0]=1;
    m_arrows[0]=m_preview[m_preview_amount-1];
    removeOddSegments();
    generatePreview(m_points[0]);
  }
}

void ArrowTool::connectNoneToLine()
{
    Arrow * lastpreview=m_preview[m_preview_amount-1];
    Direction lsegdir=lastpreview->model()->direction();
    Direction enddir=m_arrows[1]->model()->direction();
    bool isNotOpposite=!isOpposite(lsegdir,enddir);
    bool isBadCollision=lsegdir==enddir && m_poses[1]!=0;
    bool canplace=canPlacePreviews();
    if (isNotOpposite && !isBadCollision && canplace)
    {
        disconnectAllPreviews();
        QVector<DiagramObject *> v;
        if (lsegdir==enddir)
        {
            m_arrows[1]->model()->enlarge(lastpreview->model()->p1());
            delete lastpreview;
            m_preview[m_preview_amount-1]=m_arrows[1];
        }
        else
        {
            m_arrows[1]->model()->addConnector(lastpreview->model(),m_poses[1],C_INPUT);
            m_preview[m_preview_amount-1]->model()->addConnector(m_arrows[1]->model(),1.0,C_OUTPUT);
        }
        for (int i=1;i<m_preview_amount;i++)
        {
            m_preview[i-1]->model()->addConnector(m_preview[i]->model(),1.0,C_OUTPUT);
            m_preview[i]->model()->addConnector  (m_preview[i-1]->model(),0.0,C_INPUT);
        }
        //Perform addition
        addPreviewsToDiagram();
        removeOddSegments();
        initState();
    }
}

void ArrowTool::connectNoneToBox()
{
    Direction dir=getSide(m_boxes[1]->collisionRect(),m_preview[m_preview_amount-1]->model()->p2());
    bool directed_right=dir==D_TOP || dir==D_LEFT || dir==D_BOTTOM;
    bool canplace=canPlacePreviews();
    if (directed_right && canplace)
    {
        //Get position
        ObjectConnector * bx=m_boxes[1]->getBySide(dir);
        qreal pos=position( *(bx) ,m_preview[m_preview_amount-1]->model()->p2());
        //Add connectors
        bx->addConnector(m_preview[m_preview_amount-1]->model(),pos,C_INPUT);
        m_preview[m_preview_amount-1]->model()->addConnector(bx,1,C_OUTPUT);
        //Perform addition
        addPreviewsToDiagram();
        removeOddSegments();
        initState();
    }
}

void ArrowTool::connectLineToNone()
{
    Arrow * fpreview=m_preview[0];
    Direction fsegdir=fpreview->model()->direction();
    Direction begdir=m_arrows[0]->model()->direction();
    bool isNotOpposite=!isOpposite(fsegdir,begdir);
    bool isBadCollision=fsegdir==begdir && m_poses[0]!=1;
    bool canplace=canPlacePreviews();
    if (isNotOpposite && !isBadCollision && canplace)
    {
        disconnectAllPreviews();
        QVector<DiagramObject *> v;
        if (fsegdir==begdir)
        {
            m_arrows[0]->model()->enlarge(fpreview->model()->p2());
            delete fpreview;
            m_preview[0]=m_arrows[0];
            m_preview[0]->regenerate();
        }
        else
        {
            m_arrows[0]->model()->addConnector(fpreview->model(),m_poses[0],C_OUTPUT);
            m_preview[0]->model()->addConnector(m_arrows[0]->model(),0.0,C_INPUT);
        }
        for (int i=1;i<m_preview_amount;i++)
        {
            m_preview[i-1]->model()->addConnector(m_preview[i]->model(),1.0,C_OUTPUT);
            m_preview[i]->model()->addConnector  (m_preview[i-1]->model(),0.0,C_INPUT);
        }
        //Perform addition
        addPreviewsToDiagram();
        m_loc[0]=ATS_ARROW;
        m_points[0]=m_preview[m_preview_amount-1]->model()->p2();
        m_poses[0]=1;
        m_arrows[0]=m_preview[m_preview_amount-1];
        removeOddSegments();
        generatePreview(m_points[0]);
    }
}

void ArrowTool::connectLineToLine()
{
    Arrow * lastpreview=m_preview[m_preview_amount-1];
    Direction lsegdir=lastpreview->model()->direction();
    Direction enddir=m_arrows[1]->model()->direction();
    bool isNotOpposite=!isOpposite(lsegdir,enddir);
    bool isBadCollision=lsegdir==enddir && m_poses[1]!=0;
    Arrow * fpreview=m_preview[0];
    Direction fsegdir=fpreview->model()->direction();
    Direction begdir=m_arrows[0]->model()->direction();
    bool isNotOpposite2=!isOpposite(fsegdir,begdir);
    bool isBadCollision2=fsegdir==begdir && m_poses[0]!=1;
    bool canplace=canPlacePreviews();
    bool notHasWay=!hasWay(m_arrows[0]->model(),m_arrows[1]->model());
    if (isNotOpposite && !isBadCollision
                      && isNotOpposite2
                      && !isBadCollision2
                      && canplace
                      && notHasWay)
    {
        disconnectAllPreviews();
        QVector<DiagramObject *> v;
        if (fsegdir==begdir)
        {
            m_arrows[0]->model()->enlarge(fpreview->model()->p2());
            delete fpreview;
            m_preview[0]=m_arrows[0];
            m_preview[0]->regenerate();
        }
        else
        {
            m_arrows[0]->model()->addConnector(fpreview->model(),m_poses[0],C_OUTPUT);
            m_preview[0]->model()->addConnector(m_arrows[0]->model(),0.0,C_INPUT);
        }
        if (lsegdir==enddir)
        {
            m_arrows[1]->model()->enlarge(lastpreview->model()->p1());
            delete lastpreview;
            m_preview[m_preview_amount-1]=m_arrows[1];
        }
        else
        {
            m_arrows[1]->model()->addConnector(lastpreview->model(),m_poses[1],C_INPUT);
            m_preview[m_preview_amount-1]->model()->addConnector(m_arrows[1]->model(),1.0,C_OUTPUT);
        }
        for (int i=1;i<m_preview_amount;i++)
        {
            m_preview[i-1]->model()->addConnector(m_preview[i]->model(),1.0,C_OUTPUT);
            m_preview[i]->model()->addConnector  (m_preview[i-1]->model(),0.0,C_INPUT);
        }
        //Perform addition
        addPreviewsToDiagram();
        removeOddSegments();
        initState();
    }
}

void ArrowTool::connectLineToBox()
{
    Arrow * fpreview=m_preview[0];
    Direction fsegdir=fpreview->model()->direction();
    Direction begdir=m_arrows[0]->model()->direction();
    bool isNotOpposite=!isOpposite(fsegdir,begdir);
    bool isBadCollision=fsegdir==begdir && m_poses[0]!=1;
    Direction dir=getSide(m_boxes[1]->collisionRect(),m_preview[m_preview_amount-1]->model()->p2());
    bool directed_right=dir==D_TOP || dir==D_LEFT || dir==D_BOTTOM;
    bool canplace=canPlacePreviews();
    Arrow * lastpreview=m_preview[m_preview_amount-1];
    Direction lsegdir=lastpreview->model()->direction();
    bool    notcollideswithbox=lsegdir!=dir;
    if (isNotOpposite && !isBadCollision && directed_right && canplace && notcollideswithbox)
    {
        disconnectAllPreviews();
        QVector<DiagramObject *> v;
        if (fsegdir==begdir)
        {
            m_arrows[0]->model()->enlarge(fpreview->model()->p2());
            delete fpreview;
            m_preview[0]=m_arrows[0];
            m_preview[0]->regenerate();
        }
        else
        {
            m_arrows[0]->model()->addConnector(fpreview->model(),m_poses[0],C_OUTPUT);
            m_preview[0]->model()->addConnector(m_arrows[0]->model(),0.0,C_INPUT);
        }
        for (int i=1;i<m_preview_amount;i++)
        {
            m_preview[i-1]->model()->addConnector(m_preview[i]->model(),1.0,C_OUTPUT);
            m_preview[i]->model()->addConnector  (m_preview[i-1]->model(),0.0,C_INPUT);
        }
        //Get position
        ObjectConnector * bx=m_boxes[1]->getBySide(dir);
        qreal pos=position( *(bx) ,m_preview[m_preview_amount-1]->model()->p2());
        //Add connectors
        bx->addConnector(m_preview[m_preview_amount-1]->model(),pos,C_INPUT);
        m_preview[m_preview_amount-1]->model()->addConnector(bx,1,C_OUTPUT);
        //Perform addition
        addPreviewsToDiagram();
        removeOddSegments();
        initState();
    }
}

void ArrowTool::connectBoxToNone()
{
  Direction dir=getSide(m_boxes[0]->collisionRect(),m_preview[0]->model()->p1());
  bool directed_right=dir==D_RIGHT || dir==D_BOTTOM;
  bool canplace=canPlacePreviews();
  if (directed_right && canplace)
  {
      //Get position
      ObjectConnector * bx=m_boxes[0]->getBySide(dir);
      qreal pos=position( *(bx) ,m_preview[0]->model()->p1());
      //Add connectors
      bx->addConnector(m_preview[0]->model(),pos,C_OUTPUT);
      m_preview[0]->model()->addConnector(bx,0,C_INPUT);
      //Perform addition
      addPreviewsToDiagram();
      m_loc[0]=ATS_ARROW;
      m_points[0]=m_preview[m_preview_amount-1]->model()->p2();
      m_poses[0]=1;
      m_arrows[0]=m_preview[m_preview_amount-1];
      removeOddSegments();
      generatePreview(m_points[0]);
  }
}

void ArrowTool::connectBoxToLine()
{
    Direction dir=getSide(m_boxes[0]->collisionRect(),m_preview[0]->model()->p1());
    bool directed_right=dir==D_RIGHT || dir==D_BOTTOM;
    bool canplace=canPlacePreviews();
    Arrow * lastpreview=m_preview[m_preview_amount-1];
    Direction lsegdir=lastpreview->model()->direction();
    Direction enddir=m_arrows[1]->model()->direction();
    bool isNotOpposite=!isOpposite(lsegdir,enddir);
    bool isBadCollision=lsegdir==enddir && m_poses[1]!=0;
    if (isNotOpposite && !isBadCollision && directed_right && canplace)
    {
        disconnectAllPreviews();
        QVector<DiagramObject *> v;
        if (lsegdir==enddir)
        {
            m_arrows[1]->model()->enlarge(lastpreview->model()->p1());
            delete lastpreview;
            m_preview[m_preview_amount-1]=m_arrows[1];
        }
        else
        {
            m_arrows[1]->model()->addConnector(lastpreview->model(),m_poses[1],C_INPUT);
            m_preview[m_preview_amount-1]->model()->addConnector(m_arrows[1]->model(),1.0,C_OUTPUT);
        }
        for (int i=1;i<m_preview_amount;i++)
        {
            m_preview[i-1]->model()->addConnector(m_preview[i]->model(),1.0,C_OUTPUT);
            m_preview[i]->model()->addConnector  (m_preview[i-1]->model(),0.0,C_INPUT);
        }
        m_poses[0]=position(*(m_boxes[0]->getBySide(dir)),m_preview[0]->model()->p1());
        m_boxes[0]->getBySide(dir)->addConnector(m_preview[0]->model(),m_poses[0],C_OUTPUT);
        m_preview[0]->model()->addConnector(m_boxes[0]->getBySide(dir),0.0,C_INPUT);
        //Perform addition
        addPreviewsToDiagram();
        removeOddSegments();
        initState();
    }
}

void ArrowTool::connectBoxToBox()
{
    Direction dir_input=getSide(m_boxes[0]->collisionRect(),m_preview[0]->model()->p1());
    bool directed_right_i=dir_input==D_RIGHT || dir_input==D_BOTTOM;
    Direction dir_output=getSide(m_boxes[1]->collisionRect(),m_preview[m_preview_amount-1]->model()->p2());
    bool directed_right_o=dir_output==D_TOP || dir_output==D_LEFT || dir_output==D_BOTTOM;
    bool canplace=canPlacePreviews();
    bool notsamebox=m_boxes[0]!=m_boxes[1];
    if (directed_right_i && directed_right_o && canplace && notsamebox)
    {
        //Fill connections
        ObjectConnector * bxin=m_boxes[0]->getBySide(dir_input);
        qreal inpos=position(*bxin,m_preview[0]->model()->p1());
        bxin->addConnector(m_preview[0]->model(),inpos,C_OUTPUT);
        m_preview[0]->model()->addConnector(bxin,0.0,C_INPUT);
        ObjectConnector * bxout=m_boxes[1]->getBySide(dir_output);
        qreal outpos=position(*bxout,m_preview[m_preview_amount-1]->model()->p2());
        bxout->addConnector(m_preview[m_preview_amount-1]->model(),outpos,C_INPUT);
        m_preview[m_preview_amount-1]->model()->addConnector(bxout,1.0,C_OUTPUT);
        //Perform addition
        addPreviewsToDiagram();
        removeOddSegments();
        initState();
    }
}


