/*! \file   alabelitem.h
    \author Mamontov

    Describes an annotation label item
*/
#ifndef ALABELITEM_H
#define ALABELITEM_H

#include <QGraphicsItem>
#include <QRectF>
#include <QRect>

//Describes an annotation label item
#define DEFAULT_ALABEL_TEXT "Some commentary"

/*! \class ALabelItem

    Describes an annotation label item
 */
class ALabelItem : public QGraphicsItem
{
private:
    /*! Default string data
     */
    QString m_string;
    /*! Rect
     */
    QRectF   m_rect;
public:
    /*! Declares a type of item
     */
    enum ItemType
    {
        USERTYPE=QGraphicsItem::UserType+10
    };


    ALabelItem();
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
signals:

public slots:

};

#endif // ALABELITEM_H
