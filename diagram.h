/*! \file diagram.h
    \author Mamontov

    Declares an abstract diagram representation in memory
 */
#include <QPair>
#include <QVector>
#include <time.h>
#ifndef DIAGRAM_H
#define DIAGRAM_H
//Box item
class BoxItem;
/*! Declares a location of parent block
 */
class ParentLocation: public QPair<int,int>
{
  public:
           /*! Default constructor
            */
           inline ParentLocation() {}
           /*! Creates  a new location
                \param[in] diag diagram
                \param[in] block block
            */
           inline ParentLocation(int diag,int block)
           {
               first=diag;
               second=block;
           }
           /*! Copy constructor
               \param[in] o other
           */
           inline ParentLocation(const ParentLocation & o)
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
/*! Interval, between history clearing
 */
#define HISTORY_CLEAR_INTERVAL 5
/*! Class, that handles changes of numbers in blocks, used to undo
    needless swaps, when changing number
*/
class NumberChangeHistory
{
 public:
    /*! Represents change of number of item to number
    */
    typedef QPair<BoxItem *, char> ChangeEntry;
    /*! Represents swapping iterm numbers as two changes
    */
    typedef QPair<ChangeEntry,ChangeEntry> SwapEntry;
 private:
    /*! History is a vector of changes
     */
    QVector<SwapEntry> m_history;
 public:
    /*! Undos a swapping if swapped was before HISTORY_CLEAR_INTERVAL reached
        \param[in] item      item that value is changing now
        \param[in] old_value value, that is changed
     */
    void undoIfSwapped(BoxItem * item,char old_value);
    /*! Adds new swap in history
        \param[in] one first swap
        \param[in] two second swap
     */
    void addNewSwap(const SwapEntry & one, const SwapEntry & two);
};
/*! Defines a max blocks in diagram
 */
#define DIAGRAM_MAX_BLOCKS 7
/*! Declares a diagram blocks
 */
class Diagram
{
private:
        /*! Describes a parent data location
         */
        ParentLocation          m_parent;
        /*! An amount of boxes. If NULL - the block is absent
         */
        BoxItem          *      m_boxes[DIAGRAM_MAX_BLOCKS];
        /*! History data
         */
        NumberChangeHistory     m_history;
        /*! An id of diagram
         */
        int m_id;
        /*! Sets a boxes to NULL
         */
        void nullifyBoxes();
public:
        /*! Inits an empty diagram
         */
        Diagram();
        /*! Inits an empty diagram with an location
            \param[in] loc
         */
        Diagram(const ParentLocation & loc);
        /*! Determines, whether diagram is empty
         */
        bool isEmpty() const;
        /*! Determines, whether we can add a new box
         */
        bool canAddBoxes() const;
        /*! Adds a new Box
            \param[in] box amount of boxes
         */
        void addBox(BoxItem * box);
        /*! Gets an index id for box
         */
        int getBoxID() const;
        /*! Returns a diagram id
            \return diagram id
         */
        inline int getID() const { return m_id; }
        /*! Sets a diagram id
            \param[in] id diargam id
         */
        inline void setID(int id) { m_id=id;}
};

#endif // DIAGRAM_H
