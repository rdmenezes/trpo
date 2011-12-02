/*!  \file diagramobjecttest.h
     \author Mamontov
     Declares a test class for diagram object
 */
#ifndef DIAGRAMOBJECTTEST_H
#define DIAGRAMOBJECTTEST_H
#include "diagramobject.h"
#include <QPainter>
#include <QImage>

/*! \class DiagramObjectTest
    A test object for diagram test
 */
class DiagramObjectTest : public DiagramObject
{
private:
        QImage  m_img;  //!< Image
protected:
        /*! Paints an object
         */
        void paint(QPainter * p);
public:
        /*!  Creates a diagram object
             \param[in] p point
             \param[in] d diagram
         */
        inline DiagramObjectTest(const QPointF & p,Diagram *d)
            :  DiagramObject(ST_NONE)
        {
            setDiagram(d);
            setPos(p);
            m_img=QImage("s25.gif");
        }
        /*! Boundary rectangle
            \return rectangle
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
};

#endif // DIAGRAMOBJECTTEST_H
