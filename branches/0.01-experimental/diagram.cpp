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

BoxItem * Diagram::getBlockByID(int id)
{
   return m_boxes[id];
}

NumberChangeHistory::NumberChangeHistory()
{
    this->m_time=clock();
}
//Old value is reserved
void NumberChangeHistory::undoIfSwapped(BoxItem * item,char old_value,Diagram * diag)
{
 (void)old_value;
 if (clock()-m_time>HISTORY_CLEAR_INTERVAL*CLOCKS_PER_SEC || m_history.size()==0)
 {
     this->m_history.clear();
 }
 else
 {
     BoxItem * block1=m_history[0].first.first;
     BoxItem * block2=m_history[0].second.first;
     if (block1==item || block2==item )
     {
        diag->setBlockID(block1,m_history[0].second.second);
        diag->setBlockID(block2,m_history[0].first.second);
     }
     m_history.remove(0);
 }
}

void Diagram::undoIfSwapped(BoxItem * item,char old_value)
{
  this->m_history.undoIfSwapped(item,old_value,this);
}

void NumberChangeHistory::addNewSwap(const ChangeEntry & one,const  ChangeEntry & two)
{
  SwapEntry ent;
  ent.first=one;
  ent.second=two;
  this->m_history<<ent;
}

void Diagram::addNewSwap(BoxItem * item1, char new1,BoxItem * item2,char new2)
{
    NumberChangeHistory::ChangeEntry s1;
    s1.first=item1;
    s1.second=new1;
    NumberChangeHistory::ChangeEntry s2;
    s2.first=item2;
    s2.second=new2;
    this->m_history.addNewSwap(s1,s2);
    setBlockID(item1,new1);
    setBlockID(item2,new2);
}

void Diagram::setBlockID(BoxItem * item, char pos)
{
    for(int i=0;i<DIAGRAM_MAX_BLOCKS;i++)
        if (m_boxes[i]==item)
              m_boxes[i]=NULL;
    m_boxes[pos]=item;
    item->setID(pos);
}

