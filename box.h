/*! \file boxitem.h
    \author Mamonto

    Declares a box graphics item
 */
#ifndef BOXITEM_H
#define BOXITEM_H

#include <QRectF>
#include <QRect>
#include <QSize>
#include "diagramobject.h"
#include "diagram.h"
#include "moverange.h"
#include "drawingconstants.h"
#include "objectconnector.h"
#include "direction.h"
//Class of scene
class DiagramScene;
//Arrow points
class ArrowPoint;
//Dom element
class QDomElement;
//Dom document
class QDomDocument;
//Loading data
class DiagramLoadData;

/*! \class BoxSide
    Declares a box item side
 */
enum BoxSide
{
    BIS_LEFT=0,
    BIS_RIGHT=1,
    BIS_TOP=2,
    BIS_BOTTOM=3
};
//Type of arrow, that enters inside the block
enum BlockEnteringType
{
 BET_INPUT,   //! Point inputs into block
 BET_OUTPUT   //! Point emerges from a block
};
//Maximum references to a side
#define MAX_LINE_REFERENCES 3
//Block sides (total)
#define BLOCK_SIDES 4


/*! Returns a side of rectangle, where the point is located
    \param[in]   r rectangle
    \param[in]   p point
 */
Direction getSide(const QRectF & r, const QPointF & p);

/*!   Computes a line of side of block
      \param[in] r rectangle
      \param[in] dir  side of block
 */
QLineF    getLineBySide(const QRectF & r, Direction dir);

/*! \class Box

    Declares a box item class
 */
class Box : public DiagramObject
{
private:
        /*! Real text string
         */
        QString  m_real_text;
        /*! Viewed text string
         */
        QString  m_view_text;
        /*!  m_view_text position on a screen
         */
        QRectF   m_text_rect;
        /*! rendered number position
         */
        QRectF   m_number_rect;
        /*! Size of bounding rect of box
         */
        QSize    m_size;
        /*! Determines, whether number is visible
         */
        bool     m_number_is_visible;
        /*! Box id
         */
        int      m_id;
        /*! Box connector vector
         */
        QVector<ObjectConnector *> m_connectors;
        /*! Regenerates a text positions and parameters
         */
        void regenerate();
        /*! Creates an input connectors for a box
         */
        void createConnectors();
protected:
        /*! Paints a box
         */
        void paint(QPainter * p);
private:

        /*! Location of child diagram (-1 is default)
         */
        int m_childdiagram;
        /*! Propagates a label editing
            \param[in] event event editing
         */
        void addObjectTextEditor(QKeyEvent * event);
        /*! Declares, whether point can be added to side
         */
        bool canAddToSide(BoxSide side);
public:
    /*! Constructs empty box.
        Used by serializable factory.
     */
    inline Box() : DiagramObject(ST_RECTANGLE) {}
    /*! Creates a box from a specified position
        \param[in] p   position of center of box
        \param[in] d   diagram
        \param[in] txt text data
     */
    Box(const QPointF & p, Diagram * d, const QString & txt=BOX_DEFAULT_TEXT);
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
    /*! Sets a new rectangle for box
        \param[in] rect new rect
     */
    virtual  void setRect(const QRectF & rect);
    /*! Sets a text for box
        \param[in] text new text data
     */
    void setText(const QString & text);
    /*! Returns a type of box item
     */
    int type() const;
    /*! Returns a rectangle for collision testing
     */
    virtual QRectF  collisionRect() const;
    /*! Clones a diagram object
     */
    virtual DiagramObject * clone();
    /*! Sets a number of box
        \param[in] id id
    */
    inline void setNumber(int id) { m_id=id; regenerate(); update(); }
    /*! Returns an editable text for editor
     */
    QString  getEditableText() const;
    /*! Sets box position
        \param[in] x x position
        \param[in] y y position
     */
    void setPos(qreal x, qreal y);
    /*! Sets box position
        \param[in] pos position
     */
    void setPos(const QPointF & pos);
    /*! Returns a bounding rectangle of item
     */
    virtual QRectF boundingRect() const;
    /*! Returns item id
    */
    inline char id() const { return m_id; }
    /*! Returns a connector by direction
        \param[in] d direction
        \return connector
     */
    ObjectConnector * getBySide(Direction d);
    /*! Detects, whether we should draw an arrow
     */
    virtual bool drawTrianglePart() const;
    /*! Destructor
     */
    virtual ~Box();


    /*! Creates an item position at scene
        \param[in] pos    position of creation of box
        \param[in] scene  scene data
    */
    Box(const QPointF & pos,DiagramScene * scene);

    /*! Updates an item string
     */
    void updateString(const QString & text);

    /*! Returns a child diagram
        \return id
     */
    inline int childDiagram() const { return m_childdiagram;}
    /*! Sets a childdiagram
        \param[in]  id diagram id
     */
    inline void setChildDiagram(int id) { m_childdiagram=id; }
    /*! Determines, thich size of box this point
        is belong to.
        \param[in] point point
        \return side
     */
    static BoxSide sideOfPoint(ArrowPoint * point, const QRectF & m_rect);
    /*! Determines, thich size of box this point
        is belong to.
        \param[in] point point
        \return side
     */
    BoxSide sideOfPoint(ArrowPoint * point);
    /*! Attaches all points from vector
        \param[in] pts, a vector, whose points are attached
     */
    void attachAllPoints(const QVector<ArrowPoint *> pts);
    /*! Returns a range for a point
        \param[in] point point
     */
    MoveRange getRange(ArrowPoint * point);
};

#endif // BOXITEM_H
