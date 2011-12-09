/*! \file   arrow.h
    \author Mamontov
    Describes an arrow, which consists of objects
 */

#ifndef ARROW_H
#define ARROW_H

#include "diagramobject.h"
#include "diagramscene.h"
#include "objectconnector.h"
#include "drawingconstants.h"
#include "itemtypes.h"
#include <math.h>
#include "saveload.h"

#define POSITION_SENSIVITY 4

/*!  Returns 1D positions of point
     \param[in] l line
     \param[in] p point
     \return 1D positions
 */
inline qreal position(const QLineF & l,
                      const QPointF & p)
{
  if ( fabs(l.x2()-p.x())<POSITION_SENSIVITY
       && fabs(l.y2()-p.y())<POSITION_SENSIVITY)
         return 1;
  if ( fabs(l.x1()-p.x())<POSITION_SENSIVITY
       && fabs(l.y1()-p.y())<POSITION_SENSIVITY)
         return 0;

  QLineF unitVector=l.unitVector();
  QPointF unit(unitVector.dx(),unitVector.dy());

  qreal begpos=unit.x()*l.x1()+unit.y()*l.y1();
  qreal endpos=unit.x()*l.x2()+unit.y()*l.y2();

  if (fabs(endpos-begpos)<0.001) return 1;

  qreal pos=unit.x()*p.x()+unit.y()*p.y();

  return (pos-begpos)/(endpos-begpos);
}

#include <QMessageBox>

inline QPointF position(const QLineF & l,
                        qreal p)
{
 QLineF unitVector=l.unitVector();
 QPointF unit(unitVector.dx(),unitVector.dy());

 qreal begpos=unit.x()*l.x1()+unit.y()*l.y1();
 qreal endpos=unit.x()*l.x2()+unit.y()*l.y2();

 qreal pos=p*(endpos-begpos);

 return l.p1()+QPointF(unit.x()*pos,unit.y()*pos);
}

/*! \class Arrow
 */
class Arrow : public DiagramObject
{
private:
        ObjectConnector * m_self;     //!< Self object connector
        bool              m_tunneled_begin; //!< Tunneled object connector
        bool              m_tunneled_end;   //!< Whether object is connected at  end
        /*! Constructs an input roundings
            \param[in] input  input line
            \param[in] pivot  pivot point
            \param[in] p1     point
            \param[in] angle  angular points
         */
        void constructInputRounding(ObjectConnector * input,const QPointF & pivot,QRectF & p1, qreal & angle);
        /*! Draws a brackets
            \param[in] p painter
            \param[in] point point
            \param[in] bracket bracket
         */
        void drawBracket(QPainter * p,const QPointF & point, qreal angle);
protected:
        /*! Paints an object
            \param[in] p painter
         */
        void paint(QPainter * p);
        /*! Scene drawing bounds
         */
        QRectF sceneDrawingBounds() const;
public:
        /*! Default constructor, used by SerializableFactory
         */
        inline Arrow() : DiagramObject(ST_LINE) {}
        /*! Creates an arrow with following output lines
            \param[in] self      self connector
            \param[in] d         diagram
            \param[in] tunneled     whether arrow is tunneled at begin
            \param[in] tunneled_end whether arrow is tunneled at end
         */
        Arrow(ObjectConnector * self, Diagram * d, bool tunneled=false, bool tunneled_end=false);
        /*! Sets an input connector
            \param[in] input input connector
         */
        inline bool addInput(ObjectConnector * input,qreal point)  { return m_self->addConnector(input,point,C_INPUT); }
        /*! Sets an output connector
            \param[in]  output output connector
         */
        inline bool addOutput(ObjectConnector * output,qreal point) { return m_self->addConnector(output,point,C_OUTPUT);}
        /*! Toggles a tunneled state of object
         */
        inline void toggleTunneledBegin()   { m_tunneled_begin=!m_tunneled_begin; update(); }
        /*! Toggles a tunneled state of object
         */
        inline void toggleTunneledEnd()   { m_tunneled_end=!m_tunneled_end; update(); }
        /*! Returns a bounding rectangle
         */
        QRectF  boundingRect() const;

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
        /*! Draws a rounding data
         */
        bool drawRoundings() const;
        /*! Returns a type of box item
         */
        int type() const;
        /*! Returns a collision rect for item
         */
        virtual QLineF  collisionLine() const;
        /*! Clones a diagram object
         */
        virtual DiagramObject * clone();
        /*! Returns an object connector
            \return object connector
         */
        inline ObjectConnector * model() {return m_self;}
        /*! Sets an arrow parameters
            \param[in] x1 x first point
            \param[in] y1 y first point
            \param[in] x2 x second point
            \param[in] y2 y second point
         */
        void setLine(qreal x1, qreal y1, qreal x2, qreal y2);
        /*! Regenerates a bounding rect for an items
         */
        void regenerate();
        /*! Tests, whether arrow is under cursor
         */
        bool contains ( const QPointF & point ) const;
        /*! Kills self if equal to object
            \param[in] o object to compare
            \return true if will die
         */
        virtual bool dieIfEqualTo(DiagramObject * o);
        /*! Destructs an arrow
         */
        ~Arrow();
};

#endif // ARROW_H
