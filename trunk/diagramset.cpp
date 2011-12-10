#include "diagramset.h"

DiagramSet::DiagramSet()
{
    m_max_id=0;
}

Diagram * DiagramSet::create(
                             const DiagramParent & l
                            )
{
  Diagram * n=new Diagram(l);
  m_set.insert(m_max_id++,n);
  return  n;
}


Diagram * DiagramSet::get(int id)
{
 if ( ! m_set.contains(id) ) return NULL;
 return m_set[id];
}


bool DiagramSet::empty() const
{
    bool fempty=true;
    for(QHash<int,Diagram *>::const_iterator it=m_set.begin();
        it!=m_set.end();
        ++it
       )
       fempty= fempty && it.value()->empty();
    return fempty;
}

int  DiagramSet::find(const Diagram * d) const
{
  for(QHash<int,Diagram *>::const_iterator it=m_set.begin();
      it!=m_set.end();
      ++it
     )
  {
      bool cmp=it.value()==d;
      if (cmp)
          return it.key();
  }
  return -1;
}

void DiagramSet::remove(int id)
{
  if (m_set.contains(id))
  {
    Diagram * d=m_set[id];
    delete  d;
    m_set.remove(id);
  }
}


void DiagramSet::save(QDomDocument * doc,
                      QDomElement *  element)
{
     QDomElement diagramset;
     diagramset=doc->createElement("diagram_set");

     QHash<int,Diagram *>::iterator i = m_set.begin();

     pushThis(diagramset);
     diagramset.setAttribute("max_id", ::save(m_max_id));
     diagramset.setAttribute("hash",::save(m_set));

     while (i!=m_set.end())
     {
         i.value()->save(doc,&diagramset);
         i++;
     }
     element->appendChild(diagramset);
}


void DiagramSet::load(QDomElement * element,
                      QMap<void *, Serializable *> &  addressMap)
{
    QDomNamedNodeMap attributes=element->attributes();
    if (attributes.contains("this"))
    {
        addressMap.insert(::load<void*>(getValue(attributes,"this")),
                          this);
    }
    if (attributes.contains("max_id"))
    {
        m_max_id=::load<int>(getValue(attributes,"max_id"));
    }
    if (attributes.contains("hash"))
    {
        m_set=::load< QHash<int,Diagram *> >(getValue(attributes,"hash"));
    }
    //Proceed loading diagrams
    QDomNode diagram=element->firstChild();
    while(! (diagram.isNull()))
    {
        if (diagram.isElement())
        {
            QDomElement el=diagram.toElement();
            if (el.tagName()=="diagram")
            {
                Diagram * diag=new Diagram();
                diag->load(&el,addressMap);
            }
        }
        diagram=diagram.nextSibling();
    }
    //Resolve all pointers
    resolvePointers(addressMap);
}

void DiagramSet::resolvePointers(QMap<void *, Serializable *> &
                                  adressMap )
{
    QHash<int,Diagram*>::iterator it=m_set.begin();
    for (;it!=m_set.end();it++)
    {
        Diagram * diag= static_cast<Diagram*>(adressMap[it.value()]);
        it.value()=diag;
        diag->resolvePointers(adressMap);
    }
}

DiagramSet::~DiagramSet()
{
 for(QHash<int,Diagram *>::iterator
        it=m_set.begin();
        it!=m_set.end();
        ++it
        )
 {
   delete it.value();
 }
}

