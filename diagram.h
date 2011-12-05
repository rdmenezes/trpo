/*! \file diagram.h
    \author Mamontov

    Declares an abstract diagram representation in memory
 */
#include <QPair>
#include <QVector>
#include <QRectF>
#include <time.h>
#include "diagramobject.h"
#include "box.h"

#ifndef DIAGRAM_H
#define DIAGRAM_H
//Graphics item
class QGraphicsItem;
//Box item
class Box;
//Annotation label item
class FreeComment;
//Annoation lint item
class CommentLine;
//Arrow points item
class ArrowPoint;
//Arrow segments item
class ArrowSegment;
//A set of diagram
class DiagramSet;
//Diagram sce
class DiagramScene;
//Dom element
class QDomElement;
//Dom document
class QDomDocument;

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

/*! History clear time
 */
#define HISTORY_CLEAR_TIME 1


/*! Declares a diagram blocks
 */
class Diagram : public Serializable
{
private:
        /*! Set of objects
         */
        QVector<DiagramObject *> m_objects;
        /*! A diagram set, where it's belong to
        */
        DiagramSet      *       m_set;
        /*! Describes a parent data location
         */
        DiagramParent          m_parent;
        /*! Scene, which diagram is belongs to
         */
        DiagramScene    *      m_scene;
        /*! Box data info
         */
        QMap<Box *, int>       m_boxes;
        /*! Swap entries data
         */
        QVector<SwapEntry>     m_swaps;

        /*! Vector  of annotation labels
         */
        QVector<FreeComment *>   m_alabels;
        /*! Vector of annotation lines
         */
        QVector<CommentLine *>    m_alines;
        /*! Vector of arrow points
         */
        QVector<ArrowPoint *>   m_arrow_points;
        /*! Vector if arrow segments
         */
        QVector<ArrowSegment *> m_arrow_segments;
        /*! An id of diagram
         */
        int m_id;
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
        /*! Returns a parent location
         */
        inline const DiagramParent & parent() const { return m_parent; }
        /*! Sets a parent location
            \param[in] parent parent location
         */
        inline void setParent(const DiagramParent & parent) { m_parent=parent;}
        /*! Returns a scene
            \param[in] scene scen data
         */
        inline DiagramScene * scene()  { return m_scene; }
        /*! Returns scene identifier
         */
        int id() const;
        /*! Binds an diagram to data
            \param[in] scene scene data
            \param[in] set   set data
         */
        inline void bind(DiagramScene * scene,DiagramSet * set)
        {
          m_scene=scene;
          m_set=set;
        }
        /*! Returns a number for a box.
            \param[in] b box
            \return box id or non-existing id, if not found
         */
        int getBoxNumber(DiagramObject * b);
        /*!  Saves a data to document
             \param[in] doc     document data
             \param[in] element parent element data
         */
        virtual  void save(QDomDocument * doc,QDomElement * element);
        /*! Loads a default data from document populating address map
            \param[in] element element data
            \param[in] addressMap adressedMap
         */
        virtual  void load(QDomElement * element,
                           QMap<void *, Serializable *> & addressMap);
        /*! Resolves inner pointers, from stored in adress map
            \param[in] addressMap map of addresses
         */
        virtual  void resolvePointers(QMap<void *, Serializable *> & adressMap);
        /*! Scans an objects for collisions
            \param[in] obj       adding/placing object
            \param[in] exctypes  excluded from testing types
            \param[in] excobjs   excluded from testing objects
            \return true, if can place
         */
        bool canPlace(DiagramObject * obj,
                      const QVector<int> & exctypes=QVector<int>(),
                      const QVector<CollisionObject *> & excobjs=QVector<CollisionObject *>()
                     );

        /*! Adds new object to diagram
         */
        void add(DiagramObject * obj);
        /*! Clears a diagram
         */
        void clear();
        /*! Changes block number by a delta parameter
            \param[in] delta delta number
            \param[in] b     box
         */
        void changeBlockNumber(int delta, Box * b);
        /*! Get types of items under position
         */
        void items(const QPointF & pos, QList<QGraphicsItem *>  & lst);

        /*! Returns an annotation labels
         */
        inline QVector<FreeComment *> & annotationLabels() { return  m_alabels; }
        /*! Returns an annotatione lines
         */
        inline QVector<CommentLine *> &  annotationLines() { return m_alines; }
        /*! Returns an arrow points
         */
        inline QVector<ArrowPoint *> &  arrowPoints()    { return  m_arrow_points; }
        /*! Returns an arrow segments
         */
        inline QVector<ArrowSegment *> & arrowSegments() { return m_arrow_segments; }
        /*! Inits an empty diagram
         */
        Diagram();
        /*! Inits an empty diagram with an location
            \param[in] l  parent
         */
        Diagram(const DiagramParent & l);
        /*! Determines, whether diagram is empty
         */
        bool empty() const;
        /*! Determines, whether we can add a new box
         */
        bool canAddBoxes() const;
        /*! Adds a new Box
            \param[in] box box to be added
         */
        void addBox(Box * box);
        /*! Adds a new Annotation Label
            \param[in] label annotation label  to be added
         */
        void addAnnotationLabel(FreeComment * label);
        /*! Determines, whether block can be placed or not
            \param[in] rect   bounding rect geometry
            \param[in] pointer NULL if don't check
         */
        bool canBePlaced(const QRectF & rect, Box * pointer=NULL);
        /*! Determines, whether block can be placed or not
            \param[in] rect   bounding rect geometry
            \param[in] pointer NULL if don't check
         */
        bool canBePlaced(const QRectF & rect, FreeComment * pointer=NULL);
        /*! Determines, whether arrow can be placed ot not
            \param[in] point1  first point
            \param[in] point2  second point
            \param[in] block   block, which will colliding with
            \param[in] always_check_bounds determines, whether bounds needs to be checked
         */
        bool canPlaceSegment(ArrowPoint * point1, ArrowPoint * point2,
                             Box * block=NULL,
                             bool always_check_bounds=false );
        /*! Determines, whether annotation line can be placed ot not
            \param[in] point1  first point
            \param[in] point2  second point
         */
        bool canPlaceAnnotationLine(const QPointF * point1,const QPointF * point);

        /*! Gets an index id for box
         */
        int getBoxID() const;
        /*! Returns a block by id
            \param[in] id block id
         */
        Box * getBlockByID(int id);
        /*! Sets a blocks id
         */
        void setBlockID(Box * item, char pos);
        /*! Removes a block with id
            \param[in] id id of removing block
         */
        inline void removeBlock(int id) { /*m_boxes[id]=NULL;*/ }
        /*! Removes an annotation label
            \param[in] label label item
         */
        void removeAnnotationLabel(FreeComment * label);
        /*! Adds a new annotation line
            \param[in] line annotation line
         */
        void addAnnotationLine(CommentLine * line);
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
        void removeAnnotationLine(CommentLine * line);
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
        /*! Saves a diagram
            \param[in] doc document
            \param[in] sete set
         */
        //void save(QDomDocument * doc,QDomElement * sete);
        /*! Loads a diagram
         */
        //void load(QDomElement * diag,DiagramSet * parent);
        /*! Destructor
         */
        ~Diagram();
};


//Defines, how rectangle is enlarged
#define QRECTF_ENLARGE_RANGE 2
QRectF enlarge(const QRectF & rect);

#endif // DIAGRAM_H
