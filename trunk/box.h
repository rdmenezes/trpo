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
//Dom element
class QDomElement;
//Dom document
class QDomDocument;

/*! \enum BoxCorner
    BoxCorner, used for resizing
 */
enum BlockCorner
{
    BC_UPPERLEFT,
    BC_UPPERRIGHT,
    BC_LOWERLEFT,
    BC_LOWERRIGHT,

    BC_CENTER //!< Special value for center click
};


QRectF resize(const QRectF & r, BlockCorner bc, const QPointF pos);


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
        /*! Location of child diagram (-1 is default)
         */
        int m_child;
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
    /*! Receives comment line event, when it changes it's direction to set it's
        input to correct position
        \param[in] obj  comment line
        \return new input position for it. By default, it's old position
     */
    virtual QPointF receiveCommentLineMove(DiagramObject * obj);
    /*! Kills self if equal to object
        \param[in] o object to compare
        \return true if will die
     */
    virtual bool dieIfEqualTo(DiagramObject * o);
    /*! Returns a child diagram
        \return id
     */
    inline int childDiagram() const { return m_child;}
    /*! Sets a childdiagram
        \param[in]  id diagram id
     */
    inline void setChildDiagram(int id) { m_child=id; }
    /*! Tests, whether can move box by vector
     */
    bool canMove(const QPointF & delta);
    /*! Moves by vector
        \param[in] p point
     */
    void moveBy(const QPointF & vector);
    /*! Tests, whether we can resize self
        \param[in] bc block corner
        \param[in] p position
     */
    bool canResize(BlockCorner bc, const QPointF & p);
    /*! Destructor
     */
    virtual ~Box();
};

#endif // BOXITEM_H
