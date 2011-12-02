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
#include "drawingconstants.h"
#include "diagramobject.h"
//Describes an annotation label item
#define DEFAULT_ALABEL_TEXT "Some commentary"


class AttachedComment;

/*! \class FreeComment

    Describes an annotation label item
 */
class FreeComment : public DiagramObject
{
private:
    /*! Default string data
     */
    QString m_text;
    /*! Size of rect
     */
    QSizeF  m_size;
    /*! Attached comment
     */
    AttachedComment * m_parentcomment;
protected:
    /*! Paints a comment
        \param[in] p painter
     */
    void paint(QPainter * p);
    /*! Computes a rect for new text
     */
    QRectF computeRect(const QString & text);
public:
    /*! Parent commentary
        \return parent commentary
     */
    inline AttachedComment * parentComment() { return m_parentcomment; }
    /*! Sets a parent commentary
         \param[in] p parent comment
     */
    inline void setParentComment(AttachedComment * p) { m_parentcomment=p;}
    /*!  Default constructor, used by Serializable
     */
    inline FreeComment():DiagramObject(ST_RECTANGLE) { m_parentcomment=NULL; }
    /*!   Default constructor, used to create a point
          \param[in] p   middle point for comment creation
          \param[in] d   diagram
          \param[in] txt text
     */
    FreeComment(const QPointF & p, Diagram * d, const QString & txt=FC_TEXT);
    /*! Returns a bounding rectangle of item
        \return bounding rectangle
     */
    QRectF boundingRect() const;
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
    /*! Sets a text for an item
        \param[in] text text
     */
    void setText(const QString & text);
    /*! Moves a comment about to poin (tries)
        \param[in]  p   point
     */
    void moveTo(const QPointF & p);

    inline QString & string() { return m_text; }
    inline QRectF  & rect()   { return *(new QRectF());   }
    /*! Declares a type of item
     */
    enum ItemType
    {
        USERTYPE=QGraphicsItem::UserType+10
    };

    /*! Tries setting rect to label item
     */
    void setRect(const QRectF & rect);
    /*! Returns a type of annotation label item
     */
    int type() const;
    /*! Handles a text editing
        \param[in] event event of editing
     */
    void keyPressEvent(QKeyEvent *event);
    /*! Tries setting a  new text
        \param[in] text new text
     */
    void trySetText(const QString & text);

signals:

public slots:

};

#endif // ALABELITEM_H
