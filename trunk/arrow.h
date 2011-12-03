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
            \param[in] p1     point
            \param[in] angle  angular points
         */
        void constructInputRounding(ObjectConnector * input,QRectF & p1, qreal & angle);
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
        /*! Destructs an arrow
         */
        ~Arrow();
};

#endif // ARROW_H
