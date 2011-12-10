#ifndef DIAGRAMDATA_H
#define DIAGRAMDATA_H
#include <QPair>
#include <time.h>

class Box;

/*! Declares a location of parent block
 */
class DiagramParent: public QPair<int,int>
{
  public:
           /*! Default constructor
            */
           inline DiagramParent() { first=-1; second=-1; }
           /*! Creates  a new location
                \param[in] diag diagram
                \param[in] block block
            */
           inline DiagramParent(int diag,int block)
           {
               first=diag;
               second=block;
           }
           /*! Copy constructor
               \param[in] o other
           */
           inline DiagramParent(const DiagramParent & o)
           {
             first=o.first;
             second=o.second;
           }
           /*! Returns a diagram id for parent
            */
           inline int diagramID() const { return first;}
           /*! Returns a block id for parent
            */
           inline int blockID()   const { return second;}
           /*! Sets a diagram id for parent
               \param[in]  id
            */
           inline void setDiagramID(int id)  { first=id;}
           /*! Sets a block id for parent
               \param[in]  id
            */
           inline void setBlockID(int id)  { second=id;}
};

/*! \class SwapEntry
    An entry of swapping box numbers
 */
class SwapEntry
{
 public:
        clock_t m_time; //!< Time
        Box  *  m_box1; //!< First box
        Box  *  m_box2; //!< Second box data
        //! Default constructor
        inline SwapEntry() {}
        /*! Constructs a swap entry
            \param[in] time time entry
            \param[in] b1   box one
            \param[in] b2   box two
         */
        inline SwapEntry(clock_t time, Box * b1, Box * b2)
        { m_time=time; m_box1=b1; m_box2=b2;  }
};

#endif // DIAGRAMDATA_H
