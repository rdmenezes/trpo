#include "diagram.h"
#include "boxitem.h"
#include "collision.h"
Diagram::Diagram()
{
    m_parent=ParentLocation(-1,-1);
    nullifyBoxes();
}

Diagram::Diagram(const ParentLocation&
                loc)
{
    m_parent=loc;
    nullifyBoxes();
}

void Diagram::nullifyBoxes()
{
  for(int i=0;i<DIAGRAM_MAX_BLOCKS;i++)
  {
    m_boxes[i]=NULL;
  }
}

bool Diagram::isEmpty() const
{
  for (int i=0;i<DIAGRAM_MAX_BLOCKS;i++)
  {
      if (m_boxes[i]) return false;
  }
  return true;
}

bool Diagram::canAddBoxes() const
{
 for (int i=0;i<DIAGRAM_MAX_BLOCKS;i++)
 {
     if (!m_boxes[i]) return true;
 }
 return false;
}

int Diagram::getBoxID() const
{
 for (int i=0;i<DIAGRAM_MAX_BLOCKS;i++)
 {
     if (!m_boxes[i]) return i;
 }
 return false;
}

void Diagram::addBox(BoxItem * box)
{
  this->m_boxes[box->id()]=box;
}

bool Diagram::canBePlaced(const QRectF & rect, BoxItem * pointer)
{
    //Scan blocks
    for (int i=0;i<DIAGRAM_MAX_BLOCKS;i++)
    {
      if (m_boxes[i]!=NULL && m_boxes[i]!=pointer)
      {
          if (collides(rect,m_boxes[i]->boundingRect()))
              return false;
      }
    }
    return true;
}
