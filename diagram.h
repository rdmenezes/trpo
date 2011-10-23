/*! \file diagram.h
    \author Mamontov

    Declares an abstract diagram representation in memory
 */
#include <QPair>
#include <QVector>
#include <QRectF>
#include <time.h>
#ifndef DIAGRAM_H
#define DIAGRAM_H
//Graphics item
class QGraphicsItem;
//Box item
class BoxItem;
//Annotation label item
class ALabelItem;
//Annoation lint item
class ALineItem;
//Arrow points item
class ArrowPoint;
//Arrow segments item
class ArrowSegment;
//A set of diagram
class DiagramSet;
//Dom element
class QDomElement;
//Dom document
class QDomDocument;

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
/*! Interval, between history clearing in seconds
 */
#define HISTORY_CLEAR_INTERVAL 2
class Diagram;
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
    /*! Declares a last time change
     */
    clock_t            m_time;
 public:
    /*! Constructs a history
     */
    NumberChangeHistory();
    /*! Undos a swapping if swapped was before HISTORY_CLEAR_INTERVAL reached
        \param[in] item      item that value is changing now
        \param[in] old_value value, that is changed
        \param[in] diag      diagram
     */
    void undoIfSwapped(BoxItem * item,char old_value,Diagram * diag);
    /*! Adds new swap in history
        \param[in] one first swap
        \param[in] two second swap
     */
    void addNewSwap(const ChangeEntry & one, const ChangeEntry & two);
    /*! Clears a history
     */
    void clear();
};
/*! Defines a max blocks in diagram
 */
#define DIAGRAM_MAX_BLOCKS 7
/*! Declares a diagram blocks
 */
class Diagram
{
private:
        /*! A diagram set, where it's belong to
        */
        DiagramSet      *       m_set;
        /*! Describes a parent data location
         */
        ParentLocation          m_parent;
        /*! An amount of boxes. If NULL - the block is absent
         */
        BoxItem          *      m_boxes[DIAGRAM_MAX_BLOCKS];
        /*! History data
         */
        NumberChangeHistory     m_history;
        /*! Vector  of annotation labels
         */
        QVector<ALabelItem *>   m_alabels;
        /*! Vector of annotation lines
         */
        QVector<ALineItem *>    m_alines;
        /*! Vector of arrow points
         */
        QVector<ArrowPoint *>   m_arrow_points;
        /*! Vector if arrow segments
         */
        QVector<ArrowSegment *> m_arrow_segments;
        /*! An id of diagram
         */
        int m_id;
        /*! Sets a boxes to NULL
         */
        void nullifyBoxes();
        /*! Determines, whether item, doesn't collide with blocks
         */
        bool testForNoBlockCollision(const QRectF & rect,QGraphicsItem * pointer);
        /*! Determines, whether item, doesn't collide with blocks
         */
        bool testForNoALabelCollision(const QRectF & rect,QGraphicsItem * pointer);
        /*! Determines, whether item can be placed
         */
        bool canBePlaced(const QRectF & rect, QGraphicsItem * pointer);
        /*! Determines, whether item can be placed, without collision with lines
         */
        bool doesntCollideWithLines(const QRectF & rect);
public:
        /*! Returns a total boxes
         */
        inline int getTotalBoxes() { return DIAGRAM_MAX_BLOCKS; }
        /*! Returns a parent location
         */
        inline const ParentLocation & location() const { return m_parent; }
        /*! Sets a parent location
         */
        inline void setParentLocation(const ParentLocation & loc) { m_parent=loc;}
        /*! Returns an annotation labels
         */
        inline QVector<ALabelItem *> & annotationLabels() { return  m_alabels; }
        /*! Returns an annotatione lines
         */
        inline QVector<ALineItem *> &  annotationLines() { return m_alines; }
        /*! Returns an arrow points
         */
        inline QVector<ArrowPoint *> &  arrowPoints()    { return  m_arrow_points; }
        /*! Returns an arrow segments
         */
        inline QVector<ArrowSegment *> & arrowSegments() { return m_arrow_segments; }
        /*! Returns a diagram set
         */
        inline DiagramSet * set() { return m_set;}
        /*! Sets a diagram set
         */
        inline void setDiagramSet(DiagramSet * set) { m_set=set; }
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
            \param[in] box box to be added
         */
        void addBox(BoxItem * box);
        /*! Adds a new Annotation Label
            \param[in] label annotation label  to be added
         */
        void addAnnotationLabel(ALabelItem * label);
        /*! Determines, whether block can be placed or not
            \param[in] rect   bounding rect geometry
            \param[in] pointer NULL if don't check
         */
        bool canBePlaced(const QRectF & rect, BoxItem * pointer=NULL);
        /*! Determines, whether block can be placed or not
            \param[in] rect   bounding rect geometry
            \param[in] pointer NULL if don't check
         */
        bool canBePlaced(const QRectF & rect, ALabelItem * pointer=NULL);
        /*! Determines, whether arrow can be placed ot not
            \param[in] point1  first point
            \param[in] point2  second point
            \param[in] block   block, which will colliding with
            \param[in] always_check_bounds determines, whether bounds needs to be checked
         */
        bool canPlaceSegment(ArrowPoint * point1, ArrowPoint * point2,
                             BoxItem * block=NULL,
                             bool always_check_bounds=false );
        /*! Determines, whether annotation line can be placed ot not
            \param[in] point1  first point
            \param[in] point2  second point
         */
        bool canPlaceAnnotationLine(const QPointF * point1,const QPointF * point);

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
        /*! Returns a block by id
            \param[in] id block id
         */
        BoxItem * getBlockByID(int id);
        /*! Undos a swapping if swapped was before HISTORY_CLEAR_INTERVAL reached
            \param[in] item      item that value is changing now
            \param[in] old_value value, that is changed
         */
        void undoIfSwapped(BoxItem * item,char old_value);
        /*! Adds new swap in history
            \param[in] item1 first item
            \param[in] new1  new value of first item
            \param[in] item2 second item
            \param[in] new2  new value of second item
         */
        void addNewSwap(BoxItem * item1, char new1,BoxItem * item2,char new2);
        /*! Sets a blocks id
         */
        void setBlockID(BoxItem * item, char pos);
        /*! Removes a block with id
            \param[in] id id of removing block
         */
        inline void removeBlock(int id) { m_boxes[id]=NULL; }
        /*! Removes an annotation label
            \param[in] label label item
         */
        void removeAnnotationLabel(ALabelItem * label);
        /*! Adds a new annotation line
            \param[in] line annotation line
         */
        void addAnnotationLine(ALineItem * line);
        /*! Adds a new arrow point
            \param[in]  point arrow point
         */
        void addArrowPoint(ArrowPoint * point);
        /*! Adds a new arrow segment
            \param[in]  segment arrow segment
         */
        void addArrowSegment(ArrowSegment * segment);
        /*! Removes an annotation line from diagram
            \param[in] line  annotationline
         */
        void removeAnnotationLine(ALineItem * line);
        /*! Removes an arrow point from diagram
            \param[in] point arrow point
         */
        void removeArrowPoint(ArrowPoint * point);
        /*! Removes an arrow segment from digram
            \param[in] segment arrow segment
         */
        void  removeArrowSegment(ArrowSegment * segment);
        /*! Returns a vector of near arrow points
            \param[in] rect rectangle
         */
        QVector<ArrowPoint *> getNearArrowPoints(const QRectF & rect);
        /*! Check whether block can be correctly placed around blocks
            \param[in] rect rectangle block
            \param[in] pts points
         */
        bool canBePlacedAroundPoints(const QRectF & rect, const QVector<ArrowPoint *> pts);
        /*! Tests, whether diagram is correct (has at least one block)
         */
        bool isCorrect();
        /*! Clears a diagram
         */
        void clear();
        /*! Saves a diagram
            \param[in] doc document
            \param[in] sete set
         */
        void save(QDomDocument * doc,QDomElement * sete);
        /*! Loads a diagram
         */
        void load(QDomElement * diag);
        /*! Destructor
         */
        ~Diagram();
};


//Defines, how rectangle is enlarged
#define QRECTF_ENLARGE_RANGE 2
QRectF enlarge(const QRectF & rect);

#endif // DIAGRAM_H
