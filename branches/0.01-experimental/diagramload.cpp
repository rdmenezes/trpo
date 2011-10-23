#include "diagramset.h"
#include "diagram.h"
#include "alabelitem.h"
#include "alineitem.h"
#include "arrowpoint.h"
#include "arrowsegment.h"
#include "boxitem.h"
#include "saveloadutils.h"
#include <QDomDocument>
#include <QDomElement>
#include <QDomAttr>

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
           m_set[diagram]->load(&diagel);
           ++diagram;
       }
   }
   n=n.nextSibling();
 }
}

void Diagram::load(QDomElement * e)
{

}
