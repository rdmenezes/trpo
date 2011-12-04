#include "diagramset.h"
#include "diagram.h"
#include "freecomment.h"
#include "commentline.h"
#include "arrowpoint.h"
#include "arrowsegment.h"
#include "box.h"
#include "saveloadutils.h"
#include "loadelements.h"
#include <QDomAttr>

void loadBlockRef(QDomElement * el,Box * box, int &i, int &j)
{
  if(i==BLOCK_SIDES)
      return;


 QDomNamedNodeMap map=el->attributes();
 if (map.contains("value"))
     box->getLineRef(i,j)=reinterpret_cast<ArrowPoint*>(stringToPtr(
                          map.namedItem("value").toAttr().value()
                          ));

  ++j;
  if (j==MAX_LINE_REFERENCES)
       { j=0; ++i; }
}

QString getAttr(QDomNamedNodeMap & map,const QString & name)
{
    return map.namedItem(name).toAttr().value();
}

void loadBlock(QDomElement * el,DiagramLoadData * data)
{
    /*
    QDomNamedNodeMap map=el->attributes();
    void * me=NULL;
    int    id=0;
    if (map.contains("this"))
         me=stringToPtr(getAttr(map,"this"));
    if (map.contains("id"))
        id=stringToInt(getAttr(map,"id"));

    if (!me)
    {
        data->diagram->setBlockID(NULL,(char)id);
        return;
    }
    Box * item=new Box();
    data->diagram->setBlockID(item,(char)id);
    data->blocks.insert(me,item);
    item->setID((char)id);
    if (map.contains("real_string"))
    {
        QString rs=stringXMLToString(getAttr(map,"real_string"));
        item->accessRealString()=rs;
    }
    if (map.contains("viewed_string"))
    {
        QString vs=stringXMLToString(getAttr(map,"viewed_string"));
        item->accessViewedString()=vs;
    }
    if (map.contains("string_pos"))
    {
        QRect spos=stringToRect(getAttr(map,"string_pos"));
        item->accessStringPos()=spos;
    }
    if (map.contains("number_pos"))
    {
        QPointF sps=stringToPointf(getAttr(map,"number_pos"));
        item->accessNumberPos()=sps;
    }
    if (map.contains("rect"))
    {
        QRectF rct=stringToRectf(getAttr(map,"rect"));
        item->accessRect()=rct;
    }
    if (map.contains("childid"))
    {
        int childid=stringToInt(getAttr(map,"childid"));
        item->setChildDiagram(childid);
    }
    int i=0,j=0;
    QDomNode n=el->firstChild();
    while (! ( n.isNull() ) )
    {
        QDomElement refel=el->toElement();
        if (! (refel.isNull()))
        {
            if (refel.tagName()=="ptr")
                loadBlockRef(&refel,item,i,j);
        }
        n=n.nextSibling();
    }
    */
}

void loadAnnotationLine(QDomElement * el,DiagramLoadData *  data)
{
 bool is_attached=false;
 QDomNamedNodeMap map=el->attributes();
 if (map.contains("binded"))
     is_attached= map.namedItem("binded").toAttr().value() == "true";
 void * me=NULL;
 if (map.contains("this"))
     me=stringToPtr(getAttr(map,"this"));
  CommentLine * line=new CommentLine();
  line->accessIsBinded()=is_attached;
  data->diagram->annotationLines()<<line;
  data->annotationlines.insert(me,line);
  if (map.contains("free"))
      line->accessFree()=stringToPointf(getAttr(map,"free"));
  if (is_attached)
  {
      if (map.contains("binded_ptr"))
      {
          void * aload=stringToPtr(getAttr(map,"binded_ptr"));
          line->accessBinded()=(QPointF*)aload;
      }
  }
  else
  {
      if (map.contains("coords"))
          line->accessBinded()=new QPointF(stringToPointf(getAttr(map,"coords")));
  }


}

void loadAnnotationLabel(QDomElement * el,DiagramLoadData * data)
{
   QString name;
   QRectF   rect;
   QDomNamedNodeMap map=el->attributes();
   if (map.contains("string"))
       name=stringXMLToString(getAttr(map,"string"));
   if (map.contains("rect"))
       rect=stringToRectf(getAttr(map,"rect"));
   //FreeComment * label=new FreeComment();
   //label->string()=name;
   //label->rect()=rect;
   //data->diagram->annotationLabels()<<label;
}

void loadSegment(QDomElement * el,DiagramLoadData * data)
{
  QDomNamedNodeMap map=el->attributes();
  void * me=NULL;
  ArrowPoint * in=NULL;
  ArrowPoint * out=NULL;
  if (map.contains("this"))
      me=stringToPtr(getAttr(map,"this"));
  if (map.contains("in"))
      in=(ArrowPoint *)stringToPtr(getAttr(map,"in"));
  if (map.contains("out"))
      out=(ArrowPoint *)stringToPtr(getAttr(map,"out"));
  ArrowSegment * seg=new ArrowSegment();
  seg->accessIn()=in;
  seg->accessOut()=out;
  data->segments.insert(me,seg);
  data->diagram->arrowSegments()<<seg;
}



template<typename T>
void loadPtrToVector(QDomElement * el,QVector<T>  & vec)
{
  void * me=NULL;
  QDomNamedNodeMap map=el->attributes();
  if (map.contains("ptr"))
        me=stringToPtr(getAttr(map,"ptr"));
  if (me)
      vec<<(T)me;
}

void loadPoint(QDomElement * el,DiagramLoadData * data)
{
   QDomNamedNodeMap map=el->attributes();
   void * me=NULL;
   ArrowPoint * arrow=new ArrowPoint(0,0);
   if (map.contains("this"))
       me=stringToPtr(getAttr(map,"this"));
   if (map.contains("block"))
       arrow->accessBlock()=(Box*)stringToPtr(getAttr(map,"block"));
   if (map.contains("x"))
       arrow->setX(stringToDouble(getAttr(map,"x")));
   if (map.contains("y"))
       arrow->setY(stringToDouble(getAttr(map,"y")));
   data->points.insert(me,arrow);
   data->diagram->arrowPoints()<<arrow;
   QDomNode n=el->firstChild();
   while (! ( n.isNull() ) )
   {
       QDomElement refel=n.toElement();
       if (! (refel.isNull()))
       {
           if (refel.tagName()=="in")
               loadPtrToVector<ArrowSegment *>(&refel,arrow->accessIn());
           if (refel.tagName()=="out")
               loadPtrToVector<ArrowSegment *>(&refel,arrow->accessOut());
           if (refel.tagName()=="line")
               loadPtrToVector<CommentLine *>(&refel,arrow->accessLines());
       }
       n=n.nextSibling();
   }
}
