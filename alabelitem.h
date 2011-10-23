/*! \file   alabelitem.h
    \author Mamontov

    Describes an annotation label item
*/
#ifndef ALABELITEM_H
#define ALABELITEM_H

#include <QGraphicsItem>
#include <QRectF>
#include <QRect>
#include <QKeyEvent>
//Describes an annotation label item
#define DEFAULT_ALABEL_TEXT "Some commentary"

//Dom element
class QDomElement;
//Dom document
class QDomDocument;
//Loading data
class DiagramLoadData;

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
    inline QString & string() { return m_string; }
    inline QRectF  & rect()   { return m_rect;   }
    /*! Declares a type of item
     */
    enum ItemType
    {
        USERTYPE=QGraphicsItem::UserType+10
    };
    //Does nothing
    inline ALabelItem() {}
    /*! Constrcts a label item
        \param[in] rect bounding rectangle
     */
    ALabelItem(const QRectF & rect);
    /*! Tries setting rect to label item
     */
    void setRect(const QRectF & rect);
    /*! Returns a type of annotation label item
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
    /*! Handles a text editing
        \param[in] event event of editing
     */
    void keyPressEvent(QKeyEvent *event);
    /*! Tries setting a  new text
        \param[in] text new text
     */
    void trySetText(const QString & text);
    /*! Saves a label
        \param[in] doc document
        \param[in] diag set
     */
    void save(QDomDocument * doc,QDomElement * diagram);
signals:

public slots:

};

#endif // ALABELITEM_H
