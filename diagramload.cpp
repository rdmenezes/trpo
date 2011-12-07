#include "diagramset.h"
#include "diagram.h"
#include "freecomment.h"
#include "commentline.h"
#include "box.h"
#include "saveloadutils.h"
#include "loadelements.h"



/*! TODO: Replace it with properly written code
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
*/

/*! TODO: Reimplement
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
*/
/*
void correctItems(DiagramLoadData * data)
{
 Diagram * dia=data->diagram;
 //Correct block line references
 for (int i=0;i<DIAGRAM_MAX_BLOCKS;i++)
 {
     Box * block=dia->getBlockByID(i);
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
     CommentLine * aline=dia->annotationLines()[i];
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
   ArrowPoint * p=dia->arrowPoints()[i];
   p->setDiagram(dia);
   //Set Block
   Box *& block=p->accessBlock();
   if (block && data->blocks.contains(block)) block=data->blocks[block];
   else block=NULL;
   //Set in
   QVector<ArrowSegment *> & in=p->accessIn();
   for (int j=0;j<in.size();j++)
   {
       if (data->segments.contains(in[j])) in[j]=data->segments[in[j]];
       else
       {
           in.remove(j); --j;
       }
   }
   //Set out
   QVector<ArrowSegment *> & out=p->accessOut();
   for (int j=0;j<out.size();j++)
   {
       if (data->segments.contains(out[j]))
       {
           out[j]=data->segments[out[j]];
       }
       else
       {
           out.remove(j); --j;
       }
   }
   //Set lines
   QVector<CommentLine *> & lines=p->accessLines();
   for (int j=0;j<lines.size();j++)
   {
       if (data->annotationlines.contains(lines[j]))
           lines[j]=data->annotationlines[lines[j]];
       else
       {
           lines.remove(j); --j;
       }
   }
 }
}
*/
