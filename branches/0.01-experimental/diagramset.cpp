#include "diagramset.h"

DiagramSet::DiagramSet()
{
    m_max_id=1;
    m_set.insert(0,new Diagram());
    m_set[0]->setID(0);
}

int DiagramSet::create(
       const ParentLocation & loc
                      )
{
  m_set.insert(m_max_id,
               new Diagram(loc));
  m_set[m_max_id]->setID(m_max_id);
  return m_max_id++;
}


Diagram * DiagramSet::get(int id)
{
 Q_ASSERT( m_set.contains(id) );
 return m_set[id];
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

