/*! \file diagram.h
    \author Mamontov

    Declares an abstract diagram representation in memory
 */
#include <QPair>
#include <QVector>
#include <QRectF>
#include <QImage>
#include <time.h>
#include "diagramobject.h"
#include <QDomDocument>
#include <QDomElement>
#include "box.h"



#ifndef DIAGRAM_H
#define DIAGRAM_H
//Box item
class Box;
//Annotation label item
class FreeComment;
//Annoation lint item
class CommentLine;
//A set of diagram
class DiagramSet;
//Diagram sce
class DiagramScene;



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
        QHash<Box *, int>       m_boxes;
        /*! Swap entries data
         */
        QVector<SwapEntry>     m_swaps;
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
        /*! Removes an object from diagram
         */
        void remove(DiagramObject * obj);
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
        /*! Exports a diagram to an image
            \param[in] img image to export to
         */
        void exportTo(QImage & img);
        /*! Tests, whether diagram is correct (has at least one block)
         */
        bool isCorrect();
        /*! Remove box from numbers
            \param[in] box box to remove
         */
        inline void removeBoxFromNumbers(Box * box)
        { m_boxes.remove(box); }
        /*! Commits changes to a diagram, saving her state
         */
        void commit();
        /*! Rollbacks, changes ti a diagram, retrieving her state
         */
        void rollback();
        /*! Fills a scene with own items
            \param[in] scene scene to fill
         */
        void fillScene(DiagramScene * scene);
        /*! Destructor
         */
        ~Diagram();
};


//Defines, how rectangle is enlarged
#define QRECTF_ENLARGE_RANGE 2
QRectF enlarge(const QRectF & rect);

#endif // DIAGRAM_H
