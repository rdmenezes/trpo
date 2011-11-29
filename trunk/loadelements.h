/*! \file loadelement.h
    \author Mamontov
    Describes a functions of loading elements to diagram
 */
#ifndef LOADELEMENTS_H
#define LOADELEMENTS_H
#include <QDomDocument>
#include <QDomElement>
#include <QDomAttr>
#include <QHash>

class Box;
class ALineItem;
class ArrowSegment;
class ArrowPoint;
class Diagram;

//Loading data
class DiagramLoadData
{
 public:
        Diagram * diagram;
        QHash<void *,Box *>        blocks;
        QHash<void *,ALineItem  * >    annotationlines;
        QHash<void *,ArrowSegment *>   segments;
        QHash<void *,ArrowPoint   *>   points;
};
/*! Loads a block
    \param[in] el element
    \param[in] data data
 */
void loadBlock(QDomElement * el,DiagramLoadData * data);
/*! Loads annotation line
    \param[in] el element
    \param[in] data data
 */
void loadAnnotationLine(QDomElement * el,DiagramLoadData *  data);
/*! Loads annotation label
    \param[in] el element
    \param[in] data data
 */
void loadAnnotationLabel(QDomElement * el,DiagramLoadData * data);
/*! Loads a segment
    \param[in] el element
    \param[in] data data
 */
void loadSegment(QDomElement * el,DiagramLoadData * data);
/*! Loads a point
    \param[in] el element
    \param[in] data data
 */
void loadPoint(QDomElement * el,DiagramLoadData * data);
/*! Corrects an item references  of diagram in data
    \param[in] data data of diagrams
 */
void correctItems(DiagramLoadData * data);


#endif // LOADELEMENTS_H
