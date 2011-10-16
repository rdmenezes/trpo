/*! \file boxitem.h
    \author HiddenSeeker

    Declares a box graphics item, that represents block
 */
#ifndef BOXITEM_H
#define BOXITEM_H

#include <QGraphicsItem>
#include <QRectF>
#include <QRect>
//Class of scene
class DiagramScene;

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
        /*! Propagates a label editing
            \param[in] event event editing
         */
        void addLabelEdit(QKeyEvent * event);
public:
          /*! Describes a key press event
           */
          void keyPressEvent(QKeyEvent *event);
public:
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
};

#endif // BOXITEM_H
