/*! \file attachedcomment.h
    \author Mamontov

    Creates an attached comment
 */
#ifndef ATTACHEDCOMMENT_H
#define ATTACHEDCOMMENT_H
#include "freecomment.h"
#include "commentline.h"

/*! \class AttachedComment
    Attached comment
 */
class AttachedComment : public DiagramObject
{
private:
        FreeComment  * m_comment;  //!< Comment data
        CommentLine  * m_line;     //!< Line data
        int            m_iterator; //!< Iterator state
protected:
        /*! Paints an attached comment
         */
        void paint(QPainter *p);
public:
         /*! Default constructor for Serizalizable
          */
         inline AttachedComment() : DiagramObject(ST_COMPLEX) { setPos(0,0); }
         /*! Common constructor
             \param[in] comment comment data
             \param[in] line    line data
          */
         inline AttachedComment(FreeComment * comment, CommentLine * line)
             :  DiagramObject(ST_COMPLEX)
         {
             setDiagram(comment->diagram());
             m_comment=comment;
             m_line=line;
             setPos(0,0);
         }
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
         /*! Returns first object for collision
         */
         virtual CollisionObject *    firstObject();
         /*! Returns next object for collision
          */
         virtual CollisionObject *    nextObject();
         /*! Bounding rectangle
             \return bounding rectangle
          */
         QRectF boundingRect() const;
         /*! Sets a line for comment
             \param[in] l line
          */
         inline void setLine(CommentLine * line) {m_line=line;}
         /*! Returns a comment part
          */
         inline FreeComment * comment() const {return m_comment;}
         /*! Returns a line part
          */
         inline CommentLine *  line() const { return m_line; }
         /*! Returns NULL
          */
         DiagramObject * clone();
         /*! Used to perfom a custom actions, before dying
             \return true (default implementation) if just remove it from scene, false - if delete
          */
         virtual bool deleteOnRemoval();
         /*! Kills self if equal to object
             \param[in] o object to compare
             \return true if will die
          */
         virtual bool dieIfEqualTo(DiagramObject * o);
};

#endif // ATTACHEDCOMMENT_H
