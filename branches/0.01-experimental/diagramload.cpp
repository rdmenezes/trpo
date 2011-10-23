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
}
