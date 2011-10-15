/*! \file boxitem.h
    \author HiddenSeeker

    Declares a box graphics item, that represents block
 */
#ifndef BOXITEM_H
#define BOXITEM_H

#include <QGraphicsItem>
#include <QRectF>
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
        QPointF  m_string_pos;
        /*! Number position
         */
        QPointF  m_number_pos;
        /*! Rect
         */
        QRectF   m_rect;
        /*! Box id
         */
        char     m_id;
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
    inline void setID(int id) { m_id=id; }
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
