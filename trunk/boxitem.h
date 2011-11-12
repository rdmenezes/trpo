/*! \file boxitem.h
    \author HiddenSeeker

    Declares a box graphics item, that represents block
 */
#ifndef BOXITEM_H
#define BOXITEM_H

#include <QGraphicsItem>
#include <QRectF>
#include <QRect>
#include "moverange.h"
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

/*! \class BoxItemSide
    Declares a box item side
 */
enum BoxItemSide
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
//Default block text
#define DEFAULT_BLOCK_TEXT "Some action"

/*! \class BoxItem

    Declares a box item class
 */
class BoxItem : public QGraphicsItem
{
private:
        /*! Real text string
         */
        QString  m_real_string;
        /*! Viewed text string
         */
        QString  m_viewed_string;
        /*! rendered string position
         */
        QRect  m_string_pos;
        /*! Number position
         */
        QPointF  m_number_pos;
        /*! Rect
         */
        QRectF   m_rect;
        /*! Box id
         */
        char     m_id;
        /*! Location of child diagram (-1 is default)
         */
        int m_childdiagram;
        /*! Referenced points to a box
         */
        ArrowPoint * m_line_refs[BLOCK_SIDES][MAX_LINE_REFERENCES] ;
        /*! Propagates a label editing
            \param[in] event event editing
         */
        void addLabelEdit(QKeyEvent * event);
        /*! Declares, whether point can be added to side
         */
        bool canAddToSide(BoxItemSide side);
public:
          /*! Describes a key press event
           */
          void keyPressEvent(QKeyEvent *event);
public:
    //Functions for loading and saving
    QString & accessRealString()    { return m_real_string; }
    QString & accessViewedString() { return m_viewed_string; }
    QRect &   accessStringPos()     { return m_string_pos;  }
    QPointF & accessNumberPos()     { return  m_number_pos; }
    QRectF &  accessRect()          { return m_rect; }
    ArrowPoint *& getLineRef(int side,int pos)  { return m_line_refs[side][pos]; }
    inline BoxItem() {}
    /*! Declares a type of item
     */
    enum ItemType
    {
        USERTYPE=QGraphicsItem::UserType+1
    };
    /*! Regenerates a text positions and parameters
     */
    void regenerate();
    /*! Returns item id
    */
    inline char id() const { return m_id; }
    /*! Sets an item id
        \param[in] id id
    */
    inline void setID(int id) { m_id=id; update(); }
    /*! Creates an item position at scene
        \param[in] pos    position of creation of box
        \param[in] scene  scene data
    */
    BoxItem(const QPointF & pos,DiagramScene * scene);
    /*! Returns a type of box item
     */
    int type() const;
    /*! Returns a bounding rectangle of item
     */
    QRectF boundingRect() const;
    /*! Gives access to rect for changing
     */
    inline QRectF & frameRect() { return m_rect; }
    /*! Sets a new rectangle
     */
    void setRect(const QRectF & rect);
    /*! Updates an item string
     */
    void updateString(const QString & text);
    /*! Paint event reaction
        \param[in] painter painter drawing
        \param[in] option options
        \param[in] widget widget data
     */
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    /*! Returns a child diagram
        \return id
     */
    inline int childDiagram() const { return m_childdiagram;}
    /*! Sets a childdiagram
        \param[in]  id diagram id
     */
    inline void setChildDiagram(int id) { m_childdiagram=id; }
    /*! Clears all references
     */
    void clearPointReferences();
    /*! Removes a point reference from item
        \param[in] point
     */
    void removePointReference(ArrowPoint * point);
    /*! Adds a point reference to a block
        \param[in] point point to be added
     */
    void addPointReference(ArrowPoint * point);
    /*! Determines, thich size of box this point
        is belong to.
        \param[in] point point
        \return side
     */
    static BoxItemSide sideOfPoint(ArrowPoint * point, const QRectF & m_rect);
    /*! Determines, thich size of box this point
        is belong to.
        \param[in] point point
        \return side
     */
    BoxItemSide sideOfPoint(ArrowPoint * point);
    /*! Tests, whether it can add a point reference
        \param[in] point point data
        \param[in] enter entering type of point
        \return true, if can
     */
    bool canAddPointReference(ArrowPoint * point,
                              BlockEnteringType enter);
    /*! Attaches all points from vector
        \param[in] pts, a vector, whose points are attached
     */
    void attachAllPoints(const QVector<ArrowPoint *> pts);
    /*! Returns a range for a point
        \param[in] point point
     */
    MoveRange getRange(ArrowPoint * point);
    /*! Saves a block
        \param[in] doc document
        \param[in] diag set
     */
    void save(QDomDocument * doc,QDomElement * diagram);
};

#endif // BOXITEM_H
