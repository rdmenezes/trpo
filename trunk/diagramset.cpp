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


void DiagramSet::save(QDomDocument * /* doc */,
                      QDomElement *  /* element */)
{
    //!< TODO: Implement this later
}

void DiagramSet::load(QDomElement * /* element */,
                      QMap<void *, Serializable *> & /* addressMap */ )
{
    //!< TODO: Implement this later
}

void DiagramSet::resolvePointers(QMap<void *, Serializable *> &
                                 /* adressMap */)
{
    //!< TODO: Implement this later
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
