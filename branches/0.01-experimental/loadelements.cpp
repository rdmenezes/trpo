#include "diagramset.h"
#include "diagram.h"
#include "alabelitem.h"
#include "alineitem.h"
#include "arrowpoint.h"
#include "arrowsegment.h"
#include "boxitem.h"
#include "saveloadutils.h"
#include "loadelements.h"
#include <QDomAttr>

void loadBlockRef(QDomElement * el,BoxItem * box, int &i, int &j)
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

QString getAttr(QDomNamedNodeMap & map,QString name)
{
    return map.namedItem(name).toAttr().value();
}

void loadBlock(QDomElement * el,DiagramLoadData * data)
{
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
    BoxItem * item=new BoxItem();
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
}

void loadAnnotationLine(QDomElement * el,DiagramLoadData *  data)
{

}

void loadAnnotationLabel(QDomElement * el,DiagramLoadData * data)
{

}

void loadSegment(QDomElement * el,DiagramLoadData * data)
{

}

void loadPoint(QDomElement * el,DiagramLoadData * data)
{

}
