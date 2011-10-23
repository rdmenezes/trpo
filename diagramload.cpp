#include "diagramset.h"
#include "diagram.h"
#include "alabelitem.h"
#include "alineitem.h"
#include "arrowpoint.h"
#include "arrowsegment.h"
#include "boxitem.h"
#include "saveloadutils.h"
#include "loadelements.h"




void DiagramSet::load(QDomElement * e)
{
 QDomNamedNodeMap attrs=e->attributes();
 if (attrs.contains("maxid"))
 {
     QDomNode id=attrs.namedItem("maxid");
     this->m_max_id=stringToInt(id.toAttr().value());
     if (m_max_id!=1)
         m_max_id=1;
 }
 int diagram=0;
 QDomNode n=e->firstChild();
 while (diagram!=m_max_id || !(n.isNull()))
 {
   QDomElement diagel=n.toElement();
   if (!diagel.isNull())
   {
       if (diagel.tagName() == "diagram")
       {
           if (! (m_set.contains(diagram))) m_set.insert(diagram,new Diagram());
           m_set[diagram]->load(&diagel,this);
           ++diagram;
       }
   }
   n=n.nextSibling();
 }
}



void Diagram::load(QDomElement * e,DiagramSet * parent)
{
  m_set=parent;

  DiagramLoadData * data=new DiagramLoadData();
  data->diagram=this;
  QDomNamedNodeMap attrs=e->attributes();

  if (attrs.contains("id"))
  {
      QDomNode id=attrs.namedItem("id");
      this->setID(stringToInt(id.toAttr().value()));
  }
  if (attrs.contains("location"))
  {
       QDomNode loc=attrs.namedItem("location");
       this->m_parent=stringToLocation(loc.toAttr().value());
  }
  //Populate diagram with items
  QDomNode n=e->firstChild();
  while(! (n.isNull()) )
  {
      QDomElement el=n.toElement();
      if (!(el.isNull()))
      {
          if (el.tagName() == "block")            loadBlock(&el,data);
          if (el.tagName() == "annotation_line")  loadAnnotationLine(&el,data);
          if (el.tagName() == "annotation_label") loadAnnotationLabel(&el,data);
          if (el.tagName() == "segment")          loadSegment(&el,data);
          if (el.tagName() == "point")            loadPoint(&el,data);
      }
      n=n.nextSibling();
  }

  //Correct diagram's items
  correctItems(data);
  delete data;
}


void correctItems(DiagramLoadData * data)
{
 Diagram * dia=data->diagram;
 //Correct block line references
 for (int i=0;i<DIAGRAM_MAX_BLOCKS;i++)
 {
     BoxItem * block=dia->getBlockByID(i);
     if (block)
     {
       for (int j=0;j<BLOCK_SIDES;j++)
       {
           for (int k=0;k<MAX_LINE_REFERENCES;k++)
           {
               ArrowPoint *& ref=block->getLineRef(j,k);
               if (ref!=NULL)
               {
                   if (data->points.contains(ref))
                       ref=data->points[ref];
                   else
                       ref=NULL;
               }
           }
       }
     }
 }
 //Correct annotation lines
 for (int i=0;i<dia->annotationLines().size();i++)
 {
     ALineItem * aline=dia->annotationLines()[i];
     aline->setDiagram(dia);
     if (aline->isAttachedToLine())
     {
         ArrowPoint *&  ref=reinterpret_cast<ArrowPoint*&>(aline->accessBinded());
         if (data->points.contains(ref))
             ref=data->points[ref];
         else  //Set a new reference to null point
         {
             ref=new ArrowPoint(0,0);
             dia->arrowPoints()<<ref;
         }
     }
 }
 //Correct arrow segments
 for (int i=0;i<dia->arrowSegments().size();i++)
 {
     ArrowSegment * seg=dia->arrowSegments()[i];
     seg->setDiagram(dia);
     ArrowPoint *& refin=seg->accessIn();
     if (data->points.contains(refin))
         refin=data->points[refin];
     else
     {
         refin=new ArrowPoint(0,0);
         dia->arrowPoints()<<refin;
     }
     ArrowPoint *& refout=seg->accessOut();
     if (data->points.contains(refout))
         refout=data->points[refout];
     else
     {
         refout=new ArrowPoint(0,0);
         dia->arrowPoints()<<refout;
     }
 }
 //Correct arrow points
 for (int i=0;i<dia->arrowPoints().size();i++)
 {
   ArrowPoint * p=dia->arrowPoints()[0];
   p->setDiagram(dia);
   //Set Block
   BoxItem *& block=p->accessBlock();
   if (block && data->blocks.contains(block)) block=data->blocks[block];
   else block=NULL;
   //Set in
   QVector<ArrowSegment *> & in=p->accessIn();
   for (int i=0;i<in.size();i++)
   {
       if (data->segments.contains(in[i])) in[i]=data->segments[in[i]];
       else  { in.remove(i); --i;}
   }
   //Set out
   QVector<ArrowSegment *> & out=p->accessOut();
   for (int i=0;i<out.size();i++)
   {
       if (data->segments.contains(out[i])) out[i]=data->segments[out[i]];
       else  { out.remove(i); --i;}
   }
   //Set lines
   QVector<ALineItem *> lines=p->accessLines();
   for (int i=0;i<lines.size();i++)
   {
       if (data->annotationlines.contains(lines[i]))  lines[i]=data->annotationlines[lines[i]];
       else { lines.remove(i); --i; }
   }
 }
}
