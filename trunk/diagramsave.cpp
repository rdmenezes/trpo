#include "diagramset.h"
#include "diagram.h"
#include "freecomment.h"
#include "commentline.h"
#include "arrowpoint.h"
#include "arrowsegment.h"
#include "box.h"
#include "saveloadutils.h"
#include <QDomDocument>
#include <QDomElement>


/*
void DiagramSet::save(QDomDocument *doc)
{
  QDomElement set=doc->createElement("set");
  set.setAttribute("maxid",intToString(m_max_id));
  //Here must be placed actions of appending each diagram
  for (QHash<int,Diagram *>::iterator it=m_set.begin();it!=m_set.end();it++ )
      it.value()->save(doc,&set);
  doc->appendChild(set);
}
*/
/*! Handles empty block
    \param[in] doc document
    \param[in] diag diagram
    \param[in] id   id diagram
 */
void handleEmptyBlock(QDomDocument * doc,QDomElement * diag,int id);
/*! TODO: Reimplement
void Diagram::save(QDomDocument * doc,QDomElement * sete)
{
  QDomElement diag=doc->createElement("diagram");
  //diag.setAttribute("id",intToString(this->getID()));
  //diag.setAttribute("location",locationToString(m_parent));
  //Here must be placed actions of appending each element
  for (int i=0;i<DIAGRAM_MAX_BLOCKS;i++)
  {
    if (m_boxes[i])
        m_boxes[i]->save(doc,&diag);
    else
        handleEmptyBlock(doc,&diag,i);
  }
  for (int i=0;i<m_alabels.size();i++)
      m_alabels[i]->save(doc,&diag);
  for (int i=0;i<m_alines.size();i++)
      m_alines[i]->save(doc,&diag);
  for (int i=0;i<m_arrow_segments.size();i++)
      m_arrow_segments[i]->save(doc,&diag);
  for (int i=0;i<m_arrow_points.size();i++)
      m_arrow_points[i]->save(doc,&diag);
  sete->appendChild(diag);
}
*/
void handleEmptyBlock(QDomDocument * doc,QDomElement * diag,int id)
{
  QDomElement block=doc->createElement("block");
  block.setAttribute("this",ptrToString(NULL));
  block.setAttribute("id",intToString(id));
  diag->appendChild(block);
}

/*
void Box::save(QDomDocument * doc,QDomElement * diagram)
{
 QDomElement block=doc->createElement("block");

 block.setAttribute("this",ptrToString(this));
 block.setAttribute("real_string",stringToStringXML(m_real_text));
 block.setAttribute("viewed_string",stringToStringXML(m_viewtext));
 block.setAttribute("string_pos",rectToString(m_string_pos));
 block.setAttribute("number_pos",pointfToString(m_number_pos));
 block.setAttribute("rect",rectfToString(m_rect));
 block.setAttribute("id",intToString(m_id));
 block.setAttribute("childid",intToString(m_childdiagram));

 for (int i=0;i<BLOCK_SIDES;i++)
 {
     for (int j=0;j<MAX_LINE_REFERENCES;j++)
     {
        QDomElement lineref=doc->createElement("ptr");
        lineref.setAttribute("value",ptrToString(m_line_refs[i][j]));
        block.appendChild(lineref);
     }
 }
 diagram->appendChild(block);
}
*/
/*
void FreeComment::save(QDomDocument * doc,QDomElement * diagram)
{
 QDomElement alabel=doc->createElement("annotation_label");
 alabel.setAttribute("string",stringToStringXML(m_text));
 alabel.setAttribute("rect",rectfToString(m_rect));
 diagram->appendChild(alabel);
}
*/
/*
void CommentLine::save(QDomDocument * doc,QDomElement * diagram)
{
 QDomElement  aline=doc->createElement("annotation_line");
 aline.setAttribute("this",ptrToString(this));
 aline.setAttribute("free",pointfToString(m_freepoint));
 if (m_isbindedtoline)
 {
     aline.setAttribute("binded","true");
     aline.setAttribute("binded_ptr",ptrToString(m_bindedpoint));
 }
 else
 {
     aline.setAttribute("binded","false");
     aline.setAttribute("coords",pointfToString(*m_bindedpoint));
 }
 diagram->appendChild(aline);
}
*/
void ArrowSegment::save(QDomDocument * doc,QDomElement * diagram)
{
 QDomElement  aseg=doc->createElement("segment");
 aseg.setAttribute("this",ptrToString(this));
 aseg.setAttribute("in",ptrToString(m_in));
 aseg.setAttribute("out",ptrToString(m_out));
 diagram->appendChild(aseg);
}

void ArrowPoint::save(QDomDocument * doc,QDomElement * diagram)
{
 QDomElement apoint=doc->createElement("point");
 apoint.setAttribute("this",ptrToString(this));
 apoint.setAttribute("block",ptrToString(m_block));
 apoint.setAttribute("x",doubleToString(x()));
 apoint.setAttribute("y",doubleToString(y()));
 for (int i=0;i<m_in.size();i++)
 {
     QDomElement in=doc->createElement("in");
     in.setAttribute("ptr",ptrToString(m_in[i]));
     apoint.appendChild(in);
 }
 for (int i=0;i<m_out.size();i++)
 {
     QDomElement out=doc->createElement("out");
     out.setAttribute("ptr",ptrToString(m_out[i]));
     apoint.appendChild(out);
 }
 for (int i=0;i<m_lines.size();i++)
 {
     QDomElement line=doc->createElement("line");
     line.setAttribute("ptr",ptrToString(m_lines[i]));
     apoint.appendChild(line);
 }
 diagram->appendChild(apoint);
}
