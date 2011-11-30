#include "toolpanel.h"
#include "diagramscene.h"
#include <QPaintEvent>
#include <QStylePainter>
#include <QPainter>
#include <QImage>
ToolPanel::ToolPanel(QWidget *parent) :
    QWidget(NULL)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);
    //setStyleSheet("background-color: transparent;");
    //Retrieve scene from parent
    m_scene=static_cast<DiagramScene *>(
                                        static_cast<QGraphicsView *>(parent)
                                        ->scene()
                                       );
}

//Amount of gradient blocks
#define GRADIENT_PARTS_2 5
void ToolPanel::paintEvent(QPaintEvent *)
{
 //Create a painter
 QPainter painter(this);
 painter.setRenderHint(QPainter::HighQualityAntialiasing,true);

 //Draw a substrate gradient
 painter.setPen(Qt::NoPen);
 double dheight=(this->height())/2.0/GRADIENT_PARTS_2;
 double dcolor=48.0f/GRADIENT_PARTS_2;
 for(int i=0;i<GRADIENT_PARTS_2;i++)
 {
   painter.setBrush(QColor(0,0,255,32+dcolor*i));
   painter.drawRect(QRect(0,dheight*i,this->width()-PANEL_BORDER,dheight));
   int cdheight=0;
   if (i!=0)
       cdheight=dheight;
   else
       cdheight=dheight-PANEL_BORDER;
   painter.drawRect(QRect(0,this->height()-dheight*(i+1),
                          this->width()-PANEL_BORDER,
                          cdheight));
 }
 //Draw a substrate
 QPen pen(QColor(0,0,0,64));
 painter.setPen(pen);
 painter.setBrush(QBrush(QColor(0,0,0,0)));
 painter.drawRoundedRect(QRect(0,0,
                             this->width()-PANEL_BORDER,this->height()-PANEL_BORDER
                             ),PANEL_ROUND_RADIUS,PANEL_ROUND_RADIUS);

 painter.setBrush(Qt::NoBrush);
 for (int i=1;i<7;i++)
     drawIconByIndex(&painter,i);
}

void ToolPanel::drawIconByIndex(QPainter * p,int i)
{
    QString name(":/");
    name+=QString::number(i);
    name+=".png";
    QImage img(name);
    int y=PANEL_Y_OFFSET;
    int x=PANEL_FIRST_X_OFFSET+PANEL_X_OFFSET*(i-1);

    p->drawImage(QPoint(x,y),img);
    p->drawRect(QRect(x,y,PANEL_ICON_SIZE,PANEL_ICON_SIZE));
}

void ToolPanel::mousePressEvent(QMouseEvent *event)
{
    ToolType result=TT_SELECT;
    bool handled=false;
    //Scan for buttons
    {
    ToolType types[6]={TT_SELECT,TT_ERASER,TT_BLOCK,TT_ARROW,TT_ANNOTATION_LINE,
                       TT_ANNOTATION_LABEL};
    for (int i=0;i<6;i++)
    {
      int dy=event->pos().y()-PANEL_Y_OFFSET;
      int dx=event->pos().x()-PANEL_FIRST_X_OFFSET-PANEL_X_OFFSET*i;
      if (dx>=0  && dy>=0 && dx<=PANEL_ICON_SIZE && dy<=PANEL_ICON_SIZE)
      {
          result=types[i];
          handled=true;
      }
    }
    }
    if (handled)
        m_scene->setTool(result);
}
