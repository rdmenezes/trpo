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

/*! \class Arrow
 */
class Arrow : public DiagramObject
{
private:
        ObjectConnector * m_self;     //!< Self object connector
        ObjectConnector * m_input;    //!< Input object connector
        ObjectConnector * m_output;   //!< Output object connector
        bool              m_tunneled_begin; //!< Tunneled object connector
        bool              m_tunneled_end;   //!< Whether object is connected at  end
        /*! Constructs an input roundings
            \param[in] p1     point
            \param[in] angle  angular points
         */
        void constructInputRounding(QRectF & p1, qreal angle);
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
        /*! Translate an object
         */
        inline QPointF translate( const QPointF & p ) { return p-pos(); }
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
        inline void setInput(ObjectConnector * input)  { m_input=input; }
        /*! Sets an output connector
            \param[in]  output output connector
         */
        inline void setOutput(ObjectConnector * output) { m_output=output; }
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
        /*! Destructs an arrow
         */
        ~Arrow();
};

#endif // ARROW_H
